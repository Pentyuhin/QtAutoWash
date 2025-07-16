#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QLabel>
#include <QSettings>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QPixmap>
#include <QDateTime>
#include <QVBoxLayout>


class WeatherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WeatherWidget(QWidget* parent = nullptr);
    ~WeatherWidget();

    // Установка местоположения по координатам
    void setLocation(double latitude, double longitude);
    // Метод для установки названия локации
    void setLocationName(const QString& name);

private slots:
    void fetchWeatherData();
    void onWeatherDataReceived(QNetworkReply* reply);

private:
    void setupUI();
    void updateWeatherDisplay(double temp, const QString& condition, int humidity, double windSpeed);
    void saveWeatherData(double temp, const QString& condition, int humidity, double windSpeed);
    void loadSavedData();
    void showWeatherError(const QString& message);

    // Методы для преобразования данных API
    QString mapWeatherCodeToCondition(int code) const;
    QString mapConditionToIcon(const QString& condition) const;

    // UI элементы
    QNetworkAccessManager* m_networkManager;
    QTimer* m_updateTimer;
    QLabel* m_locationLabel = nullptr;
    QLabel* m_temperatureLabel;
    QLabel* m_weatherIcon;
    QLabel* m_humidityLabel;
    QLabel* m_windLabel;
    QVBoxLayout* m_mainLayout;

    // Координаты местоположения
    double m_latitude;
    double m_longitude;

    QString m_locationName;

    // Константы
    const QString m_apiUrl = "https://api.open-meteo.com/v1/forecast";
};

#endif // WEATHERWIDGET_H