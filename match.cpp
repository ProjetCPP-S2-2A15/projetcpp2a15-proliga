#include "match.h"
#include"mainwindow.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QHBoxLayout> // Pour mieux aligner les images et le texte
; // Initialize static member
#include <QPixmap>
#include <QLabel>
#include <QTimer>

Match::Match(const QDate& date, const QString& type, const QString& equipe1, const QString& equipe2, const QString& score)
    : m_dateMatch(date), m_typeMatch(type), m_equipe1(equipe1), m_equipe2(equipe2), m_scoreMatch(score)
{
    // No need to manually set m_idMatch
}
// Getters
int Match::idMatch() const {
    return m_idMatch;
}

QDate Match::dateMatch() const {
    return m_dateMatch;
}

QString Match::typeMatch() const {
    return m_typeMatch;
}

QString Match::scoreMatch() const {
    return m_scoreMatch;
}

QString Match::equipe1() const {
    return m_equipe1;  // Return the equipe1 member variable
}

QString Match::equipe2() const {
    return m_equipe2;  // Return the equipe2 member variable
}

// Setters
void Match::setDateMatch(const QDate& date) {
    m_dateMatch = date;
}

void Match::setTypeMatch(const QString& type) {
    m_typeMatch = type;
}

void Match::setScoreMatch(const QString& score) {
    m_scoreMatch = score;
}

void Match::setEquipe1(const QString& equipe1) {
    m_equipe1 = equipe1;
}

void Match::setEquipe2(const QString& equipe2) {
    m_equipe2 = equipe2;
}



ChatBotWidget::ChatBotWidget(QWidget *parent)
    : QWidget(parent)
{
    // Display the start image before showing the chatbot
    QLabel* startImage = new QLabel(this);
    QPixmap pixmap(":/images/start.png");

    // Scale the image to a smaller size (you can adjust the width and height)
    QPixmap scaledPixmap = pixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    startImage->setPixmap(scaledPixmap);
    startImage->setAlignment(Qt::AlignCenter);

    // Create a timer to remove the image after a few seconds (e.g., 3 seconds)
    QTimer::singleShot(5000, [startImage, this] {
        startImage->hide();  // Hide the start image
        // Now initialize the rest of the chat interface
        initChatInterface();  // Call a method to initialize the chat interface
    });

    // Make sure to resize the widget here (this could be set dynamically as needed)
    resize(400, 400);  // Example size, adjust to your needs
}

void ChatBotWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);  // Ensure the base class resize event is called

    // Now reposition the start image in the center
    QLabel* startImage = findChild<QLabel*>();  // Find the image label
    if (startImage) {
        QPixmap pixmap(":/interface_icons/start.png");
        QPixmap scaledPixmap = pixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        startImage->setPixmap(scaledPixmap);

        int x = (width() - scaledPixmap.width()) / 2;
        int y = (height() - scaledPixmap.height()) / 2;
        startImage->setGeometry(x, y, scaledPixmap.width(), scaledPixmap.height());
    }
}



void ChatBotWidget::initChatInterface()
{
    // Création des widgets
    lineEdit_5 = new QLineEdit(this);
    lineEdit_5->setPlaceholderText("Entrez votre question...");
    lineEdit_5->setStyleSheet("padding: 10px; border-radius: 10px; border: 1px solid #ccc;");

    chatBox = new QTextEdit(this);
    chatBox->setReadOnly(true);  // Empêcher l'utilisateur de modifier le chat
    chatBox->setStyleSheet("background-color: #f9f9f9; border: 1px solid #ccc; padding: 10px; border-radius: 10px;");

    // Agencement vertical des éléments avec un espacement
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chatBox);
    layout->addWidget(lineEdit_5);

    // Espacement
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);

    // Connecter l'événement "returnPressed" pour envoyer le message quand la touche Entrée est appuyée
    connect(lineEdit_5, &QLineEdit::returnPressed, this, &ChatBotWidget::onSendButtonClicked);
}
void ChatBotWidget::onSendButtonClicked()
{
    QString userMessage = lineEdit_5->text();
    if (!userMessage.isEmpty()) {
        // Afficher le message de l'utilisateur dans le chatBox avec l'image "log.png"
        QString htmlUserMessage = "<div style='text-align: left;'><b><span style='display:inline-block; vertical-align: middle;'><img src=':/interface_icons/logb.png' width='15' height='20' /> : </span></b> " + userMessage + "</div>";
        chatBox->append(htmlUserMessage);

        // Gérer la réponse du chatbot
        QString botResponse = getChatBotResponse(userMessage);

        // Créer la réponse du chatbot avec l'image et le texte alignés sur la même ligne
        QString htmlResponse = "<div style='text-align: right;'><b><span style='display:inline-block; vertical-align: middle;'><img src=':/interface_icons/chatbot.png' width='20' height='20' /> : </span></b> <span>" + botResponse + "</span></div>";

        // Utiliser append() pour ajouter cette réponse formatée dans le chatBox
        chatBox->append(htmlResponse);

        // Effacer le champ de saisie
        lineEdit_5->clear();
    }
}

QString ChatBotWidget::getChatBotResponse(const QString &userMessage)
{
    // Exemple de réponse simple du chatbot
    if (userMessage.contains("bonjour", Qt::CaseInsensitive)) {
        return "Bonjour! Comment puis-je vous aider?";
    } else if (userMessage.contains("ca va ?", Qt::CaseInsensitive)) {
        return "Je vais bien, merci! Et vous?";
    } else if (userMessage.contains("salut", Qt::CaseInsensitive)) {
        return "Salut ! Commment puis-je vous aider?";
    } else if (userMessage.contains("dernier score de fcb", Qt::CaseInsensitive)) {
        return "Le dernier match du FC Barcelone a eu lieu le 2 mars 2025, où ils ont battu la Real Sociedad 4-0 à l'Estadi Olímpic Lluís Companys";
    } else {
        return "Désolé, je ne pas vous repondre pour l'instant , autre question ?";
    }
}

