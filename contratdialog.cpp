#include "contratdialog.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPrinter>
#include <QTextDocument>
#include <QFileDialog>
#include <QDir>
#include <QDate>
#include <QNetworkReply>
#include <QSqlError>
#include <QDebug>

ContratDialog::ContratDialog(QComboBox* equipeCombo,
                             QTextEdit* outputText,
                             QPushButton* generateBtn,
                             QSqlDatabase db,
                             QWidget *parent)
    : QDialog(parent),
    ui_equipeCombo(equipeCombo),
    ui_contratTextEdit(outputText),
    ui_genererContratButton(generateBtn),
    m_database(db),
    m_networkManager(new QNetworkAccessManager(this))
{
    setWindowTitle("Générateur de Contrats de Joueurs");
    resize(800, 600);
    ui_genererContratButton->setEnabled(false);
    chargerEquipes();
    connect(ui_genererContratButton, &QPushButton::clicked,
            this, &ContratDialog::genererContrat);
}

ContratDialog::~ContratDialog() {
    delete m_networkManager;
}

void ContratDialog::chargerEquipes()
{
    ui_equipeCombo->clear();

    if (!m_database.isOpen()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base de données échouée");
        return;
    }

    QSqlQuery query("SELECT NOMEQUIPE FROM equipe ORDER BY NOMEQUIPE", m_database);
    if (query.lastError().isValid()) {
        QMessageBox::critical(this, "Erreur SQL", "Erreur lors de la requête:\n" + query.lastError().text());
        return;
    }

    while (query.next()) {
        QString nom = query.value(0).toString();
        if (!nom.isEmpty()) {
            ui_equipeCombo->addItem(nom);
        }
    }

    if (ui_equipeCombo->count() == 0) {
        QMessageBox::warning(this, "Avertissement", "Aucune équipe trouvée dans la base");
    } else {
        ui_genererContratButton->setEnabled(true);
    }
}

void ContratDialog::genererContrat() {
    QString nomEquipe = ui_equipeCombo->currentText().trimmed();
    if (nomEquipe.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucune équipe sélectionnée.");
        return;
    }

    // Désactiver le bouton pendant la génération
    ui_genererContratButton->setEnabled(false);
    ui_genererContratButton->setText("Génération en cours...");

    QSqlQuery query(m_database);
    query.prepare("SELECT DATEDEBUTCONTRAT, DATEFINCONTRAT FROM equipe WHERE NOMEQUIPE = ?");
    query.addBindValue(nomEquipe);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur de base de données: " + query.lastError().text());
        ui_genererContratButton->setEnabled(true);
        ui_genererContratButton->setText("Générer Contrat");
        return;
    }

    if (!query.next()) {
        QMessageBox::critical(this, "Erreur", "Aucun contrat trouvé pour cette équipe.");
        ui_genererContratButton->setEnabled(true);
        ui_genererContratButton->setText("Générer Contrat");
        return;
    }

    QDate dateDebut = query.value(0).toDate();
    QDate dateFin = query.value(1).toDate();

    QString prompt = genererPromptContrat(nomEquipe, dateDebut, dateFin);

    // Configuration de la requête API Hugging Face
    QNetworkRequest request(QUrl("https://api-inference.huggingface.co/models/gpt2")); // Vous pouvez changer le modèle
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "https://huggingface.co/apipack/API-Pack-Model"); // Remplacez par votre clé API Hugging Face

    QJsonObject data;
    data["inputs"] = prompt;
    data["parameters"] = QJsonObject{
        {"max_length", 1000}, // Augmentez selon vos besoins
        {"temperature", 0.7},
        {"do_sample", true}
    };

    QByteArray postData = QJsonDocument(data).toJson();

    // Envoi de la requête
    QNetworkReply *reply = m_networkManager->post(request, postData);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleIAResponse(reply); });
}

void ContratDialog::handleIAResponse(QNetworkReply *reply)
{
    ui_genererContratButton->setEnabled(true);
    ui_genererContratButton->setText("Générer Contrat");

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, "Erreur API", reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response);

    if (jsonResponse.isNull()) {
        QMessageBox::critical(this, "Erreur", "Réponse API invalide");
        reply->deleteLater();
        return;
    }

    // Structure de réponse différente pour Hugging Face
    QString contrat;
    if (jsonResponse.isArray()) {
        QJsonArray jsonArray = jsonResponse.array();
        if (!jsonArray.isEmpty()) {
            QJsonObject firstItem = jsonArray[0].toObject();
            if (firstItem.contains("generated_text")) {
                contrat = firstItem["generated_text"].toString();
            }
        }
    } else if (jsonResponse.isObject()) {
        QJsonObject jsonObject = jsonResponse.object();
        if (jsonObject.contains("generated_text")) {
            contrat = jsonObject["generated_text"].toString();
        }
    }

    if (contrat.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Le contrat généré est vide");
        reply->deleteLater();
        return;
    }

    ui_contratTextEdit->setPlainText(contrat);
    exporterPDF();
    reply->deleteLater();
}

QString ContratDialog::genererPromptContrat(const QString& nomEquipe,
                                            const QDate& dateDebut,
                                            const QDate& dateFin) const
{
    return QString(
               "Génère un contrat de joueur professionnel avec ces spécifications :\n\n"
               "1. Club : %1\n"
               "2. Durée : du %2 au %3\n"
               "3. Format :\n"
               "   - Titre centré 'CONTRAT PROFESSIONNEL'\n"
               "   - Espaces vides pour :\n"
               "     * Nom joueur : __________\n"
               "     * Date naissance : __/__/____\n"
               "     * Nationalité : __________\n"
               "4. Contenu :\n"
               "   - 6 articles minimum\n"
               "   - Adapté spécifiquement au club %1\n"
               "   - Inclure clauses spécifiques au sport\n"
               "5. Style : Professionnel, en français, prêt pour impression\n\n"
               "Exemple de structure :\n"
               "CONTRAT PROFESSIONNEL\n\n"
               "Entre le club %1 (...) et le joueur __________ (...)"
               ).arg(nomEquipe, dateDebut.toString("dd/MM/yyyy"), dateFin.toString("dd/MM/yyyy"));
}

void ContratDialog::exporterPDF()
{
    QString defaultName = "Contrat_" + ui_equipeCombo->currentText().replace(" ", "_") + ".pdf";
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Enregistrer le contrat",
        QDir::homePath() + "/" + defaultName,
        "Fichiers PDF (*.pdf)"
        );

    if (fileName.isEmpty()) return;

    QTextDocument doc;
    doc.setPlainText(ui_contratTextEdit->toPlainText());

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(20, 20, 20, 15), QPageLayout::Millimeter);

    doc.print(&printer);
    QMessageBox::information(this, "Succès", "Contrat exporté en PDF avec espaces à remplir.");
}
