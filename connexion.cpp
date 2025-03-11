#include "connexion.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Projet2A15");
    db.setUserName("SYSTEM");
    db.setPassword("ahmedcss");

    if (db.open())
        test=true;





    return  test;
}
