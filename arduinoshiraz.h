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
    explicit  Arduinoshiraz(QWidget *parent = nullptr, const QString &team1 = "", const QString &team2 = "");

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
    void sendScoreToArduino(int score1, int score2);

};

#endif // ARDUINOSHIRAZ_H
