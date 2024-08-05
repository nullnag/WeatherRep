#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WeatherRep)
{
    ui->setupUi(this);
    QWidget::setWindowTitle("Weather");
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
    if (weather == "Rain" || weather == "Thunderstorm"){
        ui->frame->setStyleSheet("background-image: url(:/backgrounds/images/rainy.png);");
    }
    else if (weather == "Clear"){
        ui->frame->setStyleSheet("background-image: url(:/backgrounds/images/sunny.png);");
    }
    else if (weather == "Snow"){
        ui->frame->setStyleSheet("background-image: url(:/backgrounds/images/snow.png);");
    }
    else if (weather == "Clouds"){
        ui->frame->setStyleSheet("background-image: url(:/backgrounds/images/cloudy.jpg);;");
    }
}

void MainWindow::on_lineEdit_editingFinished()
{
    getWeather(ui->lineEdit->text());
}


void MainWindow::on_pushButton_clicked()
{
    getWeather(ui->lineEdit->text());
}

