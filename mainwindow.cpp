#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onResult);
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::on_lineEdit_editingFinished);
    updateTimer->start(10000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getWeather(const QString &city){
    QString apiKey = "46bd3b92e0dacd1e2839888b6194db6e";
    QString url = QString("http://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric")
                      .arg(city).arg(apiKey);
    manager->get(QNetworkRequest(QUrl(url)));
}

void MainWindow::onResult(QNetworkReply *reply){
    ui->hourlyTemps->clear();
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();
        QJsonArray hourlyArray = obj["hourly"].toArray();
        double temperature = obj["main"].toObject()["temp"].toDouble();
        QString weather = obj["weather"].toArray()[0].toObject()["main"].toString();
        QString description = obj["weather"].toArray()[0].toObject()["description"].toString();
        weatherUpdated(temperature, weather, description);
    }
    reply->deleteLater();
}

void MainWindow::weatherUpdated(double temperature, QString weather, QString description){
    QString hourlyTemp;
    ui->Temperature->setText(QString::number(temperature) + " °C");
    ui->Weather->setText(weather);
}

void MainWindow::on_lineEdit_editingFinished()
{
    getWeather(ui->lineEdit->text());
}

