#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow (QWidget *parent)
    : QMainWindow (parent)
    , ui (new Ui::MainWindow)
{
    ui->setupUi (this);
    createLogFile();
    loadCities();
}

MainWindow::~MainWindow()
{
    delete ui;
    logFile->close();
    delete logFile;
}

void MainWindow::createLogFile()
{
    logFile = new QFile ("log.txt");
    logFile->open (QIODevice::WriteOnly);
}

void MainWindow::loadCities()
{
    ui->comboBox->clear();
    idCities.clear();
    QFile fileCities (":/cities/cities.json");
    fileCities.open (QIODevice::ReadOnly);
    if (!fileCities.isOpen()) {
        writeLog ("File \"cities.json\" not found.");
        return;
    }
    QJsonArray mas_cities = QJsonDocument::fromJson (fileCities.readAll()).array();
    for (int i = 0; i < mas_cities.count(); ++i) {
        if (!mas_cities[i].isObject()) continue;
        auto objCity = mas_cities[i].toObject();
        if (objCity.contains ("id") && objCity.contains ("name")) {
            idCities.push_back (
                qMakePair (
                    objCity["name"].toString(),
                    objCity["id"].toInt()
                )
            );
        }
    }
    std::sort (idCities.begin(), idCities.end());
    for (auto idCity : idCities) {
        ui->comboBox->addItem (idCity.first);
    }
}

void MainWindow::writeLog (QString logMessage)
{
    if (!logFile->isOpen()) {
        return;
    }
    logFile->write ((
                        QDateTime::currentDateTime().toString ("d MMMM yy hh:mm:ss - ") +
                        logMessage +
                        "\n").toUtf8());
};

void MainWindow::on_pushButton_clicked()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager (this);

    connect (manager, SIGNAL (finished (QNetworkReply *)),
             this, SLOT (replyFinished (QNetworkReply *)));
    QString url (
        "http://api.openweathermap.org/data/2.5/"
        "weather?id=%1&appid=0ad3df4030a233c298c8cf93b05395a7&lang=ru");
    manager->get (QNetworkRequest (
                      QUrl (
                          url.arg (idCities[ui->comboBox->currentIndex()].second)
                      )
                  ));
    writeLog (QString (
                  "Request weather in \"%1\"").arg (
                  idCities[ui->comboBox->currentIndex()].first
              ));
}

void MainWindow::replyFinished (QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        writeLog ("Error: " + reply->errorString());
    } else {
        parseReply (reply->readAll());
    }
    reply->deleteLater();
}

void MainWindow::parseReply (QByteArray data)
{
    auto reply = QJsonDocument::fromJson (data);
    if (!reply.isObject()) return;
    showReply (reply.toJson (QJsonDocument::Indented));
}

void MainWindow::showReply (QString str)
{
    ui->textEdit->clear();
    ui->textEdit->setPlainText (str);
}
