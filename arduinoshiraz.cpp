#include "arduinoshiraz.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

Arduinoshiraz::Arduinoshiraz(QWidget *parent, const QString &team1Name, const QString &team2Name) :
    QDialog(parent),
    serial(new QSerialPort(this)),
    scoreTeam1(0),
    scoreTeam2(0),
    team1Name(team1Name),
    team2Name(team2Name)
{
    // Apply the gradient background to the whole dialog
    this->setStyleSheet(R"(
        Arduinoshiraz {
            background: qlineargradient(
                spread:pad, x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(124, 235, 198, 255),
                stop:0.468504 rgba(152, 235, 182, 255),
                stop:0.854331 rgba(184, 216, 194, 255)
            );
        }
    )");

    // Main vertical layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Top layout with logo aligned to top-right
    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *logoLabel = new QLabel(this);
    QPixmap logoPixmap(":/interface_icons/logb.png");
    logoLabel->setPixmap(logoPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
    logoLabel->setAttribute(Qt::WA_TranslucentBackground);
    logoLabel->setStyleSheet("background: transparent;");

    topLayout->addStretch();
    topLayout->addWidget(logoLabel);
    mainLayout->addLayout(topLayout);

    // Score label centered with styling
    scoreLabel = new QLabel(QString("%1: 0 | %2: 0").arg(team1Name).arg(team2Name), this);
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet("font-size: 24px; font-weight: bold; padding: 10px;");
    mainLayout->addWidget(scoreLabel);

    // Stop button centered and styled
    stopButton = new QPushButton("Arrêter le suivi", this);
    stopButton->setStyleSheet("padding: 10px; font-size: 16px;");
    mainLayout->addWidget(stopButton, 0, Qt::AlignCenter);
    connect(stopButton, &QPushButton::clicked, this, &Arduinoshiraz::stopTracking);

    // Serial communication setup
    serial->setPortName("COM6");  // Update this according to your system
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        sendScoreToArduino(scoreTeam1, scoreTeam2);

        connect(serial, &QSerialPort::readyRead, this, &Arduinoshiraz::readSerialData);
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le port série: " + serial->errorString());
    }
}

Arduinoshiraz::~Arduinoshiraz()
{
    if (serial->isOpen()) {
        serial->close();
    }
}

void Arduinoshiraz::sendScoreToArduino(int score1, int score2)
{
    if (serial->isOpen()) {
        QString message = QString("T1=%1;T2=%2;").arg(score1).arg(score2);
        QByteArray data = message.toUtf8();
        serial->write(data);
        serial->waitForBytesWritten(1000);
    }
}

void Arduinoshiraz::readSerialData()
{
    QByteArray data = serial->readAll();
    QString dataString = QString::fromUtf8(data);

    if (dataString.contains("Team 1:") && dataString.contains("Team 2:")) {
        QStringList scoreParts = dataString.split("|");
        if (scoreParts.size() >= 2) {
            QString team1Score = scoreParts[0].split(":")[1].trimmed();
            QString team2Score = scoreParts[1].split(":")[1].trimmed();

            scoreTeam1 = team1Score.toInt();
            scoreTeam2 = team2Score.toInt();

            scoreLabel->setText(QString(" %1  -  %2").arg(scoreTeam1).arg(scoreTeam2));

            sendScoreToArduino(scoreTeam1, scoreTeam2);
        }
    }
}

void Arduinoshiraz::stopTracking()
{
    if (serial->isOpen()) {
        serial->close();
    }
    accept();
}

QString Arduinoshiraz::getFinalScore()
{
    return QString("%1-%2").arg(scoreTeam1).arg(scoreTeam2);
}
