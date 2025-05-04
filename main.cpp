#include "mainwindow.h"
#include "connexion.h"
#include "loginwindow.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection c;
    bool test = c.createconnect();

    MainWindow w;

    if (!test) {
        QMessageBox::critical(nullptr, QObject::tr("Database Error"),
                              QObject::tr("Connection failed.\nClick Cancel to exit."), QMessageBox::Cancel);
        return -1;
    }


    QMessageBox::information(nullptr, QObject::tr("Connection Successful"),
                             QObject::tr("Connection successful.\nClick OK to continue."),
                             QMessageBox::Ok);


    LoginWindow login;
    login.show();
    return a.exec();
}
