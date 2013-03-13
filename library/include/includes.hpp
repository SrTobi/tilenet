#pragma once
#ifndef _INCLUDES_HPP
#define _INCLUDES_HPP


#ifdef TILENET_USE_PRECOMPILED_HEADER

// ********** std ********** //
#include <type_traits>
#include <string>
#include <stack>
#include <list>
#include <vector>
#include <queue>
#include <iostream>



// ********** boost ********** //
#include <boost/shared_ptr.hpp>
#include <boost/thread/tss.hpp>
#include <boost/variant/variant.hpp>
#include <boost/exception/all.hpp>
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr/detail/spinlock_pool.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>

#ifdef TILENET_TESTS
//#include <boost/test/test_tools.hpp>
#endif

#endif // TILENET_USE_PRECOMPILED_HEADER





#endif
