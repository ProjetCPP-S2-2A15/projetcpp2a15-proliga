#ifndef ARBITRE_H
#define ARBITRE_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QTableWidget>
#include <QNetworkAccessManager>

class MainWindow;

class Arbitre : public QObject
{
    Q_OBJECT

public:
    // Constructors
    explicit Arbitre(MainWindow *mainWindow = nullptr, QObject *parent = nullptr);
    explicit Arbitre(QString nom, QString prenom, int age, int experience,
                     QString sexe, QString email, QString telephone,
                     QObject *parent = nullptr);
    explicit Arbitre(int id, QString nom, QString prenom, int age, int experience,
                     QString sexe, QString email, QString telephone,
                     QObject *parent = nullptr);

    // Core functionality
    bool ajouter();
    bool supprimer(int id);
    bool modifier(int id, QString nom, QString prenom, int age, int experience,
                  QString sexe, QString email, QString telephone);

    // UI operations
    void afficher(QTableWidget *tableWidget);
    void exporterEnPDF(QTableWidget *tableWidget);

    // Validation and utilities
    bool validerDonnees(QString& erreur);
    QString getExperienceStats();
    static void drawSectionHeader(QPainter* painter, int x, int y,
                                  int width, const QString& text);

    // Email functionality
    void envoyerConfirmationsArbitres();
    void setSmtpCredentials(const QString &server, int port,
                            const QString &user, const QString &password);

private:
    // Member variables
    MainWindow *mainWindow;
    int currentArbitreId;
    int id_arbitre;
    QString nom;
    QString prenom;
    QString sexe;
    QString email;
    QString telephone;
    int age;
    int experience;

    // Email related
    QNetworkAccessManager *networkManager;
    QString smtpServer;
    int smtpPort;
    QString smtpUser;
    QString smtpPassword;

    // Private methods
    void sendEmail(const QString &email, const QString &subject, const QString &body);
    void generateCV(QPainter& painter, const QString& id, const QString& nom,
                    const QString& prenom, const QString& age,
                    const QString& experience, const QString& sexe,
                    const QString& email, const QString& telephone);
};

#endif // ARBITRE_H
