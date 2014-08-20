#ifndef DBTEST_HPP
#define DBTEST_HPP


#include "db/mongo.hpp"


class dbtest
{
public:
    dbtest();
private:
    MongoDB m_db;
};

#endif // DBTEST_HPP
