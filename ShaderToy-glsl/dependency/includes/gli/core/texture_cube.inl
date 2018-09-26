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
/// @file gli/core/texture_cube.inl
/// @date 2011-04-06 / 2012-12-12
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

namespace gli
{
	inline textureCube::textureCube()
	{}

	inline textureCube::textureCube
	(
		size_type const & Faces,
		format_type const & Format,
		dim_type const & Dimensions
	)
		: texture(1, Faces, gli::levels(Dimensions), Format, storage::dim_type(Dimensions, 1))
	{}

	inline textureCube::textureCube
	(
		size_type const & Faces,
		size_type const & Levels,
		format_type const & Format,
		dim_type const & Dimensions
	)
		: texture(1, Faces, Levels, Format, storage::dim_type(Dimensions, 1))
	{}

	inline textureCube::textureCube(storage const & Storage)
		: texture(Storage)
	{}

	inline textureCube::textureCube
	(
		storage const & Storage,
		format_type const & Format,
		size_type BaseLayer, size_type MaxLayer,
		size_type BaseFace, size_type MaxFace,
		size_type BaseLevel, size_type MaxLevel
	)
		: texture(
			Storage, Format,
			BaseLayer, MaxLayer,
			BaseFace, MaxFace,
			BaseLevel, MaxLevel)
	{}

	inline textureCube::textureCube
	(
		textureCube const & Texture,
		size_type const & BaseFace, size_type const & MaxFace,
		size_type const & BaseLevel, size_type const & MaxLevel
	)
		: texture(
			Texture, Texture.format(),
			Texture.baseLayer(), Texture.maxLayer(),
			Texture.baseFace() + BaseFace, Texture.baseFace() + MaxFace,
			Texture.baseLevel() + BaseLevel, Texture.baseLevel() + MaxLevel)
	{}

	inline textureCube::textureCube
	(
		textureCubeArray const & Texture,
		size_type const & BaseLayer,
		size_type const & BaseFace, size_type const & MaxFace,
		size_type const & BaseLevel, size_type const & MaxLevel
	)
		: texture(
			Texture, Texture.format(),
			Texture.baseLayer() + BaseLayer, Texture.baseLayer() + BaseLayer,
			Texture.baseFace() + BaseFace, Texture.baseFace() + MaxFace,
			Texture.baseLevel() + BaseLevel, Texture.baseLevel() + MaxLevel)
	{}

	inline textureCube::textureCube
	(
		texture2D const & Texture,
		size_type const & BaseLevel,
		size_type const & MaxLevel
	)
		: texture(
			Texture, Texture.format(),
			Texture.baseLayer(), Texture.maxLayer(),
			Texture.baseFace(), Texture.maxFace(),
			Texture.baseLevel() + BaseLevel, Texture.baseLevel() + MaxLevel)
	{}

	inline textureCube::operator storage() const
	{
		return this->Storage;
	}

	inline texture2D textureCube::operator[](size_type const & Face) const
	{
		assert(Face < this->faces());

		return texture2D(
			this->Storage, this->format(),
			this->baseLayer(), this->maxLayer(),
			this->baseFace() + Face, 	this->baseFace() + Face,
			this->baseLevel(), this->maxLevel());
	}

	inline textureCube::dim_type textureCube::dimensions() const
	{
		assert(!this->empty());

		return textureCube::dim_type(this->Storage.dimensions(this->baseLevel()));
	}
}//namespace gli
