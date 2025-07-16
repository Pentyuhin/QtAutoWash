#include "WeatherWidget.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <QUrlQuery>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBuffer>
#include <QToolTip>
#include <QNetworkReply>

//Состояния погоды в Яндексе могут быть :
//"clear" (ясно)
//"partly-cloudy" (малооблачно)
//"cloudy" (облачно)
//"overcast" (пасмурно)
//"light-rain" (небольшой дождь)
//"rain" (дождь)
//"heavy-rain" (сильный дождь)
//"showers" (ливень)
//"wet-snow" (дождь со снегом)
//"light-snow" (небольшой снег)
//"snow" (снег)
//"snow-showers" (снегопад)
//"hail" (град)
//"thunderstorm" (гроза)

WeatherWidget::WeatherWidget(QWidget* parent)
    : QWidget(parent),
    m_networkManager(new QNetworkAccessManager(this)),
    m_updateTimer(new QTimer(this)),
    m_temperatureLabel(new QLabel(this)),
    m_weatherIcon(new QLabel(this)),
    m_humidityLabel(new QLabel(this)),
    m_windLabel(new QLabel(this)),
    m_mainLayout(new QVBoxLayout(this))
{
    setupUI();
    loadSavedData();

    connect(m_updateTimer, &QTimer::timeout, this, &WeatherWidget::fetchWeatherData);
    connect(m_networkManager, &QNetworkAccessManager::finished,
        this, &WeatherWidget::onWeatherDataReceived);

    // Первое обновление сразу и затем каждые 10 минут
    fetchWeatherData();
    m_updateTimer->start(600000);
}

WeatherWidget::~WeatherWidget()
{
    m_updateTimer->stop();
}

void WeatherWidget::setupUI()
{
    // Настройка стилей
    QFont tempFont = m_temperatureLabel->font();
    tempFont.setPointSize(16);
    tempFont.setBold(true);
    m_temperatureLabel->setFont(tempFont);

    // Основной layout
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(5);

    QHBoxLayout* weatherLayout = new QHBoxLayout();
    weatherLayout->setContentsMargins(100, 0, 10, 0);
    weatherLayout->setSpacing(15);

    // Добавляем виджеты
    weatherLayout->addWidget(m_weatherIcon, 0, Qt::AlignBottom);
    weatherLayout->addWidget(m_temperatureLabel, 0, Qt::AlignBottom);
    weatherLayout->addWidget(m_humidityLabel, 0, Qt::AlignBottom);
    weatherLayout->addWidget(m_windLabel, 0, Qt::AlignBottom);

    m_mainLayout->addLayout(weatherLayout);
    setLayout(m_mainLayout);
}

void WeatherWidget::fetchWeatherData()
{
    QUrl url("https://api.weather.yandex.ru/v2/forecast");
    QUrlQuery query;
    query.addQueryItem("lat", "55.0794"); // Широта Москвы
    query.addQueryItem("lon", "38.7783"); // Долгота Москвы
    query.addQueryItem("lang", "ru_RU");
    query.addQueryItem("limit", "1");
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-Yandex-API-Key", "cce2d236-c69a-4da3-81ec-c321eb84e2e4"); // Вставьте ваш ключ

    m_networkManager->get(request);
}

