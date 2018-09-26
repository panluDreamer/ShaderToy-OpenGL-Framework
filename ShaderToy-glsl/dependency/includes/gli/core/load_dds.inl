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
/// @file gli/core/load_dds.inl
/// @date 2010-09-26 / 2015-06-16
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cassert>

namespace gli{
namespace detail
{
	enum ddsCubemapflag
	{
		DDSCAPS2_CUBEMAP				= 0x00000200,
		DDSCAPS2_CUBEMAP_POSITIVEX		= 0x00000400,
		DDSCAPS2_CUBEMAP_NEGATIVEX		= 0x00000800,
		DDSCAPS2_CUBEMAP_POSITIVEY		= 0x00001000,
		DDSCAPS2_CUBEMAP_NEGATIVEY		= 0x00002000,
		DDSCAPS2_CUBEMAP_POSITIVEZ		= 0x00004000,
		DDSCAPS2_CUBEMAP_NEGATIVEZ		= 0x00008000,
		DDSCAPS2_VOLUME					= 0x00200000
	};

	enum
	{
		DDSCAPS2_CUBEMAP_ALLFACES = DDSCAPS2_CUBEMAP_POSITIVEX | DDSCAPS2_CUBEMAP_NEGATIVEX | DDSCAPS2_CUBEMAP_POSITIVEY | DDSCAPS2_CUBEMAP_NEGATIVEY | DDSCAPS2_CUBEMAP_POSITIVEZ | DDSCAPS2_CUBEMAP_NEGATIVEZ
	};

	enum ddsFlag
	{
		DDSD_CAPS			= 0x00000001,
		DDSD_HEIGHT			= 0x00000002,
		DDSD_WIDTH			= 0x00000004,
		DDSD_PITCH			= 0x00000008,
		DDSD_PIXELFORMAT	= 0x00001000,
		DDSD_MIPMAPCOUNT	= 0x00020000,
		DDSD_LINEARSIZE		= 0x00080000,
		DDSD_DEPTH			= 0x00800000
	};

	enum ddsSurfaceflag
	{
		DDSCAPS_COMPLEX				= 0x00000008,
		DDSCAPS_MIPMAP				= 0x00400000,
		DDSCAPS_TEXTURE				= 0x00001000
	};

	struct ddsPixelFormat
	{
		glm::uint32 size; // 32
		dx::DDPF flags;
		dx::D3DFORMAT fourCC;
		glm::uint32 bpp;
		glm::u32vec4 Mask;
	};

	struct ddsHeader
	{
		char Magic[4];
		glm::uint32 size;
		glm::uint32 flags;
		glm::uint32 height;
		glm::uint32 width;
		glm::uint32 pitch;
		glm::uint32 depth;
		glm::uint32 mipMapLevels;
		glm::uint32 reserved1[11];
		ddsPixelFormat format;
		glm::uint32 surfaceFlags;
		glm::uint32 cubemapFlags;
		glm::uint32 reserved2[3];
	};

	enum D3D10_RESOURCE_DIMENSION 
	{
		D3D10_RESOURCE_DIMENSION_UNKNOWN     = 0,
		D3D10_RESOURCE_DIMENSION_BUFFER      = 1,
		D3D10_RESOURCE_DIMENSION_TEXTURE1D   = 2,
		D3D10_RESOURCE_DIMENSION_TEXTURE2D   = 3,
		D3D10_RESOURCE_DIMENSION_TEXTURE3D   = 4 
	};

	enum D3D10_RESOURCE_MISC_FLAG
	{
		D3D10_RESOURCE_MISC_GENERATE_MIPS		= 0x01,
		D3D10_RESOURCE_MISC_SHARED				= 0x02,
		D3D10_RESOURCE_MISC_TEXTURECUBE			= 0x04,
		D3D10_RESOURCE_MISC_SHARED_KEYEDMUTEX	= 0x10,
		D3D10_RESOURCE_MISC_GDI_COMPATIBLE		= 0x20,
	};

	enum
	{
		DDS_ALPHA_MODE_UNKNOWN					= 0x0,
		DDS_ALPHA_MODE_STRAIGHT					= 0x1,
		DDS_ALPHA_MODE_PREMULTIPLIED			= 0x2,
		DDS_ALPHA_MODE_OPAQUE					= 0x3,
		DDS_ALPHA_MODE_CUSTOM					= 0x4
	};

	struct ddsHeader10
	{
		ddsHeader10() :
			Format(dx::DXGI_FORMAT_UNKNOWN),
			resourceDimension(D3D10_RESOURCE_DIMENSION_UNKNOWN),
			miscFlag(0),
			arraySize(1),
			reserved(0)
		{}

