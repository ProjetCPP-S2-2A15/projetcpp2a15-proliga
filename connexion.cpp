#include "connexion.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projet");
    db.setUserName("C##hedi");
    db.setPassword("ghhmmss");



    if (db.open())
        test=true;





    return  test;
}
