#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include "mainwindow.h"

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

private slots:
    void checkLogin();

    void on_faceRecognitionButton_clicked();


private:
    QLineEdit *emailInput;
    QLineEdit *passwordInput;
};

#endif
