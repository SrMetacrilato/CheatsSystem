#pragma once

THIRD_PARTY_INCLUDES_START
#pragma push_macro("check")
#ifdef check
#undef check
#endif
//#define BOOST_NO_EXCEPTIONS 1
//#define BOOST_NO_CXX11_NOEXCEPT 1
#include <boost/signals2/signal.hpp>
#pragma pop_macro("check")