#include "stdafx.h"
/*
* Copyright (c) 2007-2009 SlimDX Group
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

#include "IDXGIAdapterMock.h"

#include "Asserts.h"
#include "ScopedThrowOnError.h"

#include "IDXGIOutputMock.h"

using namespace testing;
using namespace System;
using namespace SlimDX;
using namespace SlimDX::DXGI;

TEST( IDXGIAdapterTests, GetDescription )
{
	IDXGIAdapterMock mockAdapter;
	Adapter^ adapter = Adapter::FromPointer( System::IntPtr( &mockAdapter ) );
	
	LUID expectedLuid;
	expectedLuid.HighPart = 0xAABBCCDD;
	expectedLuid.LowPart = 0x00112233;

	DXGI_ADAPTER_DESC expectedDescription;
	StringCchPrintf( expectedDescription.Description, 128, L"Fake Adapter" );
	expectedDescription.VendorId = 1;
	expectedDescription.DeviceId = 2;
	expectedDescription.SubSysId = 3;
	expectedDescription.Revision = 4;
	expectedDescription.DedicatedVideoMemory = 1000;
	expectedDescription.DedicatedSystemMemory = 2000;
	expectedDescription.SharedSystemMemory = 3000;
	expectedDescription.AdapterLuid = expectedLuid;

	EXPECT_CALL( mockAdapter, GetDesc( _ ) )
		.WillOnce( DoAll( SetArgumentPointee<0>( expectedDescription ), Return( S_OK ) ) );
	
	AdapterDescription description = adapter->Description;
	ASSERT_TRUE( Result::Last.IsSuccess );

	ASSERT_TRUE( L"Fake Adapter" == description.Description );
	ASSERT_EQ( 1, description.VendorId );
	ASSERT_EQ( 2, description.DeviceId );
	ASSERT_EQ( 3, description.SubsystemId );
	ASSERT_EQ( 4, description.Revision );
	ASSERT_EQ( 1000, description.DedicatedVideoMemory );
	ASSERT_EQ( 2000, description.DedicatedSystemMemory );
	ASSERT_EQ( 3000, description.SharedSystemMemory );
	ASSERT_EQ( 0xAABBCCDD00112233, description.Luid );
	
	delete adapter;
}

TEST( IDXGIAdapterTests, GetDescriptionThrows )
{
	IDXGIAdapterMock mockAdapter;
	Adapter^ adapter = Adapter::FromPointer( System::IntPtr( &mockAdapter ) );
	
	EXPECT_CALL( mockAdapter, GetDesc( _ ) )
		.WillRepeatedly( Return( E_FAIL ) );
	
	ASSERT_MANAGED_THROW( adapter->Description, DXGIException );
	ASSERT_TRUE( Result::Last.IsFailure );

	delete adapter;
}

TEST( IDXGIAdapterTests, GetDescriptionFails )
{
	IDXGIAdapterMock mockAdapter;
	Adapter^ adapter = Adapter::FromPointer( System::IntPtr( &mockAdapter ) );
	
	EXPECT_CALL( mockAdapter, GetDesc( _ ) )
		.WillRepeatedly( Return( E_FAIL ) );
	
	{
		SCOPED_THROW_ON_ERROR( false );

		ASSERT_NO_THROW( adapter->Description );
		ASSERT_TRUE( Result::Last.IsFailure );
	
		AdapterDescription description = adapter->Description;
		ASSERT_TRUE( AdapterDescription() == description );
	}

	delete adapter;
}

TEST( IDXGIAdapterTests, GetOutputCountCanReturnZero )
{
	IDXGIAdapterMock mockAdapter;
	Adapter^ adapter = Adapter::FromPointer( System::IntPtr( &mockAdapter ) );
	
	EXPECT_CALL( mockAdapter, EnumOutputs( _, _ ) )
		.WillOnce( Return( DXGI_ERROR_NOT_FOUND ) );
	
	int count = adapter->GetOutputCount();
	ASSERT_EQ( 0, count );

	delete adapter;
}


TEST( IDXGIAdapterTests, GetOutputCountCanReturnNonZero )
{
	IDXGIAdapterMock mockAdapter;
	IDXGIOutputMock mockOutput0;
	IDXGIOutputMock mockOutput1;
	IDXGIOutputMock mockOutput2;
	Adapter^ adapter = Adapter::FromPointer( System::IntPtr( &mockAdapter ) );
	
	EXPECT_CALL( mockAdapter, EnumOutputs( 0, _ ) )
		.WillOnce( DoAll( SetArgumentPointee<1>( &mockOutput0 ), Return( S_OK ) ) );
	EXPECT_CALL( mockAdapter, EnumOutputs( 1, _ ) )
		.WillOnce( DoAll( SetArgumentPointee<1>( &mockOutput1 ), Return( S_OK ) ) );
	EXPECT_CALL( mockAdapter, EnumOutputs( 2, _ ) )
		.WillOnce( DoAll( SetArgumentPointee<1>( &mockOutput2 ), Return( S_OK ) ) );
	EXPECT_CALL( mockAdapter, EnumOutputs( 3, _ ) )
		.WillOnce( Return( DXGI_ERROR_NOT_FOUND ) );
	
	int count = adapter->GetOutputCount();
	ASSERT_EQ( 3, count );

	delete adapter;
}

TEST( IDXGIAdapterTests, GetOutput )
{
	IDXGIAdapterMock mockAdapter;
	IDXGIOutputMock mockOutput;
	Adapter^ adapter = Adapter::FromPointer( System::IntPtr( &mockAdapter ) );
	
	EXPECT_CALL( mockAdapter, EnumOutputs( 0, _ ) )
		.WillOnce( DoAll( SetArgumentPointee<1>( &mockOutput ), Return( S_OK ) ) );
	
	IOutput^ output = adapter->GetOutput( 0 );
	ASSERT_TRUE( output != nullptr );
	
	//TODO: To enable this assert, SlimDX must #pragma make_public(N) where N is the native interface.
	//ASSERT_TRUE( output->InternalPointer == &mockOutput );
	delete adapter;
}

TEST( IDXGIAdapterTests, IsInterfaceSupported )
{
	IDXGIAdapterMock mockAdapter;
	IDXGIOutputMock mockOutput;
	Adapter^ adapter = Adapter::FromPointer( System::IntPtr( &mockAdapter ) );
	
	EXPECT_CALL( mockAdapter, CheckInterfaceSupport( _, _ ) )
		.WillOnce( Return( S_OK ) );
	
	bool result = false;
	ASSERT_NO_THROW( result = adapter->IsInterfaceSupported( System::Type::typeid ) );
	ASSERT_TRUE( result );

	delete adapter;
}

TEST( IDXGIAdapterTests, IsInterfaceSupportedCanFail )
{
	IDXGIAdapterMock mockAdapter;
	IDXGIOutputMock mockOutput;
	Adapter^ adapter = Adapter::FromPointer( System::IntPtr( &mockAdapter ) );
	
	EXPECT_CALL( mockAdapter, CheckInterfaceSupport( _, _ ) )
		.WillOnce( Return( E_FAIL ) );
	
	bool result = true;
	ASSERT_NO_THROW( result = adapter->IsInterfaceSupported( System::Type::typeid ) );
	ASSERT_FALSE( result );

	delete adapter;
}

TEST( IDXGIAdapterTests, ToString )
{
	IDXGIAdapterMock mockAdapter;
	Adapter^ adapter = Adapter::FromPointer( System::IntPtr( &mockAdapter ) );
	
	DXGI_ADAPTER_DESC expectedDescription = { };
	StringCchPrintf( expectedDescription.Description, 128, L"Fake Adapter" );

	EXPECT_CALL( mockAdapter, GetDesc( _ ) )
		.WillOnce( DoAll( SetArgumentPointee<0>( expectedDescription ), Return( S_OK ) ) );
	
	ASSERT_TRUE( L"Fake Adapter" == adapter->ToString() );
	ASSERT_TRUE( Result::Last.IsSuccess );

	delete adapter;
}