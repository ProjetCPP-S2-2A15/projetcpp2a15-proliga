#include "loginwindow.h"
#include <QPalette>
#include <QFont>
#include <QMessageBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPixmap>
#include <QSpacerItem>
#include <QWidget>
#include <QTransform>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Login");
    setFixedSize(800, 450);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(0, 0, 0));
    setPalette(palette);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(0, 20, 20, 20);
    QLabel *imageLabel = new QLabel(this);
    QPixmap image(":/interface_icons/koura.png");
    imageLabel->setPixmap(image.scaled(50, 470, Qt::KeepAspectRatioByExpanding));
    imageLabel->setAlignment(Qt::AlignLeft);

    QWidget *greenLine = new QWidget(this);
    greenLine->setStyleSheet("background-color: #32CD32;");
    greenLine->setFixedWidth(2);
    greenLine->setFixedHeight(400);

    QWidget *loginWidget = new QWidget(this);
    QVBoxLayout *loginLayout = new QVBoxLayout(loginWidget);

    QLabel *logoLabel = new QLabel(this);
    QPixmap logo(":/interface_icons/log.png");
    logoLabel->setPixmap(logo.scaled(35, 35, Qt::KeepAspectRatio));
    logoLabel->setAlignment(Qt::AlignRight);
    loginLayout->addWidget(logoLabel, 0, Qt::AlignRight);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(10);

    emailInput = new QLineEdit(this);
    emailInput->setPlaceholderText("Entrez votre email");
    emailInput->setStyleSheet("QLineEdit { background-color: #f1f1f1; color: black; border-radius: 20px; padding: 10px; font-size: 14px; }");

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Entrez votre mot de passe");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet("QLineEdit { background-color: #f1f1f1; color: black; border-radius: 20px; padding: 10px; font-size: 14px; }");

    QLabel *emailLabel = new QLabel("Email:");
    emailLabel->setStyleSheet("QLabel { font-weight: normal; font-size: 14px; color: white; }");
    QLabel *passwordLabel = new QLabel("Password:");
    passwordLabel->setStyleSheet("QLabel { font-weight: normal; font-size: 14px; color: white; }");

    formLayout->addRow(emailLabel, emailInput);
    formLayout->addRow(passwordLabel, passwordInput);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *faceIdButton = new QPushButton("Face ID", this);
    faceIdButton->setStyleSheet("QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1E90FF, stop:1 #32CD32); "
                                "color: white; border: none; border-radius: 5px; padding: 7px; font-size: 16px; }"
                                "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1E90FF, stop:1 #3CB371); }");

    QPushButton *loginButton = new QPushButton("Login", this);
    loginButton->setStyleSheet("QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1E90FF, stop:1 #32CD32); "
                               "color: white; border: none; border-radius: 5px; padding: 7px; font-size: 16px; }"
                               "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1E90FF, stop:1 #3CB371); }");

    QLabel *faceIdImage = new QLabel(this);
    QPixmap faceIdImagePix(":/interface_icons/faceid.png");
    faceIdImage->setPixmap(faceIdImagePix.scaled(50, 50, Qt::KeepAspectRatio));

    QLabel *loginImage = new QLabel(this);
    QPixmap loginImagePix(":/interface_icons/login.png");
    loginImage->setPixmap(loginImagePix.scaled(30, 30, Qt::KeepAspectRatio));
    buttonLayout->addWidget(faceIdButton);
    buttonLayout->addWidget(faceIdImage);
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(loginImage);
    buttonLayout->setSpacing(10);

    loginLayout->addLayout(formLayout);
    loginLayout->addLayout(buttonLayout);

    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(greenLine);
    mainLayout->addWidget(loginWidget);

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::checkLogin);

    connect(faceIdButton, &QPushButton::clicked, this, &LoginWindow::on_faceRecognitionButton_clicked);


    mainLayout->setSpacing(20);
}

void LoginWindow::checkLogin() {
    QString email = emailInput->text().trimmed();
    QString password = passwordInput->text().trimmed();

    if (email == "s" && password == "s") {
        MainWindow *mainWindow = new MainWindow();
        mainWindow->show();
        close();
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid email or password.");
    }
}




#include <QMessageBox>
#include <QProcess>
#include <QCoreApplication>
#include <QString>

void LoginWindow::on_faceRecognitionButton_clicked()
{
    QString appDir = QCoreApplication::applicationDirPath();  // Get current directory
    QString pythonPath = "C:\\Users\\alabe\\AppData\\Local\\Programs\\Python\\Python312\\python.exe";
    QString scriptPath = "C:\\Users\\alabe\\Pictures\\metiers avances\\face\\detect.py";  // Path to script

    QProcess *process = new QProcess(this);
    process->setWorkingDirectory(appDir);  // Set working directory to app directory
    // Set up the environment for the process
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH", "C:\\Users\\alabe\\AppData\\Local\\Programs\\Python\\Python312\\Scripts");  // Add your Python path
    env.insert("PYTHONPATH", "C:\\Users\\alabe\\AppData\\Local\\Programs\\Python\\Python312");  // Add Python home

    process->setProcessEnvironment(env);  // Apply the environment

    // Connect signals to capture output
    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]() {
        QByteArray output = process->readAllStandardOutput();
        qDebug() << "Output: " << output;

        // Check for "Access denied" in the output
        if (output.contains("Access denied")) {
            QMessageBox::critical(this, "Error", "Face recognition failed. Access denied.");
        } else {
            // Success, proceed to the main window
            QMessageBox::information(this, "Success", "Face recognition completed successfully.");
            MainWindow *mainWindow = new MainWindow();
            mainWindow->show();
            this->close();  // Close the current LoginWindow
        }
    });

    connect(process, &QProcess::readyReadStandardError, this, [process]() {
        QByteArray errorOutput = process->readAllStandardError();
        qDebug() << "Error: " << errorOutput;
    });

    process->start(pythonPath, QStringList() << scriptPath);

    if (!process->waitForStarted()) {
        qDebug() << "Failed to start Python script!";
    } else {
        qDebug() << "Face recognition started.";
    }
}

