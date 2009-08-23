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

BOOST_AUTO_TEST_CASE( lexical_cast_any )
{
    any anyVar = 20;
    string str = lexical_cast< string >( any_cast< const int >( anyVar ) );

    cout << str.c_str() << endl;

}


BOOST_AUTO_TEST_CASE( misc )
{

    string str = "test";
    string capsolation = "[ " + str + " ]";

    cout << capsolation << endl;
    cout << string( "[ " + str + " ]" ) << endl;

	shared_ptr< int > i( new int );

}

