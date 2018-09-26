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
/// @file gli/core/storage.inl
/// @date 2012-06-21 / 2013-01-12
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

namespace gli
{
	inline storage::impl::impl() :
		Layers(0),
		Faces(0),
		Levels(0),
		Format(static_cast<gli::format>(FORMAT_INVALID)),
		Dimensions(0)
	{}

	inline storage::impl::impl(
		size_type const & Layers, 
		size_type const & Faces,
		size_type const & Levels,
		format_type const & Format,
		dim_type const & Dimensions
	)
	:	Layers(Layers),
		Faces(Faces),
		Levels(Levels),
		Format(Format),
		Dimensions(Dimensions)
	{}

	inline storage::storage()
	{}

	inline storage::storage
	(
		size_type const & Layers,
		size_type const & Faces,
		size_type const & Levels,
		format_type const & Format,
		dim_type const & Dimensions
	) :
		Impl(new impl(
			Layers,
			Faces,
			Levels,
			Format,
			Dimensions))
	{
		assert(Layers > 0);
		assert(Faces > 0);
		assert(Levels > 0);
		assert(glm::all(glm::greaterThan(Dimensions, dim_type(0))));

		Impl->Data.resize(this->layer_size(0, Faces - 1, 0, Levels - 1) * Layers, 0);
	}

	inline bool storage::empty() const
	{
		if(this->Impl.get() == 0)
			return true;
		return this->Impl->Data.empty();
	}

	inline storage::format_type storage::format() const
	{
		return this->Impl->Format;
	}

	inline storage::size_type storage::layers() const
	{
		return this->Impl->Layers;
	}

	inline storage::size_type storage::faces() const
	{
		return this->Impl->Faces;
	}

	inline storage::size_type storage::levels() const
	{
		return this->Impl->Levels;
	}

	inline storage::dim_type storage::dimensions(size_type const & Level) const
	{
		assert(Level < this->Impl->Levels);

		return glm::max(this->Impl->Dimensions >> storage::dim_type(static_cast<glm::uint>(Level)), storage::dim_type(static_cast<glm::uint>(1)));
	}

	inline storage::size_type storage::size() const
	{
		assert(!this->empty());

		return this->Impl->Data.size();
	}

	inline storage::data_type const * storage::data() const
	{
		assert(!this->empty());

		return &this->Impl->Data[0];
	}

	inline storage::data_type * storage::data()
	{
		assert(!this->empty());

		return &this->Impl->Data[0];
	}

	inline storage::size_type storage::level_size(size_type const & Level) const
	{
		assert(Level < this->levels());

		dim_type const BlockDimensions(gli::block_dimensions_x(this->format()), gli::block_dimensions_y(this->format()), gli::block_dimensions_z(this->format()));
		dim_type const Dimensions = this->dimensions(Level);
		dim_type const Multiple = glm::ceilMultiple(Dimensions, BlockDimensions);
		std::size_t const BlockSize = gli::block_size(this->format());

		return BlockSize * glm::compMul(Multiple / BlockDimensions);
	}

	inline storage::size_type storage::face_size(
		size_type const & BaseLevel,
		size_type const & MaxLevel) const
	{
		assert(MaxLevel < this->levels());
		
		size_type FaceSize(0);

		// The size of a face is the sum of the size of each level.
		for(storage::size_type Level(BaseLevel); Level <= MaxLevel; ++Level)
			FaceSize += this->level_size(Level);

		return FaceSize;// * TexelSize;
	}

	inline storage::size_type storage::layer_size(
		size_type const & BaseFace,
		size_type const & MaxFace,
		size_type const & BaseLevel,
		size_type const & MaxLevel) const
	{
		assert(MaxFace < this->faces());
		assert(MaxLevel < this->levels());

		// The size of a layer is the sum of the size of each face.
		// All the faces have the same size.
		return this->face_size(BaseLevel, MaxLevel) * (MaxFace - BaseFace + 1);
	}

/*
	inline storage extractLayers
	(
		storage const & Storage, 
		storage::size_type const & Offset, 
		storage::size_type const & Size
	)
	{
		assert(Storage.layers() > 1);
		assert(Storage.layers() >= Size);
		assert(Storage.faces() > 0);
		assert(Storage.levels() > 0);

		storage SubStorage(
			Size, 
			Storage.faces(), 
			Storage.levels(),
			Storage.dimensions(0),
			Storage.blockSize());

		memcpy(
			SubStorage.data(), 
			Storage.data() + Storage.imageAddressing(Offset, 0, 0), 
			Storage.layerSize() * Size);

		return SubStorage;
	}
*/
/*
	inline storage extractFace
	(
		storage const & Storage, 
		face const & Face
	)
	{
		assert(Storage.faces() > 1);
		assert(Storage.levels() > 0);

		storage SubStorage(
			Storage.layers(),
			Face, 
			Storage.levels(),
			Storage.dimensions(0),
			Storage.blockSize());

		memcpy(
			SubStorage.data(), 
			Storage.data() + Storage.imageAddressing(0, storage::size_type(Face), 0), 
			Storage.faceSize());

		return SubStorage;
	}
*/
/*
	inline storage extractLevel
	(
		storage const & Storage, 
		storage::size_type const & Level
	)
	{
		assert(Storage.layers() == 1);
		assert(Storage.faces() == 1);
		assert(Storage.levels() >= 1);

		storage SubStorage(
			1, // layer
			glm::uint(FACE_DEFAULT),
			1, // level
			Storage.dimensions(0),
			Storage.blockSize());

		memcpy(
			SubStorage.data(), 
			Storage.data() + Storage.imageAddressing(0, 0, Level), 
			Storage.levelSize(Level));

		return SubStorage;
	}
*/
/*
	inline void copy_layers
	(
		storage const & SourceStorage, 
		storage::size_type const & SourceLayerOffset,
		storage::size_type const & SourceLayerSize,
		storage & DestinationStorage, 
		storage::size_type const & DestinationLayerOffset
	)
	{
		assert(DestinationStorage.blockSize() == SourceStorage.blockSize());
		assert(DestinationStorage.layers() <= SourceStorage.layers());
		assert(SourceStorage.layers() <= SourceLayerOffset + SourceLayerSize);
		assert(DestinationStorage.layers() <= DestinationLayerOffset + SourceLayerSize);

		std::size_t OffsetSrc = SourceStorage.imageAddressing(SourceLayerOffset, 0, 0);
		std::size_t OffsetDst = DestinationStorage.imageAddressing(DestinationLayerOffset, 0, 0);

		memcpy(
			DestinationStorage.data() + OffsetDst * DestinationStorage.blockSize(), 
			SourceStorage.data() + OffsetSrc * SourceStorage.blockSize(), 
			SourceStorage.layerSize() * SourceLayerSize * SourceStorage.blockSize());
	}
*/
}//namespace gli
