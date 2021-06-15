#pragma once
#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <conio.h>
#include <set>
#include <iostream>
#include <boost/enable_shared_from_this.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/thread.hpp>
#include <boost/regex.hpp>
#include <map>

static bool debugmode = true;
static boost::mutex debug_mutex;
static std::ostream debug_global(std::clog.rdbuf()); 
