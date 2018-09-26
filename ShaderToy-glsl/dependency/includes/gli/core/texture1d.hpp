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
/// @file gli/core/texture1d.hpp
/// @date 2012-06-25 / 2013-01-11
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "image.hpp"

namespace gli
{
	class texture1DArray;

	class texture1D : public texture
	{
	public:
		typedef dim1_t dim_type;
		typedef vec1 texcoord_type;

	public:
		texture1D();

		/// Create a texture1D and allocate a new storage
		explicit texture1D(
			size_type const & Levels,
			format_type const & Format,
			dim_type const & Dimensions);

		/// Create a texture1D and allocate a new storage with a complete mipmap chain
		explicit texture1D(
			format_type const & Format,
			dim_type const & Dimensions);

		/// Create a texture1D view with an existing storage
		explicit texture1D(
			storage const & Storage);

		/// Create a texture1D view with an existing storage
		explicit texture1D(
			storage const & Storage,
			format_type const & Format,
			size_type BaseLayer, size_type MaxLayer,
			size_type BaseFace, size_type MaxFace,
			size_type BaseLevel, size_type MaxLevel);

		/// Create a texture1D view, reference a subset of an existing texture1D instance
		explicit texture1D(
			texture1D const & Texture,
			size_type const & BaseLevel, size_type const & MaxLevel);

		/// Create a texture1D view, reference a subset of an existing texture1DArray instance
		explicit texture1D(
			texture1DArray const & Texture,
			size_type const & BaseLayer,
			size_type const & BaseLevel, size_type const & MaxLevel);

		operator storage() const;
		image operator[] (size_type const & Level) const;

		dim_type dimensions() const;
	};
}//namespace gli
