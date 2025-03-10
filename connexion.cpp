#include "connexion.h"

Connection::Connection(){
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("ProjetCpp2A15");
    db.setUserName("system");
    db.setPassword("ala123");
}

bool Connection::createconnect(){
    return db.open();
}


QSqlDatabase Connection::getDatabase(){
    return db;
}
