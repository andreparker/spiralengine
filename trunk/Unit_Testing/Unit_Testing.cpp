// Unit_Testing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define BOOST_TEST_MAIN

#include <boost/test/included/unit_test.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <iostream>
#include <algorithm>

using namespace boost;
using namespace std;

template< int bit_, int or_ >
struct bitChain
{
	static const int value = (1 << bit_) | or_;
};

int bits = bitChain< 0, bitChain< 1, bitChain< 2, bitChain< 3, 0 >::value >::value >::value >::value;

BOOST_AUTO_TEST_CASE( lexical_cast_any )
{
    any anyVar = 20;
    string str = lexical_cast< string >( any_cast< const int >( anyVar ) );

    cout << str.c_str() << endl;
	cout << "bit value " << bits << endl;

}


BOOST_AUTO_TEST_CASE( misc )
{

    string str = "test";
    string capsolation = "[ " + str + " ]";

    cout << capsolation << endl;
    cout << string( "[ " + str + " ]" ) << endl;

	shared_ptr< int > i( new int );

}

