/*
#include "connexion.h"

//the problem must be from the code (i use connection without clearing it maybe) not because i have too many databases !
Connection::Connection() {
    if (!QSqlDatabase::contains("my_connection")) {
        db = QSqlDatabase::addDatabase("QODBC", "my_connection");
    } else {
        db = QSqlDatabase::database("my_connection");
    }

    db.setDatabaseName("ProjetCpp2A15");
    db.setUserName("system");
    db.setPassword("ala123");
}


bool Connection::createconnect(){
    return db.open();
}

QSqlDatabase Connection::getDatabase() {
    if (!db.isOpen() && !db.open()) {
        qDebug() << "Failed to open database!";
        return QSqlDatabase();
    }
    return db;
}

-------2------------
*/




#include "connexion.h"

Connection::Connection(){
    // Check if the connection already exists before adding it
    if (!QSqlDatabase::contains("ProjetCpp2A15")) {
        db = QSqlDatabase::addDatabase("QODBC", "ProjetCpp2A15");
        db.setDatabaseName("ProjetCpp2A15");
        db.setUserName("system");
        db.setPassword("ala123");
    } else {
        db = QSqlDatabase::database("ProjetCpp2A15");
    }
}

bool Connection::createconnect(){
    // Open the connection only if it is not already open
    if (!db.isOpen()) {
        return db.open();
    }
    return true;  // If already open, just return true
}

QSqlDatabase Connection::getDatabase() {
    if (!db.isOpen() && !db.open()) {
        qDebug() << "Failed to open database!";
        return QSqlDatabase();
    }
    return db;
}


