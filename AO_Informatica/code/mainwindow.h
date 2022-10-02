#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFile>
#include <QFileDialog>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDateTime>

#include <QVector>
#include <QPair>

#include <QNetworkReply>

#include <algorithm>
//#include <Q>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow (QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_pushButton_clicked();
        void replyFinished (QNetworkReply *);

    private:
        Ui::MainWindow *ui;
        QFile *logFile;
        QList <QPair<QString, int>> idCities;
        void createLogFile();
        void loadCities();
        void writeLog (QString);
        void parseReply (QByteArray);
        void showReply (QString);


};
#endif // MAINWINDOW_H
