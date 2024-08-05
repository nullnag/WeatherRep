#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QListWidgetItem>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class WeatherRep;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void getWeather(const QString &city);
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_lineEdit_editingFinished();

    void on_pushButton_clicked();

private:
    QTimer *updateTimer;
    void onResult(QNetworkReply *reply);
    void weatherUpdated(double temperature, QString weather, QString description);
    QNetworkAccessManager* manager;
    Ui::WeatherRep *ui;
};
#endif // MAINWINDOW_H
