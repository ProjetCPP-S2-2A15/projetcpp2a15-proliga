#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "contratdialog.h"
#include <QThread>
#include <QFuture>
#include <QtConcurrent>
#include <QTextEdit>
#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include "connexion.h"
#include"equipe.h"
#include"statistique.h"
#include <QFutureWatcher>
#include <QAudioInput>    // Changed from QAudioSource
#include <QAudioDevice>
#include <QMediaDevices>
#include <QFile>
#include<QCheckBox>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:


    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow* getUi() const { return ui; }

private slots:
    void Ajouter_clicked();

    void refreshTable();
    void openModificationDialog(int idEquipe);
    void modifyEquipe(int equipeId);
    void addButtonsToTable();
    void addActionColumn();
    void exportTableToPDF();
    void trieEquipe() ;
    void rechercheEquipe() ;
    void afficherStatistiques() ;
//void  initArticles();
    void GenererContratJoueur();
    void on_voc_nom_clicked();
    void on_voc_coach_clicked();
    void on_voc_nb_clicked();
    void on_voc_pays_clicked();
    QString recordText();
  //  void generateContractPDF(const QString &teamName, const QStringList &articles);
    bool isTeamNameUnique(const QString& teamName, int excludedId = -1);
    void loadTeamsIntoComboBox();
 //   void updateSelectedArticles();
private:
    Ui::MainWindow *ui;
    Statistique *statistique; // Declare the Statistique pointer
    QChartView *chartView;
    QTimer *searchTimer;
    Equipe *Eq;
    bool isModifying;
    QFutureWatcher<QString> *futureWatcher;
    QString lastClickedField;
    QString m_lastAudioFile;
    QList<QCheckBox*> articleCheckboxes;
    QMap<QString, QStringList> equipeArticles;


};
#endif // MAINWINDOW_H