void WeatherWidget::onWeatherDataReceived(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        showWeatherError(tr("Ошибка сети: ") + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        showWeatherError(tr("Ошибка данных погоды"));
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject fact = root["fact"].toObject();

    double temp = fact["temp"].toDouble();
    QString condition = fact["condition"].toString();
    int humidity = fact["humidity"].toInt();
    double windSpeed = fact["wind_speed"].toDouble();

    updateWeatherDisplay(temp, condition, humidity, windSpeed);
    saveWeatherData(temp, condition, humidity, windSpeed);
}

void WeatherWidget::updateWeatherDisplay(double temp, const QString& condition,
    int humidity, double windSpeed)
{
    // Температура
    QString tempText = QString("%1°C").arg(temp, 0, 'f', 1);
    m_temperatureLabel->setText(tempText);

    // Иконка погоды
    QString iconName = mapConditionToIcon(condition);
    QPixmap icon(QString(":/weather/%1.png").arg(iconName));
    if (icon.isNull()) {
        icon = QPixmap(":/weather/unknown.png");
    }
    m_weatherIcon->setPixmap(icon.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Дополнительная информация
    m_humidityLabel->setText(QString("%1%").arg(humidity));
    m_windLabel->setText(QString("%1 м/с").arg(windSpeed, 0, 'f', 1));

    // Цвет температуры
    QString style = temp < 0 ? "color: #3498db;" :
        temp > 25 ? "color: #e74c3c;" : "color: #2ecc71;";
    m_temperatureLabel->setStyleSheet(style);
}

QString WeatherWidget::mapConditionToIcon(const QString& condition) const
{
    static const QMap<QString, QString> iconMap = {
        {"clear", "sunny"},
        {"partly-cloudy", "partly-cloudy"},
        {"cloudy", "cloudy"},
        {"overcast", "overcast"},
        {"light-rain", "light-rain"},
        {"rain", "rain"},
        {"heavy-rain", "heavy-rain"},
        {"showers", "showers"},
        {"wet-snow", "snow-rain"},
        {"light-snow", "light-snow"},
        {"snow", "snow"},
        {"snow-showers", "snow-showers"},
        {"hail", "hail"},
        {"thunderstorm", "thunderstorm"}
    };
    return iconMap.value(condition, "unknown");
}

void WeatherWidget::saveWeatherData(double temp, const QString& condition,
    int humidity, double windSpeed)
{
    QPixmap icon(QString(":/weather/%1.png").arg(mapConditionToIcon(condition)));
    QByteArray iconData;
    QBuffer buffer(&iconData);
    buffer.open(QIODevice::WriteOnly);
    icon.save(&buffer, "PNG");

    QSettings settings;
    settings.setValue("weather/lastUpdate", QDateTime::currentDateTime());
    settings.setValue("weather/lastTemp", QString("%1°C").arg(temp, 0, 'f', 1));
    settings.setValue("weather/lastHumidity", humidity);
    settings.setValue("weather/lastWind", windSpeed);
    settings.setValue("weather/lastIcon", iconData);
}

void WeatherWidget::loadSavedData()
{
    QSettings settings;
    if (settings.contains("weather/lastUpdate")) {
        QDateTime lastUpdate = settings.value("weather/lastUpdate").toDateTime();
        if (lastUpdate.secsTo(QDateTime::currentDateTime()) < 3600) { // 1 час
            double temp = settings.value("weather/lastTemp").toString().replace("°C", "").toDouble();
            int humidity = settings.value("weather/lastHumidity").toInt();
            double windSpeed = settings.value("weather/lastWind").toDouble();

            m_temperatureLabel->setText(settings.value("weather/lastTemp").toString());
            m_humidityLabel->setText(QString("%1%").arg(humidity));
            m_windLabel->setText(QString("%1 м/с").arg(windSpeed, 0, 'f', 1));

            QPixmap icon;
            icon.loadFromData(settings.value("weather/lastIcon").toByteArray());
            if (!icon.isNull()) {
                m_weatherIcon->setPixmap(icon.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
            return;
        }
    }
    showWeatherError(tr("Ожидание данных..."));
}

void WeatherWidget::showWeatherError(const QString& message)
{
    m_temperatureLabel->setText("--");
    m_humidityLabel->setText("--");
    m_windLabel->setText("--");

    QPixmap icon(":/weather/error.png");
    if (!icon.isNull()) {
        m_weatherIcon->setPixmap(icon.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    QToolTip::showText(mapToGlobal(QPoint(0, height())), message, this, QRect(), 3000);
}