#include "WeatherWidget.h"
#include <QNetworkReply>
#include <QHBoxLayout>
#include <QToolTip>
#include <QBuffer>
#include <QDebug>

WeatherWidget::WeatherWidget(QWidget* parent)
    : QWidget(parent),
    m_networkManager(new QNetworkAccessManager(this)),
    m_updateTimer(new QTimer(this)),
    m_temperatureLabel(new QLabel(this)),
    m_weatherIcon(new QLabel(this)),
    m_humidityLabel(new QLabel(this)),
    m_windLabel(new QLabel(this)),
    m_mainLayout(new QVBoxLayout(this)),
    m_locationLabel(new QLabel(this)),
    m_latitude(55.7558),  // Москва по умолчанию
    m_longitude(37.6176),
    m_locationName(tr("Коломна, Россия"))
{
    setupUI();
    loadSavedData();

    connect(m_updateTimer, &QTimer::timeout, this, &WeatherWidget::fetchWeatherData);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &WeatherWidget::onWeatherDataReceived);

    fetchWeatherData();
    m_updateTimer->start(600000); // Обновление каждые 10 минут
}

WeatherWidget::~WeatherWidget()
{
    m_updateTimer->stop();
}

void WeatherWidget::setLocation(double latitude, double longitude)
{
    m_latitude = latitude;
    m_longitude = longitude;
    fetchWeatherData(); // Обновляем данные при изменении местоположения
}


void WeatherWidget::setLocationName(const QString& name) {
    
    m_locationName = name;
    m_locationLabel->setText(name);

}

void WeatherWidget::setupUI()
{
    // Настройка шрифта для температуры
    QFont tempFont = m_temperatureLabel->font();
    tempFont.setPointSize(14);
    tempFont.setBold(true);
    m_temperatureLabel->setFont(tempFont);

    // Создаем главный контейнер
    QWidget* container = new QWidget(this);
    container->setFixedSize(200, 85);
    container->setStyleSheet("background-color: #1E90FF; border-radius: 10px;");

    // Создаем layout для контейнера
    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(10, 10, 10, 10);
    containerLayout->setSpacing(5);

    // Добавляем label с локацией
    containerLayout->addWidget(m_locationLabel, 0, Qt::AlignVCenter);
    m_locationLabel->setStyleSheet("");

    // Горизонтальный layout для элементов погоды
    QHBoxLayout* weatherLayout = new QHBoxLayout();
    weatherLayout->setSpacing(8);

    // Добавляем элементы погоды
    weatherLayout->addWidget(m_weatherIcon, 0, Qt::AlignVCenter);
    weatherLayout->addWidget(m_temperatureLabel, 0, Qt::AlignVCenter);
    weatherLayout->addWidget(m_humidityLabel, 0, Qt::AlignVCenter);
    weatherLayout->addWidget(m_windLabel, 0, Qt::AlignVCenter);

    // Добавляем weatherLayout в контейнер
    containerLayout->addLayout(weatherLayout);

    // Очищаем главный layout и добавляем контейнер
    QLayoutItem* child;
    while ((child = m_mainLayout->takeAt(0))) {
        delete child;
    }

    m_mainLayout->addWidget(container, 0, Qt::AlignTop | Qt::AlignRight);
    setLayout(m_mainLayout);

    // Настройка стилей для текстовых меток
    QString labelStyle = "color: white;";
    m_temperatureLabel->setStyleSheet(labelStyle);
    m_humidityLabel->setStyleSheet(labelStyle);
    m_windLabel->setStyleSheet(labelStyle);
}

void WeatherWidget::fetchWeatherData()
{
    QString url = QString("%1?latitude=%2&longitude=%3&current_weather=true&hourly=relativehumidity_2m,windspeed_10m")
        .arg(m_apiUrl)
        .arg(m_latitude)
        .arg(m_longitude);

    m_networkManager->get(QNetworkRequest(QUrl(url)));
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
    QJsonObject currentWeather = root["current_weather"].toObject();

    // Получаем текущий час для выбора правильных значений из hourly
    int currentHour = QDateTime::currentDateTime().time().hour();

    double temp = currentWeather["temperature"].toDouble();
    int weatherCode = currentWeather["weathercode"].toInt();
    double windSpeed = currentWeather["windspeed"].toDouble();

    // Получаем влажность из hourly данных
    int humidity = 50; // Значение по умолчанию
    if (root.contains("hourly")) {
        QJsonObject hourly = root["hourly"].toObject();
        if (hourly.contains("relativehumidity_2m")) {
            QJsonArray humidityArray = hourly["relativehumidity_2m"].toArray();
            if (currentHour < humidityArray.size()) {
                humidity = humidityArray.at(currentHour).toInt();
            }
        }
    }

    QString condition = mapWeatherCodeToCondition(weatherCode);
    updateWeatherDisplay(temp, condition, humidity, windSpeed);
    saveWeatherData(temp, condition, humidity, windSpeed);
}

QString WeatherWidget::mapWeatherCodeToCondition(int code) const
{
    switch (code) {
    case 0: return "clear";
    case 1: case 2: case 3: return "partly-cloudy";
    case 45: case 48: return "fog";
    case 51: case 53: case 55: return "light-rain";
    case 56: case 57: return "freezing-drizzle";
    case 61: case 63: case 65: return "rain";
    case 66: case 67: return "freezing-rain";
    case 71: case 73: case 75: return "snow";
    case 77: return "snow-grains";
    case 80: case 81: case 82: return "showers";
    case 85: case 86: return "snow-showers";
    case 95: return "thunderstorm";
    case 96: case 99: return "thunderstorm-hail";
    default: return "unknown";
    }
}

QString WeatherWidget::mapConditionToIcon(const QString& condition) const
{
    static const QMap<QString, QString> iconMap = {
        {"clear", "sunny"},
        {"partly-cloudy", "partly-cloudy"},
        {"fog", "fog"},
        {"light-rain", "light-rain"},
        {"freezing-drizzle", "snow-rain"},
        {"rain", "rain"},
        {"freezing-rain", "snow-rain"},
        {"snow", "snow"},
        {"snow-grains", "snow"},
        {"showers", "showers"},
        {"snow-showers", "snow-showers"},
        {"thunderstorm", "thunderstorm"},
        {"thunderstorm-hail", "hail"},
        {"unknown", "unknown"}
    };
    return iconMap.value(condition, "unknown");
}

void WeatherWidget::updateWeatherDisplay(double temp, const QString& condition, int humidity, double windSpeed)
{

    // Обновляем локацию (если она была изменена)
    m_locationLabel->setText(m_locationName);

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
    QString style = temp < 0 ? "color: #3498db;" : temp > 25 ? "color: #FF4500" : "color: #E0FFFF";
    m_temperatureLabel->setStyleSheet(style);
}

void WeatherWidget::saveWeatherData(double temp, const QString& condition, int humidity, double windSpeed)
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
        if (lastUpdate.secsTo(QDateTime::currentDateTime()) < 3600) {
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