		dx::dxgiFormat				Format;
		D3D10_RESOURCE_DIMENSION	resourceDimension;
		glm::uint32					miscFlag; // D3D10_RESOURCE_MISC_GENERATE_MIPS
		glm::uint32					arraySize;
		glm::uint32					reserved;
	};
}//namespace detail

inline storage load_dds(char const * Data, std::size_t Size)
{
	assert(Data && (Size >= (sizeof(char[4]) + sizeof(detail::ddsHeader))));

	detail::ddsHeader const & HeaderDesc(*reinterpret_cast<detail::ddsHeader const *>(Data));
	std::size_t Offset = sizeof(detail::ddsHeader);

	assert(strncmp(HeaderDesc.Magic, "DDS ", 4) == 0);

	detail::ddsHeader10 HeaderDesc10;
	if(HeaderDesc.format.flags & dx::DDPF_FOURCC && HeaderDesc.format.fourCC == dx::D3DFMT_DX10)
	{
		std::memcpy(&HeaderDesc10, Data + Offset, sizeof(HeaderDesc10));
		Offset += sizeof(detail::ddsHeader10);
	}

	dx DX;

	gli::format Format(static_cast<gli::format>(gli::FORMAT_INVALID));
	if((HeaderDesc.format.flags & (dx::DDPF_RGB | dx::DDPF_ALPHAPIXELS | dx::DDPF_ALPHA | dx::DDPF_YUV | dx::DDPF_LUMINANCE)) && Format == static_cast<format>(gli::FORMAT_INVALID) && HeaderDesc.format.flags != dx::DDPF_FOURCC_ALPHAPIXELS)
	{
		switch(HeaderDesc.format.bpp)
		{
			case 8:
			{
				if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_L8_UNORM).Mask)))
					Format = FORMAT_L8_UNORM;
				else if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_A8_UNORM).Mask)))
					Format = FORMAT_A8_UNORM;
				else if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_R8_UNORM).Mask)))
					Format = FORMAT_R8_UNORM;
				else if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_RG3B2_UNORM).Mask)))
					Format = FORMAT_RG3B2_UNORM;
				break;
			}
			case 16:
			{
				if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_LA8_UNORM).Mask)))
					Format = FORMAT_LA8_UNORM;
				else if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_RG8_UNORM).Mask)))
					Format = FORMAT_RG8_UNORM;
				else if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_R5G6B5_UNORM).Mask)))
					Format = FORMAT_R5G6B5_UNORM;
				else if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_L16_UNORM).Mask)))
					Format = FORMAT_L16_UNORM;
				else if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_A16_UNORM).Mask)))
					Format = FORMAT_A16_UNORM;
				else if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_R16_UNORM).Mask)))
					Format = FORMAT_R16_UNORM;
				else if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_RGB5A1_UNORM).Mask)))
					Format = FORMAT_RGB5A1_UNORM;
				break;
			}
			case 24:
			{
				dx::format const & DXFormat = DX.translate(FORMAT_RGB8_UNORM);
				if(glm::all(glm::equal(HeaderDesc.format.Mask, DXFormat.Mask)))
					Format = FORMAT_RGB8_UNORM;
				break;
			}
			case 32:
			{
				if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_BGRX8_UNORM).Mask)))
					Format = FORMAT_BGRX8_UNORM;
				else if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_BGRA8_UNORM).Mask)))
					Format = FORMAT_BGRA8_UNORM;
				else if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_RGB10A2_UNORM).Mask)))
					Format = FORMAT_RGB10A2_UNORM;
				else if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_LA16_UNORM).Mask)))
					Format = FORMAT_LA16_UNORM;
				else if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_RG16_UNORM).Mask)))
					Format = FORMAT_RG16_UNORM;
				else if(glm::all(glm::equal(HeaderDesc.format.Mask, DX.translate(FORMAT_R32_SFLOAT).Mask)))
					Format = FORMAT_R32_SFLOAT;
			}
			break;
		}
	}
	else if((HeaderDesc.format.flags & dx::DDPF_FOURCC) && (HeaderDesc.format.fourCC != dx::D3DFMT_DX10) && (Format == static_cast<format>(gli::FORMAT_INVALID)))
		Format = DX.find(HeaderDesc.format.fourCC);
	else if((HeaderDesc.format.fourCC == dx::D3DFMT_DX10) && (HeaderDesc10.Format != dx::DXGI_FORMAT_UNKNOWN))
		Format = DX.find(HeaderDesc10.Format);

	assert(Format != static_cast<format>(gli::FORMAT_INVALID));

	storage::size_type const MipMapCount = (HeaderDesc.flags & detail::DDSD_MIPMAPCOUNT) ? HeaderDesc.mipMapLevels : 1;
	storage::size_type FaceCount(1);
	if(HeaderDesc.cubemapFlags & detail::DDSCAPS2_CUBEMAP)
		FaceCount = int(glm::bitCount(HeaderDesc.cubemapFlags & detail::DDSCAPS2_CUBEMAP_ALLFACES));

	storage::size_type DepthCount = 1;
	if(HeaderDesc.cubemapFlags & detail::DDSCAPS2_VOLUME)
		DepthCount = HeaderDesc.depth;

	storage Storage(
		HeaderDesc10.arraySize, FaceCount, MipMapCount, Format,
		storage::dim_type(HeaderDesc.width, HeaderDesc.height, DepthCount));

	assert(Offset + Storage.size() == Size);

	std::memcpy(Storage.data(), Data + Offset, Storage.size());

	return Storage;
}

inline storage load_dds(char const * Filename)
{
	FILE* File = std::fopen(Filename, "rb");
	assert(File);

	long Beg = std::ftell(File);
	std::fseek(File, 0, SEEK_END);
	long End = std::ftell(File);
	std::fseek(File, 0, SEEK_SET);

	std::vector<char> Data(static_cast<std::size_t>(End - Beg));

	std::fread(&Data[0], 1, Data.size(), File);
	std::fclose(File);

	return load_dds(&Data[0], Data.size());
}

inline storage load_dds(std::string const & Filename)
{
	return load_dds(Filename.c_str());
}

}//namespace gli
