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
/// @file gli/core/texture1d_array.hpp
/// @date 2012-06-25 / 2013-01-11
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "texture1d.hpp"

namespace gli
{
	class texture1D;

	class texture1DArray : public texture
	{
	public:
		typedef dim1_t dim_type;
		typedef vec1 texcoord_type;

	public:
		texture1DArray();

		/// Create a texture1DArray and allocate a new storage
		explicit texture1DArray(
			size_type const & Layers,
			size_type const & Levels,
			format_type const & Format,
			dim_type const & Dimensions);

		/// Create a texture1DArray and allocate a new storage with a complete mipmap chain
		explicit texture1DArray(
			size_type const & Layers,
			format_type const & Format,
			dim_type const & Dimensions);

		/// Create a texture1DArray view with an existing storage
		explicit texture1DArray(
			storage const & Storage);

		/// Create a texture1DArray view with an existing storage
		explicit texture1DArray(
			storage const & Storage,
			format_type const & Format,
			size_type BaseLayer, size_type MaxLayer,
			size_type BaseFace, size_type MaxFace,
			size_type BaseLevel, size_type MaxLevel);

		/// Create a texture view, reference a subset of an exiting storage
		explicit texture1DArray(
			texture1DArray const & Texture,
			size_type const & BaseLayer, size_type const & MaxLayer,
			size_type const & BaseLevel, size_type const & MaxLevel);

		/// Create a texture view, reference a subset of an exiting texture1D instance
		explicit texture1DArray(
			texture1D const & Texture,
			size_type const & BaseLevel, size_type const & MaxLevel);

		operator storage() const;
		texture1D operator[](size_type const & Layer) const;

		dim_type dimensions() const;
	};
}//namespace gli


