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

void Connection::closeconnect(){db.close();}


