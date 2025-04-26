#include "scene1.h"
#include <QDebug>

Scene1::Scene1() {}

bool Scene1::openSerialPort(const QString &portName) {
    serial.setPortName(portName);
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    return serial.open(QIODevice::ReadWrite);
}

void Scene1::sendChampionnatList(const QStringList &championnatNames) {
    QByteArray startMessage = "##START##\n";
    serial.write(startMessage);
    serial.waitForBytesWritten(100);


    for (const QString &name : championnatNames) {
        QByteArray data = name.toUtf8() + '\n';
        serial.write(data);
        serial.waitForBytesWritten(100);
    }


    QByteArray endMessage = "##END##\n";
    serial.write(endMessage);
    serial.waitForBytesWritten(100);
}

void Scene1::closeSerialPort() {
    if (serial.isOpen()) {
        serial.close();
    }
}

void Scene1::getChampDB() {
    QStringList names;
    QSqlQuery query;
    query.prepare("SELECT NOM FROM MALEK.CHAMPIONNATS");


    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();  // Log the error
        return;
    }

    while (query.next()) {
        names << query.value(0).toString();
    }

    if (!names.isEmpty()) {
        if (openSerialPort("COM8")) {
            qDebug() << "Serial port opened.";
            sendChampionnatList(names);
            qDebug() << "Names sent. Closing port.";
        } else {
            qDebug() << "Could not open serial port";
        }
    } else {
        qDebug() << "No championnat names found!";
    }
}

QString Scene1::readSerialData() {
    QString name = "";

    if (serial.bytesAvailable() > 0) {
        QByteArray data = serial.readAll();
        qDebug() << "Raw serial data:" << data;

        QString receivedData = QString::fromUtf8(data);
        qDebug() << "Received UTF-8 data:" << receivedData;

        name = receivedData.trimmed();
        qDebug() << "Trimmed name:" << name;
    } else {
        qDebug() << "No data available on serial port.";
    }

    return name;
}

void Scene1::ARD_sendCS(const QString &message)
{
    if (serial.isOpen() && serial.isWritable()) {
        QByteArray data = message.toUtf8();
        data.append('\n');
        serial.write(data);
        serial.flush();
    } else {
        qDebug() << "Serial port is not open or not writable!";
    }
}
