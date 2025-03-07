#include "connexion.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("ProjetCpp2A15");
    db.setUserName("khadija");
    db.setPassword("khadija");

    if (db.open())
        test=true;





    return  test;
}
