///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2015 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref core
/// @file gli/core/save_dds.inl
/// @date 2013-01-28 / 2013-01-28
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <cstdio>

namespace gli
{
	inline void save_dds(storage const & Storage, std::vector<char> & Memory)
	{
		if(Storage.empty())
			return;

		dx DX;
		dx::format const & DXFormat = DX.translate(Storage.format());

		dx::D3DFORMAT const FourCC = Storage.layers() > 1 ? dx::D3DFMT_DX10 : DXFormat.D3DFormat;

		Memory.resize(Storage.size() + sizeof(detail::ddsHeader) + (FourCC == dx::D3DFMT_DX10 ? sizeof(detail::ddsHeader10) : 0));

		detail::ddsHeader & HeaderDesc = *reinterpret_cast<detail::ddsHeader*>(&Memory[0]);

		detail::format_info const & Desc = detail::getFormatInfo(Storage.format());

		glm::uint32 Caps = detail::DDSD_CAPS | detail::DDSD_WIDTH | detail::DDSD_PIXELFORMAT | detail::DDSD_MIPMAPCOUNT;
		Caps |= Storage.dimensions(0).y > 1 ? detail::DDSD_HEIGHT : 0;
		Caps |= Storage.dimensions(0).z > 1 ? detail::DDSD_DEPTH : 0;
		//Caps |= Storage.levels() > 1 ? detail::DDSD_MIPMAPCOUNT : 0;
		Caps |= (Desc.Flags & detail::CAP_COMPRESSED_BIT) ? detail::DDSD_LINEARSIZE : detail::DDSD_PITCH;

		memcpy(HeaderDesc.Magic, "DDS ", sizeof(char) * 4);
		memset(HeaderDesc.reserved1, 0, sizeof(HeaderDesc.reserved1));
		memset(HeaderDesc.reserved2, 0, sizeof(HeaderDesc.reserved2));
		HeaderDesc.size = sizeof(detail::ddsHeader);
		HeaderDesc.flags = Caps;
		assert(Storage.dimensions(0).x < std::numeric_limits<glm::uint32>::max());
		HeaderDesc.width = static_cast<glm::uint32>(Storage.dimensions(0).x);
		assert(Storage.dimensions(0).y < std::numeric_limits<glm::uint32>::max());
		HeaderDesc.height = static_cast<glm::uint32>(Storage.dimensions(0).y);
		HeaderDesc.pitch = glm::uint32((Desc.Flags & detail::CAP_COMPRESSED_BIT) ? Storage.size() / Storage.faces() : 32);
		assert(Storage.dimensions(0).z < std::numeric_limits<glm::uint32>::max());
		HeaderDesc.depth = static_cast<glm::uint32>(Storage.dimensions(0).z > 1 ? Storage.dimensions(0).z : 0);
		HeaderDesc.mipMapLevels = glm::uint32(Storage.levels());
		HeaderDesc.format.size = sizeof(detail::ddsPixelFormat);
		HeaderDesc.format.flags = Storage.layers() > 1 ? dx::DDPF_FOURCC : DXFormat.DDPixelFormat;
		HeaderDesc.format.fourCC = Storage.layers() > 1 ? dx::D3DFMT_DX10 : DXFormat.D3DFormat;
		HeaderDesc.format.bpp = glm::uint32(detail::bits_per_pixel(Storage.format()));
		HeaderDesc.format.Mask = DXFormat.Mask;
		//HeaderDesc.surfaceFlags = detail::DDSCAPS_TEXTURE | (Storage.levels() > 1 ? detail::DDSCAPS_MIPMAP : 0);
		HeaderDesc.surfaceFlags = detail::DDSCAPS_TEXTURE | detail::DDSCAPS_MIPMAP;
		HeaderDesc.cubemapFlags = 0;

		// Cubemap
		if(Storage.faces() > 1)
		{
			assert(Storage.faces() == 6);
			HeaderDesc.cubemapFlags |= detail::DDSCAPS2_CUBEMAP_ALLFACES | detail::DDSCAPS2_CUBEMAP;
		}

		// Texture3D
		if(Storage.dimensions(0).z > 1)
			HeaderDesc.cubemapFlags |= detail::DDSCAPS2_VOLUME;

		storage::size_type DepthCount = 1;
		if(HeaderDesc.cubemapFlags & detail::DDSCAPS2_VOLUME)
				DepthCount = HeaderDesc.depth;

		std::size_t Offset = sizeof(detail::ddsHeader);
		if(HeaderDesc.format.fourCC == dx::D3DFMT_DX10)
		{
			detail::ddsHeader10 & HeaderDesc10 = *reinterpret_cast<detail::ddsHeader10*>(&Memory[0] + sizeof(detail::ddsHeader));
			Offset += sizeof(detail::ddsHeader10);

			HeaderDesc10.arraySize = glm::uint32(Storage.layers());
			HeaderDesc10.resourceDimension = detail::D3D10_RESOURCE_DIMENSION_TEXTURE2D;
			HeaderDesc10.miscFlag = 0;//Storage.levels() > 0 ? detail::D3D10_RESOURCE_MISC_GENERATE_MIPS : 0;
			HeaderDesc10.Format = static_cast<dx::dxgiFormat>(DXFormat.DXGIFormat);
			HeaderDesc10.reserved = 0;
		}

		std::memcpy(&Memory[0] + Offset, Storage.data(), Storage.size());
	}

	inline void save_dds(storage const & Storage, char const * Filename)
	{
		if(Storage.empty())
			return;

		FILE* File = std::fopen(Filename, "wb");
		if (!File)
			return;

		std::vector<char> Memory;
		save_dds(Storage, Memory);

		std::fwrite(&Memory[0], 1, Memory.size(), File);
		std::fclose(File);
	}

	inline void save_dds(storage const & Storage, std::string const & Filename)
	{
		save_dds(Storage, Filename.c_str());
	}
}//namespace gli
