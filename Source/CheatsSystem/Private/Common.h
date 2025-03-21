#pragma once

THIRD_PARTY_INCLUDES_START
#pragma push_macro("check")
#ifdef check
#undef check
#endif
//#define BOOST_NO_EXCEPTIONS
//#define BOOST_NO_CXX11_NOEXCEPT 1
//#define BOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY 1
#include <boost/signals2/signal.hpp>
#include <boost/signals2/shared_connection_block.hpp>
#pragma pop_macro("check")