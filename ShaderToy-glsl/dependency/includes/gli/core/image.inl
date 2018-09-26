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
/// @file gli/core/image.inl
/// @date 2011-10-06 / 2013-01-12
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

namespace gli{
namespace detail
{
	size_t imageAddressing(
		storage const & Storage,
		storage::size_type const & LayerOffset, 
		storage::size_type const & FaceOffset, 
		storage::size_type const & LevelOffset);

	size_t texelLinearAdressing(
		dim1_t const & Dimensions,
		dim1_t const & TexelCoord);

	size_t texelLinearAdressing(
		dim2_t const & Dimensions,
		dim2_t const & TexelCoord);

	size_t texelLinearAdressing(
		dim3_t const & Dimensions,
		dim3_t const & TexelCoord);

	size_t texelMortonAdressing(
		dim1_t const & Dimensions,
		dim1_t const & TexelCoord);

	size_t texelMortonAdressing(
		dim2_t const & Dimensions,
		dim2_t const & TexelCoord);

	size_t texelMortonAdressing(
		dim3_t const & Dimensions,
		dim3_t const & TexelCoord);
}//namespace detail

	inline image::image() :
		BaseLayer(0),
		MaxLayer(0),
		BaseFace(0),
		MaxFace(0),
		BaseLevel(0),
		MaxLevel(0)
	{}

	inline image::image
	(
		format const & Format,
		dim_type const & Dimensions
	) :
		Storage(
			1, 1, 1,
			Format,
			dim_type(Dimensions)),
		BaseLayer(0),
		MaxLayer(0),
		BaseFace(0),
		MaxFace(0),
		BaseLevel(0),
		MaxLevel(0)
	{}

	inline image::image
	(
		storage const & Storage,
		size_type BaseLayer,
		size_type MaxLayer,
		size_type BaseFace,
		size_type MaxFace,
		size_type BaseLevel,
		size_type MaxLevel
	) :
		Storage(Storage),
		BaseLayer(BaseLayer),
		MaxLayer(MaxLayer),
		BaseFace(BaseFace),
		MaxFace(MaxFace),
		BaseLevel(BaseLevel),
		MaxLevel(MaxLevel)
	{}

	inline image::operator storage() const
	{
		return this->Storage;
	}

	inline bool image::empty() const
	{
		return this->Storage.empty();
	}

	inline image::size_type image::size() const
	{
		assert(!this->empty());

		return this->Storage.level_size(this->BaseLevel);
	}

	template <typename genType>
	inline image::size_type image::size() const
	{
		assert(sizeof(genType) <= block_size(this->Storage.format()));

		return this->size() / sizeof(genType);
	}

	inline image::dim_type image::dimensions() const
	{
		return image::dim_type(this->Storage.dimensions(this->BaseLevel));
	}

	inline void * image::data()
	{
		assert(!this->empty());

		size_type const offset = detail::imageAddressing(
			this->Storage, this->BaseLayer, this->BaseFace, this->BaseLevel);

		return this->Storage.data() + offset;
	}

	inline void const * image::data() const
	{
		assert(!this->empty());
		
		size_type const offset = detail::imageAddressing(
			this->Storage, this->BaseLayer, this->BaseFace, this->BaseLevel);

		return this->Storage.data() + offset;
	}

	template <typename genType>
	inline genType * image::data()
	{
		assert(!this->empty());
		assert(block_size(this->Storage.format()) >= sizeof(genType));

		return reinterpret_cast<genType *>(this->data());
	}

	template <typename genType>
	inline genType const * image::data() const
	{
		assert(!this->empty());
		assert(block_size(this->Storage.format()) >= sizeof(genType));

		return reinterpret_cast<genType const *>(this->data());
	}

	inline void image::clear()
	{
		assert(!this->empty());

		memset(this->data<glm::byte>(), 0, this->size<glm::byte>());
	}

	template <typename genType>
	inline void image::clear(genType const & Texel)
	{
		assert(!this->empty());
		assert(block_size(this->Storage.format()) == sizeof(genType));

		for(size_type TexelIndex = 0; TexelIndex < this->size<genType>(); ++TexelIndex)
			*(this->data<genType>() + TexelIndex) = Texel;
	}

	template <typename genType>
	genType image::load(dim_type const & TexelCoord)
	{
		assert(!this->empty());
		assert(block_size(this->Storage.format()) == sizeof(genType));

		return *(this->data<genType>() + detail::texelLinearAdressing(this->dimensions(), TexelCoord));
	}

	template <typename genType>
	void image::store(dim_type const & TexelCoord, genType const & Data)
	{
		assert(!this->empty());
		assert(block_size(this->Storage.format()) == sizeof(genType));

		*(this->data<genType>() + detail::texelLinearAdressing(this->dimensions(), TexelCoord)) = Data;
	}

	inline image::size_type image::baseLayer() const
	{
		return this->BaseLayer;
	}

	inline image::size_type image::maxLayer() const
	{
		return this->MaxLayer;
	}

	inline image::size_type image::baseFace() const
	{
		return this->BaseFace;
	}

	inline image::size_type image::maxFace() const
	{
		return this->MaxFace;
	}

	inline image::size_type image::baseLevel() const
	{
		return this->BaseLevel;
	}

	inline image::size_type image::maxLevel() const
	{
		return this->MaxLevel;
	}
}//namespace gli
