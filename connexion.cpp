#include "connexion.h"

Connection::Connection(){
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("system");
    db.setPassword("ala123");
}

bool Connection::createconnect(){
    return db.open();
}

/*
QSqlDatabase Connection::getDatabase(){
    return db;
}*/

QSqlDatabase Connection::getDatabase() {
    if (!db.isOpen() && !db.open()) {
        qDebug() << "Failed to open database!";
        return QSqlDatabase();
    }
    return db;
}
