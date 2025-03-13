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
    // Check if the connection exists, if not, create a new one
    if (!db.isOpen() && !db.open()) {
        // Handle the database connection failure
        qDebug() << "Failed to open database!";
        return QSqlDatabase();
    }
    return db;
}
