#include "stdafx.h"
/*
* Copyright (c) 2007-2012 SlimDX Group
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
#include <d3d9.h>
#include <d3dx9.h>

#include "VertexStats.h"

using namespace System;

namespace SlimDX
{
namespace Direct3D9
{
	bool VertexStats::operator == ( VertexStats left, VertexStats right )
	{
		return VertexStats::Equals( left, right );
	}

	bool VertexStats::operator != ( VertexStats left, VertexStats right )
	{
		return !VertexStats::Equals( left, right );
	}

	int VertexStats::GetHashCode()
	{
		return RenderedTriangleCount.GetHashCode() + ExtraClippingTriangleCount.GetHashCode();
	}

	bool VertexStats::Equals( Object^ value )
	{
		if( value == nullptr )
			return false;

		if( value->GetType() != GetType() )
			return false;

		return Equals( safe_cast<VertexStats>( value ) );
	}

	bool VertexStats::Equals( VertexStats value )
	{
		return ( RenderedTriangleCount == value.RenderedTriangleCount && ExtraClippingTriangleCount == value.ExtraClippingTriangleCount );
	}

	bool VertexStats::Equals( VertexStats% value1, VertexStats% value2 )
	{
		return ( value1.RenderedTriangleCount == value2.RenderedTriangleCount && value1.ExtraClippingTriangleCount == value2.ExtraClippingTriangleCount );
	}
}
}