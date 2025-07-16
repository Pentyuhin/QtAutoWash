#include "AuthorizationsWidget.h"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QThread>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>




AuthorizationsWidget::AuthorizationsWidget(QWidget* parent)
	: QWidget(parent),
	m_userName(new QLineEdit(this)),
	m_titleWindow(new QLabel(this)),
	m_userPassword(new QLineEdit(this))
{

	setupUI();

	// Подключаем клик кнопки к испусканию сигнала
	connect(m_registrationBtn, &QPushButton::clicked, this, &AuthorizationsWidget::registrationRequested);
	connect(m_enterAppBtn, &QPushButton::clicked, this, &AuthorizationsWidget::onOkClicked);
	
}


void AuthorizationsWidget::setupUI() {


	// Контейнер
	QWidget* container = new QWidget(this);
	container->setFixedSize(260, 350);
	container->setStyleSheet("background-color: #F8D500; border-radius: 10px;");

	// Создаем QLabel
	QLabel* m_carImgLabel = new QLabel();
	// Загружаем изображение из файла
	QPixmap pixmap(":/image/car_logo.png");
	// Устанавливаем изображение в QLabel
	// Масштабируем с сохранением пропорций
	QPixmap scaled = pixmap.scaled(container->size(),
		Qt::KeepAspectRatio,
		Qt::SmoothTransformation);

	m_carImgLabel->setAlignment(Qt::AlignCenter);
	m_carImgLabel->setStyleSheet(
		"border-radius: 10px;"
	);

	m_carImgLabel->setPixmap(scaled);


	m_userName->setPlaceholderText("Например: Иван");
	m_userPassword->setPlaceholderText("**********");

	// Layout формы
	QFormLayout* formLayout = new QFormLayout();
	formLayout->setSpacing(10);
	formLayout->setContentsMargins(10, 10, 10, 10);

	formLayout->addRow(new QLabel("Авторизация"));
	formLayout->addRow(new QLabel("Phone:"), m_userName);
	formLayout->addRow(new QLabel("Пароль:"), m_userPassword);

	m_userName->setInputMask("+7 (999) 999-99-99");
	m_userPassword->setEchoMode(QLineEdit::Password);

	// Главный layout контейнера
	QVBoxLayout* containerLayout = new QVBoxLayout(container);
	containerLayout->addWidget(m_carImgLabel);
	containerLayout->addLayout(formLayout);
	// Добавляем кнопки (горизонтальный layout)
	QHBoxLayout* buttonLayout = new QHBoxLayout();
	buttonLayout->setSpacing(5);

	m_enterAppBtn = new QPushButton("OK");
	m_exitAppBtn = new QPushButton("Отмена");
	m_registrationBtn = new QPushButton("Регистрация");


	// Настройка стилей кнопок
	QString buttonStyle = 
		"QWidget {"
		"	padding-top: 10px;"
		"	background-color: #F8D500; border-radius: 10px;"
		"	color: 030303;"
		"}"
		"QLabel {"
		"	font-weight: bold;"
		"	font-size: 12px;"
		"	qproperty-alignment: AlignCenter;"
		"}"

		"QLineEdit {"
		"	font-size: 12px;"
		"	background: white;" 
		"	border: 1px solid #ccc;"
		"	padding: 3px; "
		"}"

		"QPushButton {"
		"	background-color: #F8D500;"
		"	text-decoration: none;"
		"	color: #030202;"
		"	border-radius: 2px;"
		"	border: 2px solid #4a4848;"
		"	padding: 5px;"
		"	min-width: 50px;"
		"	margin-bottom: 5px;"
		"	}"
		"QPushButton:hover {"
		"	background-color: #4a4848;"
		"	color: #f0e9e9;"
		"}";


	container->setStyleSheet(buttonStyle);

	buttonLayout->addWidget(m_enterAppBtn);
	buttonLayout->addWidget(m_exitAppBtn);
	buttonLayout->addWidget(m_registrationBtn);

	containerLayout->addLayout(buttonLayout);

	// Размещение контейнера по центру
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(container, 0, Qt::AlignCenter);
	
	// Убедитесь, что установили layout для текущего виджета
	setLayout(mainLayout);

}



bool AuthorizationsWidget::authenticateUser(const QString& login, const QString& password) {
	QSqlQuery query;
	query.prepare("SELECT id, name, email, phone, password_hash FROM users WHERE phone = :login OR email = :login ");

	query.bindValue(":login", login);

	//if (!query.exec()) {
	//	qDebug() << "Ошибка запроса:" << query.lastError().text();
	//	return;
	//}

	//qDebug() << "ID | Имя | Email | Телефон";
	//qDebug() << "--------------------------";
	//while (query.next()) {
	//	QString id = query.value("id").toString();
	//	QString name = query.value("name").toString();
	//	QString email = query.value("email").toString();
	//	QString phone = query.value("phone").toString();
	//	qDebug() << id << "|" << name << "|" << email << "|" << phone;
	//}


	//QSqlQuery query;
	//query.prepare("SELECT * FROM users");

	//query.bindValue(":login", login);

	if (!query.exec()) {
		qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
		return false;
	}

	if (!query.next()) {
		qDebug() << "Пользователь не найден";
		QThread::msleep(1000);
		return false;
	}

	QString user_id = query.value("id").toString(); // user_id

	emit getUserId(user_id);

	qDebug() << "id user: " << user_id;

	QString storedHash = query.value("password_hash").toString(); // password_hash

	//qDebug() << "ID | Имя | Email | Телефон";
	//qDebug() << "--------------------------";
	//while (query.next()) {
	//	QString id = query.value("id").toString();
	//	QString name = query.value("name").toString();
	//	QString email = query.value("email").toString();
	//	QString phone = query.value("phone").toString();
	//	QString storedHash = query.value("password_hash").toString(); // password_hash
	//	qDebug() << id << "|" << name << "|" << email << "|" << phone;
	//}

	//QString storedHash = query.value("password_hash").toString(); // password_hash

	// Хешируем введенный пароль
	QString hashedPassword = m_carInfoWidget->hashPasswordPbkdf2(password);

	// Сравниваем хеши
	return (storedHash == hashedPassword);
}


void AuthorizationsWidget::onOkClicked() {
	
	QString login = m_userName->text().trimmed();
	QString password = m_userPassword->text();

	// Валидация ввода
	if (login.isEmpty() || password.isEmpty()) {
	
		QMessageBox::warning(this, "Ошибка", "Заполните все поля");
		return;
	}

	//authenticateUser(login, password);
	//Проверка аутентификации
	if (authenticateUser(login, password)) {
		emit authorizationsSuccess();	// Сигнал об успешной авторизации
		QMessageBox::information(this, "Успех", "Авторизация прошла успешно!");
	}
	else {
	
		QMessageBox::warning(this, "Ошибка", "Неверные учетные данные");
		m_userPassword->clear();
	}


}
