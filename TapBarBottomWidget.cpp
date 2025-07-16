#include "TapBarBottomWidget.h"

#include <QHBoxLayout>

TapBarBottomWidget::TapBarBottomWidget(QWidget* parent) :
	QWidget(parent),
	m_settingsBtn(new QPushButton()),
	m_personalAccountBtn(new QPushButton()),
	m_mapsBtn(new QPushButton()),
	m_recordBtn(new QPushButton())
{

	setupUI();

}


void TapBarBottomWidget::setupUI() {

	QWidget* tapBarContainer = new QWidget(this);
	tapBarContainer->setStyleSheet(
		"background-color: #696969;"
		"border-radius: 10px;"  
	);


	QHBoxLayout* containerLayout = new QHBoxLayout(tapBarContainer);
	containerLayout->setContentsMargins(15, 10, 15, 10);  // Внутренние отступы
	containerLayout->setSpacing(20); // Расстояние между кнопками

	// Настройка кнопок
	setupButton(m_settingsBtn, ":/image/settings.png");
	setupButton(m_personalAccountBtn, ":/image/account.png");
	setupButton(m_mapsBtn, ":/image/map.png");
	setupButton(m_recordBtn, ":/image/calendar.png");

	containerLayout->addWidget(m_settingsBtn);
	containerLayout->addWidget(m_personalAccountBtn);
	containerLayout->addWidget(m_mapsBtn);
	containerLayout->addWidget(m_recordBtn);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 10);  // Убираем отступы у главного layout
	mainLayout->addStretch();  // Растягиваем пространство сверху
	// Добавляем контейнер с выравниванием по центру
	mainLayout->addWidget(tapBarContainer, 0, Qt::AlignCenter);

	// Устанавливаем выравнивание всего layout по центру
	mainLayout->setAlignment(Qt::AlignCenter);
}

void TapBarBottomWidget::setupButton(QPushButton* button, const QString &iconPath) {

	// Загружаем SVG иконку
	QIcon icon(iconPath);

	button->setIcon(icon);
	button->setIconSize(QSize(32, 32));

	// Стилизация кнопки
	button->setStyleSheet(
		"QPushButton {"
		"   background: transparent;"
		"   border: none;"
		"   padding: 10px;"
		"}"
		"QPushButton:hover {"
		"   background: rgba(255, 255, 255, 0.1);"
		"   border-radius: 10px;"
		"}"
		"QPushButton:pressed {"
		"   background: rgba(255, 255, 255, 0.2);"
		"}"
	);

	// Фиксируем размер кнопки
	button->setFixedSize(50, 50);
}