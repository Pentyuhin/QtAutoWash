#include "CalendarWidget.h"
#include <QCalendarWidget>
#include <QVBoxLayout>


CalendarWidget::CalendarWidget(QWidget* parent) :
	QWidget(parent) 
{
	
	setupUI();

}

QVBoxLayout* CalendarWidget::containerTimeUI()
{
    // Главный контейнер (сделаем его членом класса, чтобы не терять)
    QWidget* timeContainer = new QWidget;
    timeContainer->setFixedSize(300, 230);
    timeContainer->setStyleSheet(
        "background-color: #696969;"
        "border-radius: 10px;"
    );

    // Создаем и настраиваем layout
    QVBoxLayout* timeContainerLayout = new QVBoxLayout(timeContainer);
    timeContainerLayout->setContentsMargins(0, 0, 0, 0);
    timeContainerLayout->setAlignment(Qt::AlignCenter);

    // Убираем margin-top из стилей и добавляем через layout
    timeContainerLayout->setContentsMargins(0, 15, 0, 0);

    return timeContainerLayout;
}


void CalendarWidget::setupUI() {

   

    // Главный контейнер
    QWidget* calendarContainer = new QWidget(this);
    calendarContainer->setFixedSize(300, 230);
    calendarContainer->setStyleSheet(
        "background-color: #404040;"
        "border-radius: 10px;"
        "margin-top: 15px;"
    );

    // Получаем layout из метода
    QVBoxLayout* timeLayout = containerTimeUI();

    // Layout контейнера - теперь с выравниванием по центру
    QVBoxLayout* calContainerLayout = new QVBoxLayout(calendarContainer);
    calContainerLayout->setContentsMargins(0, 0, 0, 0);  // Убрали все внутренние отступы
    calContainerLayout->setAlignment(Qt::AlignCenter);   // Центрируем содержимое

    // Создаем календарь
    QCalendarWidget* calendar = new QCalendarWidget();
    calendar->setFixedSize(280, 225);  // Размер немного меньше контейнера

    // Упрощенный стиль для лучшего контроля
    calendar->setStyleSheet(R"(
        QCalendarWidget {
            background-color: #696969;
            border: 1px solid #3a3a3a;
            border-radius: 8px;
           /* margin: 0;
            padding: 0;*/
        }
        QCalendarWidget QWidget#qt_calendar_navigationbar {
            background-color: #252525;
            border: none;
            height: 25px;
        }
        QCalendarWidget QToolButton {
            background-color: #3a3a3a;
            color: #A9A9A9;
            font-size: 9px;
            border: none;
            min-width: 40px;
            min-height: 40px;
        }
        QCalendarWidget QToolButton:hover {
            background-color: #ffcc00;
        }
        /* Заголовки дней недели */
        QCalendarWidget QAbstractItemView {
            background: #3a3a3a;
            color: #A9A9A9;
            selection-background-color: #ffcc00;
            selection-color: black;
        }
        /* Неактивные дни (из других месяцев) */
        QCalendarWidget QAbstractItemView:disabled {
            color: #666666;
        }
        
        /* Сегодняшняя дата */
        QCalendarWidget QAbstractItemView:selected {
            border: 1px solid #ffffff;
        }
        /* Выпадающее меню */
        QCalendarWidget QMenu {
            background-color: #3a3a3a;
            color: white;
            border: 1px solid #ffcc00;
        }
    )");



    // Добавляем календарь в контейнер
    calContainerLayout->addWidget(calendar);
    calContainerLayout->addLayout(containerTimeUI());


    // Настройка главного layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);  // Убираем отступы у главного виджета
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(calendarContainer);
    mainLayout->addStretch();
}