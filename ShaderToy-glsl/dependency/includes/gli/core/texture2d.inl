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
/// @file gli/core/texture2d.inl
/// @date 2010-09-27 / 2012-10-16
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include "levels.hpp"

namespace gli
{
	inline texture2D::texture2D()
	{}

	inline texture2D::texture2D(format_type const & Format, dim_type const & Dimensions)
		: texture(1, 1, gli::levels(Dimensions), Format, storage::dim_type(Dimensions, 1))
	{}

	inline texture2D::texture2D(size_type const & Levels, format_type const & Format, dim_type const & Dimensions)
		: texture(1, 1, Levels, Format, storage::dim_type(Dimensions, 1))
	{}

	inline texture2D::texture2D(storage const & Storage)
		: texture(Storage)
	{}

	inline texture2D::texture2D
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
	
	inline texture2D::texture2D
	(
		texture2D const & Texture,
		size_type const & BaseLevel, size_type const & MaxLevel
	)
		: texture(
			Texture.Storage, Texture.format(),
			Texture.baseLayer(), Texture.maxLayer(),
			Texture.baseFace(), Texture.maxFace(),
			Texture.baseLevel() + BaseLevel, Texture.baseLevel() + MaxLevel)
	{}

	inline texture2D::texture2D
	(
		texture2DArray const & Texture,
		size_type const & BaseLayer,
		size_type const & BaseLevel, size_type const & MaxLevel
	)
		: texture(
			Texture, Texture.format(),
			Texture.baseLayer() + BaseLayer, Texture.baseLayer() + BaseLayer,
			Texture.baseFace(), Texture.maxFace(),
			Texture.baseLevel() + BaseLevel, Texture.baseLevel() + MaxLevel)
	{}

	inline texture2D::texture2D
	(
		textureCube const & Texture,
		size_type const & BaseFace,
		size_type const & BaseLevel, size_type const & MaxLevel
	)
		: texture(
			Texture, Texture.format(),
			Texture.baseLayer(), Texture.maxLayer(),
			Texture.baseFace() + BaseFace, Texture.baseFace() + BaseFace,
			Texture.baseLevel() + BaseLevel, Texture.baseLevel() + MaxLevel)
	{}

	inline texture2D::texture2D
	(
		textureCubeArray const & Texture,
		size_type const & BaseLayer,
		size_type const & BaseFace,
		size_type const & BaseLevel, size_type const & MaxLevel
	)
		: texture(
			Texture, Texture.format(),
			Texture.baseLayer() + BaseLayer, Texture.baseLayer() + BaseLayer,
			Texture.baseFace() + BaseFace, Texture.baseFace() + BaseFace,
			Texture.baseLevel() + BaseLevel, Texture.baseLevel() + MaxLevel)
	{}

	inline texture2D::operator storage() const
	{
		return this->Storage;
	}

	inline image texture2D::operator[](texture2D::size_type const & Level) const
	{
		assert(Level < this->levels());

		return image(
			this->Storage,
			this->baseLayer(), this->maxLayer(),
			this->baseFace(), this->maxFace(),
			this->baseLevel() + Level, this->baseLevel() + Level);
	}

	inline texture2D::dim_type texture2D::dimensions() const
	{
		assert(!this->empty());

		return texture2D::dim_type(this->Storage.dimensions(this->baseLevel()));
	}

