#include "personalAccountWidget.h"

#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVBoxLayout>

PersonalAccountWidget::PersonalAccountWidget(QWidget* parent) :
	QWidget(parent),
	m_carImageLabel(new QLabel(this)),
	m_userInfoLabel(new QLabel(this))
{

	setupUI();
}


void PersonalAccountWidget::setupUI() {

	// Главный контейнер
	QWidget* infoUserContainer = new QWidget(this);
	infoUserContainer->setStyleSheet(
		"background-color: #696969;"
		"border-radius: 10px;"
		"margin: 10px;"
	);
	infoUserContainer->setFixedSize(310, 120);  // Фиксированный размер контейнера

	// Layout для содержимого внутри контейнера
	QVBoxLayout* containerLayout = new QVBoxLayout(infoUserContainer);
	containerLayout->setContentsMargins(15, 15, 15, 15);  // Внутренние отступы

	// Настройка текстового поля
	m_userInfoLabel->setWordWrap(true);
	m_userInfoLabel->setAlignment(Qt::AlignLeft);
	m_userInfoLabel->setStyleSheet(
		"color: #C0C0C0;"
		"font-size: 14px;"
	);

	// Добавляем label в контейнер
	containerLayout->addWidget(m_userInfoLabel);

	// Главный layout виджета
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);  // Убираем отступы у главного layout

	m_carImageLabel->setFixedSize(295, 200);
	// Устанавливаем атрибуты для поддержки прозрачности
	m_carImageLabel->setAttribute(Qt::WA_TranslucentBackground);
	m_carImageLabel->setStyleSheet(
		"background: transparent;"
		"border: none;"  // Убираем границу, если она есть

	);

	mainLayout->addWidget(m_carImageLabel, 0, Qt::AlignCenter);

	// Добавляем контейнер с выравниванием по центру
	mainLayout->addWidget(infoUserContainer, 0, Qt::AlignCenter);

	// Устанавливаем выравнивание всего layout по центру
	mainLayout->setAlignment(Qt::AlignCenter);

}


void PersonalAccountWidget::loadUserData(const QString &user_id) {

	QSqlQuery query;
	query.prepare("SELECT name, car_model, car_num, car_body FROM users WHERE id = :user_id");

	query.bindValue(":user_id", user_id);
	

	if (!query.exec()) {
		qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
		return;
	}

	if (query.next()) {
		
		qDebug() << "Пользователь найден";

		QString userInfo = QString(
			"Имя: %1\n"
			"Модель автомобиля: %2\n"
			"Номер автомобиля:	%3\n"
			"Кузов авто:		%4"
		).arg(
			query.value("name").toString(),
			query.value("car_model").toString(),
			query.value("car_num").toString(),
			query.value("car_body").toString()
		);

		updateCarImage(query.value("car_body").toString());
		displayUserInfo(userInfo);
		
	}
	else {
		
		qDebug() << "Пользователь не найден" << user_id;
		QThread::msleep(1000);
		return;
	}

}

void PersonalAccountWidget::displayUserInfo(const QString &info) {


	m_userInfoLabel->setText(info);

}

void PersonalAccountWidget::updateCarImage(const QString &carName) {
	
	// Проверка на пустую строку
	if(carName.isEmpty())
	{
		qDebug() << "Car name is empty, using default image";
		loadDefaultImage();
		return;
	}

	// Создаем map для соответствия названий и путей к изображениям
	static const QMap<QString, QString> carImages = {

		{"Седан", ":/image/car/sedan.png"},
		{"Джип", ":/image/car/jeep.png"},
		{"Кроссовер", ":/image/car/crossover.png"},
		{"Универсал", ":/image/car/universal.png"},

	};

	QString imagePath = carImages.value(carName, ":/image/car/default.png");


	qDebug() << "Trying to load image from:" << carName;
	loadCarImage(imagePath);
}



void PersonalAccountWidget::loadCarImage(const QString &imagePath) {

	QPixmap pixmap(imagePath);

	QPixmap scaled = pixmap.scaled(m_carImageLabel->size(),
		Qt::KeepAspectRatioByExpanding,
		Qt::SmoothTransformation
	);

	//m_carImageLabel->setFixedSize(295, 200);
	// Устанавливаем атрибуты для поддержки прозрачности
	m_carImageLabel->setAttribute(Qt::WA_TranslucentBackground);
	/*m_carImageLabel->setStyleSheet(
		"background: transparent;"
	);*/

	m_carImageLabel->setAlignment(Qt::AlignCenter);
	m_carImageLabel->setPixmap(scaled);

}

void PersonalAccountWidget::loadDefaultImage()
{
	loadCarImage(":/image/car/default.png");
}