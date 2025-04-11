#ifndef ARBITRE_H
#define ARBITRE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QTableWidget> // Include QTableWidget

// Forward declare MainWindow to avoid circular dependency
class MainWindow;

class Arbitre {
public:
    // Default Constructor
    Arbitre();
    Arbitre(MainWindow *mainWindow);  // Constructor that accepts MainWindow pointer

    // Constructor for adding a referee (ID is auto-incremented, so it's not included)
    Arbitre(QString nom, QString prenom, int age, int experience, QString sexe, QString email);

    // Constructor with ID (for modifying or deleting an existing referee)
    Arbitre(int id, QString nom, QString prenom, int age, int experience, QString sexe, QString email);

    // CRUD Operations
    bool ajouter();
    bool supprimer(int id);
    bool modifier(int id, QString nom, QString prenom, int age, int experience, QString sexe, QString email);
    void afficher(QTableWidget *tableWidget);
    bool validerDonnees(QString& erreur);
    void on_pushButton_update_clicked();
    void exporterEnPDF(QTableWidget *tableWidget);

private:
    MainWindow *mainWindow;  // Pointer to the MainWindow
    int currentArbitreId;    // Store the current referee's ID

    int id_arbitre;          // Auto-incremented by Oracle DB
    QString nom, prenom, sexe, email;
    int age, experience;
};

#endif // ARBITRE_H