	template <typename genType>
	inline genType texture2D::fetch(dim_type const & TexelCoord, size_type const & Level)
	{
		assert(!this->empty());
		assert(!is_compressed(this->format()));
		assert(block_size(this->Storage.format()) >= sizeof(genType));

		dim_type const Dimensions(this->dimensions());
		size_type const Address = TexelCoord.x + TexelCoord.y * Dimensions.x;

		return *(this->data<genType>() + Address);
	}

/*
	template <typename genType>
	inline void texture2D::swizzle(glm::comp X, glm::comp Y, glm::comp Z, glm::comp W)
	{
		for(texture2D::level_type Level = 0; Level < this->levels(); ++Level)
		{
			genType * Data = reinterpret_cast<genType*>(this->Images[Level].data());
			texture2D::size_type Components = this->Images[Level].components();
			//gli::detail::getComponents(this->Images[Level].format());
			texture2D::size_type Size = (glm::compMul(this->Images[Level].dimensions()) * Components) / sizeof(genType);

			for(texture2D::size_type i = 0; i < Size; ++i)
			{
				genType Copy = Data[i];
				if(Components > 0)
					Data[i][0] = Copy[X];
				if(Components > 1)
					Data[i][1] = Copy[Y];
				if(Components > 2)
					Data[i][2] = Copy[Z];
				if(Components > 3)
					Data[i][3] = Copy[W];
			}
		}
	}
*/
/*
	template <typename T>
	inline T texture<T>::texture(float x, float y) const
	{
		size_type x_below = size_type(std::floor(x * (_width - 1)));
		size_type x_above = size_type(std::ceil(x * (_width - 1)));
		size_type y_below = size_type(std::floor(y * (_height - 1)));
		size_type y_above = size_type(std::ceil(y * (_height - 1)));

		float x_step = 1.0f / float(_width);
		float y_step = 1.0f / float(_height);

		float x_below_normalized = float(x_below) / float(_width - 1);
		float x_above_normalized = float(x_above) / float(_width - 1);
		float y_below_normalized = float(y_below) / float(_height - 1);
		float y_above_normalized = float(y_above) / float(_height - 1);
		
		T value1 = _data[x_below + y_below * _width];
		T value2 = _data[x_above + y_below * _width];
		T value3 = _data[x_above + y_above * _width];
		T value4 = _data[x_below + y_above * _width];

		T valueA = glm::mix(value1, value2, x - x_below_normalized);
		T valueB = glm::mix(value4, value3, x - x_below_normalized);
		T valueC = glm::mix(valueA, valueB, y - y_below_normalized);
		return valueC;
	}
*/
/*
	template <typename T>
	inline T texture(const texture2D<T>& Image2D, const glm::vec2& TexCoord)
	{
		texture2D<T>::size_type s_below = texture2D<T>::size_type(std::floor(TexCoord.s * (Image2D.width() - 1)));
		texture2D<T>::size_type s_above = texture2D<T>::size_type(std::ceil(TexCoord.s * (Image2D.width() - 1)));
		texture2D<T>::size_type t_below = texture2D<T>::size_type(std::floor(TexCoord.t * (Image2D.height() - 1)));
		texture2D<T>::size_type t_above = texture2D<T>::size_type(std::ceil(TexCoord.t * (Image2D.height() - 1)));

		glm::vec2::value_type s_step = 1.0f / glm::vec2::value_type(Image2D.width());
		glm::vec2::value_type t_step = 1.0f / glm::vec2::value_type(Image2D.height());

		glm::vec2::value_type s_below_normalized = glm::vec2::value_type(s_below) / glm::vec2::value_type(Image2D.width() - 1);
		glm::vec2::value_type s_above_normalized = glm::vec2::value_type(s_above) / glm::vec2::value_type(Image2D.width() - 1);
		glm::vec2::value_type t_below_normalized = glm::vec2::value_type(t_below) / glm::vec2::value_type(Image2D.height() - 1);
		glm::vec2::value_type t_above_normalized = glm::vec2::value_type(t_above) / glm::vec2::value_type(Image2D.height() - 1);
		
		T value1 = Image2D[s_below + t_below * Image2D.width()];
		T value2 = Image2D[s_above + t_below * Image2D.width()];
		T value3 = Image2D[s_above + t_above * Image2D.width()];
		T value4 = Image2D[s_below + t_above * Image2D.width()];

		T valueA = glm::mix(value1, value2, TexCoord.s - s_below_normalized);
		T valueB = glm::mix(value4, value3, TexCoord.s - s_below_normalized);
		T valueC = glm::mix(valueA, valueB, TexCoord.t - t_below_normalized);
		return valueC;
	}

	template <typename T>
	inline T textureNearest(const texture2D<T>& Image2D, const glm::vec2& TexCoord)
	{
		texture2D<T>::size_type s = texture2D<T>::size_type(glm::roundGTX(TexCoord.s * (Image2D.width() - 1)));
		texture2D<T>::size_type t = texture2D<T>::size_type(std::roundGTX(TexCoord.t * (Image2D.height() - 1)));

		return Image2D[s + t * Image2D.width()];
	}
*/

}//namespace gli
