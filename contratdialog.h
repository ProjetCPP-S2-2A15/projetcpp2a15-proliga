#ifndef CONTRATDIALOG_H
#define CONTRATDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QNetworkAccessManager>
#include <QNetworkReply>  // Ajouté pour QNetworkReply
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>

class ContratDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContratDialog(QComboBox* equipeCombo,
                           QTextEdit* outputText,
                           QPushButton* generateBtn,
                           QSqlDatabase db,
                           QWidget *parent = nullptr);
    ~ContratDialog();

private slots:
    void genererContrat();
    void handleIAResponse(QNetworkReply *reply);
    void exporterPDF();
    // Ajoutez cette déclaration de signal
signals:
    void equipesChargees(bool succes);
private:
    QComboBox* ui_equipeCombo;
    QTextEdit* ui_contratTextEdit;
    QPushButton* ui_genererContratButton;
    QSqlDatabase m_database;
    QNetworkAccessManager* m_networkManager;
    const QString m_openAIKey = "sk-your-api-key"; // Remplacez par votre clé

    void chargerEquipes();
    QString genererPromptContrat(const QString& nomEquipe,
                                 const QDate& dateDebut,
                                 const QDate& dateFin) const;
};

#endif // CONTRATDIALOG_H
