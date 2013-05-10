#ifndef _INCLUDES_HPP
#define _INCLUDES_HPP


#ifdef TILENET_USE_PRECOMPILED_HEADER

#ifdef _MSC_VER
#include <sdkddkver.h>
#endif

// ********** std ********** //
#include <type_traits>
#include <string>
#include <stack>
#include <list>
#include <vector>
#include <queue>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// ********** boost ********** //
#include <boost/thread/tss.hpp>
#include <boost/variant/variant.hpp>
#include <boost/exception/all.hpp>
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>
#include <boost/asio.hpp>


// ********** sfml *********** //
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


#ifdef TILENET_TESTS
//#include <boost/test/test_tools.hpp>
#endif

#endif // TILENET_USE_PRECOMPILED_HEADER





#endif
