#ifndef ARDUINOSHIRAZ_H
#define ARDUINOSHIRAZ_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

class Arduinoshiraz : public QDialog
{
    Q_OBJECT

public:
    explicit Arduinoshiraz(QWidget *parent = nullptr, const QString &team1Name = "Team 1", const QString &team2Name = "Team 2");
    ~Arduinoshiraz();
    QString getFinalScore();

private slots:
    void readSerialData();
    void stopTracking();

private:
    QSerialPort *serial; // Serial port to communicate with Arduino
    QLabel *scoreLabel;
    QPushButton *stopButton;
    int scoreTeam1;
    int scoreTeam2;
    QString team1Name;
    QString team2Name;
};

#endif // ARDUINOSHIRAZ_H
