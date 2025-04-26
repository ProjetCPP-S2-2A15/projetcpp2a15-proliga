#include "connexion.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projet2A15");
    db.setUserName("malek");
    db.setPassword("sys");
    if (db.open())
        test=true;





    return  test;
}
