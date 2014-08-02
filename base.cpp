#include "base.hpp"

Base::Base()
{
    cout << "Base class of project" << endl;
}

void Base::stdLog(string msg)
{
    cout << "Log : " << msg << endl;
}