#ifndef SCENE1_H
#define SCENE1_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <connexion.h>
#include <QString>

class Scene1
{
public:
    Scene1();
    bool openSerialPort(const QString &portName);
    void sendChampionnatList(const QStringList &championnatNames);
    void closeSerialPort();
    void getChampDB();
    QString readSerialData();
    void ARD_sendCS(const QString &message);

private:
    QSerialPort serial;
};

#endif // SCENE1_H
