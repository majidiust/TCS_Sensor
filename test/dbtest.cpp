#include "dbtest.hpp"

dbtest::dbtest()
{
    m_db.connectToDB();
    for(int i = 0 ; i < 10 ; i++){
        m_db.insertNewTraffic();
        sleep(2);
    }
}
