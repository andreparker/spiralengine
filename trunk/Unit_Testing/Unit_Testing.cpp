// Unit_Testing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define BOOST_TEST_MAIN

#include "../GameEngine/Spiral.hpp"
#include <boost/test/included/unit_test.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/thread.hpp>
#include <string>
#include <iostream>
#include <algorithm>
#include <boost/function.hpp>
#include <boost/functional/hash.hpp>

using namespace Spiral;
using namespace boost;
using namespace std;

template< int bit_, int or_ >
struct bitChain
{
	static const int value = (1 << bit_) | or_;
};

template< boost::uint32_t bits, int place = 0 >
struct binary2Dec
{
	enum
	{
		holePart = bits / 10L
	};
	static const boost::uint32_t value = ( static_cast<boost::uint32_t>(  ( float( bits * 0.1f - holePart ) > 0.0f ? 1 : 0 )  ) << place ) | 
		binary2Dec< holePart, place+1 >::value;
};

template< int place >
struct binary2Dec< 0, place >
{
	static const boost::uint32_t value = 0;
};

int bits = bitChain< 0, bitChain< 1, bitChain< 2, bitChain< 3, 0 >::value >::value >::value >::value;

BOOST_AUTO_TEST_CASE( lexical_cast_any )
{
    any anyVar = 20;
    string str = lexical_cast< string >( any_cast< const int >( anyVar ) );

    cout << str.c_str() << endl;
	cout << "bit value " << bits << endl;

}


BOOST_AUTO_TEST_CASE( ThreadInfo )
{
	cout << "Number of hyperthreading/Core's - " << boost::thread::hardware_concurrency() << endl;
}

BOOST_AUTO_TEST_CASE( Binary2DecTest )
{
	cout << "binaray - 1 to Dec " << binary2Dec< 1 >::value << endl;
	cout << "binaray - 111 to Dec " << binary2Dec< 111 >::value << endl;
	cout << "binaray - 101 to Dec " << binary2Dec< 101 >::value << endl;
	cout << "binaray - 110 to Dec " << binary2Dec< 110 >::value << endl;
	cout << "binaray - 1111 to Dec " << binary2Dec< 1111 >::value << endl;
	cout << "binaray - 1110 to Dec " << binary2Dec< 1110 >::value << endl;
	cout << "binaray - 11100 to Dec " << binary2Dec< 11100 >::value << endl;
	cout << "binaray - 11101 to Dec " << binary2Dec< 11101 >::value << endl;
	cout << "binaray - 11111111 to Dec " << binary2Dec< 11111111 >::value << endl;
}

boost::int32_t RoundNextPwr2( boost::int32_t n )
{
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n++;

	return n;
}

BOOST_AUTO_TEST_CASE( PowerOf2_Rounding )
{
	cout << "next power2 24 = " << RoundNextPwr2( 24 ) << endl;
	cout << "next power2 47 = " << RoundNextPwr2( 47 ) << endl;
	cout << "next power2 56 = " << RoundNextPwr2( 56 ) << endl;
	cout << "next power2 100 = " << RoundNextPwr2( 100 ) << endl;
}

BOOST_AUTO_TEST_CASE( BoostHash )
{
	boost::hash< string > test;
	string str = "A Simple String";

	cout << "hash for str: " << str <<" = "<< test( str ) << endl;
}