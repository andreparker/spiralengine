#ifndef COPY_POLICIES_H
#define COPY_POLICIES_H
// Copyright 2008 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/utility.hpp>


namespace boost { namespace msm
{
    // deactivates copy
    struct NoCopy : boost::noncopyable
    {
	    // tags
	    typedef mpl::bool_<false>		shallow_copy;
    };
	
    // activate the shallow copy flag
    struct ShallowCopy
    {
	    // tags
	    typedef mpl::bool_<true>		shallow_copy;
    };
    // deactivate the shallow copy flag
    struct DeepCopy
    {
	    // tags
	    typedef mpl::bool_<false>		shallow_copy;
    };
} } //boost::msm


#endif //COPY_POLICIES_H
