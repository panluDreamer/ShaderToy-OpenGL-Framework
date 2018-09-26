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
/// @file gli/core/texture1d_array.inl
/// @date 2012-06-27 / 2013-01-12
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include "levels.hpp"

namespace gli
{
	inline texture1DArray::texture1DArray()
	{}

	inline texture1DArray::texture1DArray
	(
		size_type const & Layers,
		format_type const & Format,
		dim_type const & Dimensions
	)
		: texture(Layers, 1, gli::levels(Dimensions), Format, storage::dim_type(Dimensions.x, 1, 1))
	{}

	inline texture1DArray::texture1DArray
	(
		size_type const & Layers,
		size_type const & Levels,
		format_type const & Format,
		dim_type const & Dimensions
	)
		: texture(Layers, 1, Levels, Format, storage::dim_type(Dimensions.x, 1, 1))
	{}

	inline texture1DArray::texture1DArray(storage const & Storage)
		: texture(Storage)
	{}

	inline texture1DArray::texture1DArray
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

	inline texture1DArray::texture1DArray
	(
		texture1DArray const & Texture,
		size_type const & BaseLayer, size_type const & MaxLayer,
		size_type const & BaseLevel, size_type const & MaxLevel
	)
		: texture(
			Texture, Texture.format(),
			Texture.baseLayer() + BaseLayer, Texture.baseLayer() + MaxLayer,
			Texture.baseFace(), Texture.maxFace(),
			Texture.baseLevel() + BaseLevel, Texture.baseLevel() + MaxLevel)
	{}

	inline texture1DArray::texture1DArray
	(
		texture1D const & Texture,
		size_type const & BaseLevel, size_type const & MaxLevel
	)
		: texture(
			Texture, Texture.format(),
			Texture.baseLayer(), Texture.maxLayer(),
			Texture.baseFace(), Texture.maxFace(),
			Texture.baseLevel() + BaseLevel, Texture.baseLevel() + MaxLevel)
	{}

	inline texture1DArray::operator storage() const
	{
		return this->Storage;
	}

	inline texture1D texture1DArray::operator[](size_type const & Layer) const
	{
		assert(!this->empty());
		assert(Layer < this->layers());

		return texture1D(
			this->Storage, this->format(),
			this->baseLayer() + Layer, this->baseLayer() + Layer,
			this->baseFace(), 	this->maxFace(),
			this->baseLevel(), this->maxLevel());
	}

	inline texture1DArray::dim_type texture1DArray::dimensions() const
	{
		assert(!this->empty());

		return texture1DArray::dim_type(this->Storage.dimensions(this->baseLevel()).x);
	}
}//namespace gli


