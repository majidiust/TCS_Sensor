#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>
#include <cstring>
#include <boost/asio.hpp>
#include <boost/utility.hpp>
#include <boost/signal2/signal.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

using namespace std;

class Base
{
    public:
	Base();
	void	stdLog(string msg);
};

#endif