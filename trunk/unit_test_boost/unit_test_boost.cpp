// unit_test_boost.cpp : Defines the entry point for the console application.
//
#define BOOST_TEST_MAIN
#include "stdafx.h"

#include "../GameEngine/Core/delegate.ipp"
#include "../GameEngine/Core/engineassert.hpp"
#include "../GameEngine/Core/filemanager.hpp"
#include <boost/test/included/unit_test.hpp>
#include <iostream>

using namespace std;
using namespace gameEngine;
using namespace gameEngine::common;


class example
{
	public:

		void functionCall()
		{

		}

		void functionCall1( int a )
		{
			int i = a;
			++i;
		}

		void functionCall2( int a, char b )
		{
			int i = b;
		}

		void functionCall3( int a, char b, float c )
		{
			float i = c;
		}

	protected:

};

void functionCall()
{
}

void functionCall1( int a )
{
	int i = a;
	++i;
}

BOOST_AUTO_TEST_CASE( delegateTest )
{
	delegate0< void > testDel;
	delegate0< void > testfunc;

	delegate1< int > testDel1;
	delegate1< int > testfunc1;

	delegate2< int, char > test2;

	example ex;

	testDel = delegate0< void >::bind< example, &example::functionCall >( &ex  );
	testfunc = delegate0< void >::function( &functionCall );

	testDel();
	testfunc();

	testDel1 = delegate1<int>::bind< example, &example::functionCall1 >( &ex );
	testfunc1 = delegate1<int>::function( &functionCall1 );

	testDel1( 1 );
	testfunc1( 1 );

	test2 = delegate2<int,char>::bind< example,&example::functionCall2 >( &ex );

	test2( 1, 'c' );
}

BOOST_AUTO_TEST_CASE( packFile_test )
{
	if( FileManager::instance().openPack( "zlib.pak" ) )
	{
		IFile* file;

		if( FileManager::instance().getFile( "win32/DLL_FAQ.txt", file ) )
		{
			uint32 size = file->size();
			file->close();
		}

		FileManager::instance().closePack();
	}
}