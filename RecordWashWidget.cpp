#include "RecordWashWidget.h"
#include <QCheckBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QButtonGroup>
#include <QScrollArea>
#include <QPropertyAnimation>


#include <QDebug> // Удалить после тестов 

RecordWash::RecordWash(QWidget* parent) :
    QWidget(parent),
    m_times(new QList<QTime>()),
    m_dates(new QList<QDate>())
{


    printTimeSlots();
    printDateSlots();
    setupUI();
}

RecordWash::~RecordWash()
{
    delete m_times;
    delete m_dates;
}


void RecordWash::setupUI()
{
    // Основной контейнер и layout
    QWidget* mainContainer = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(mainContainer);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Создаем виджеты тарифов и дополнительных услуг
    QWidget* ratesWidget = createRatesWidget(mainContainer);
    QWidget* servicesWidget = createServicesWidget(mainContainer);
    QWidget* dateButtonWidget = createDateWidget(mainContainer);
    QWidget* timeButtonWidget = createTimeWidget(mainContainer);

    // Выравниваем ширину виджетов
    alignWidgetsWidth({ ratesWidget, servicesWidget });

    // Добавляем виджеты в основной layout
    mainLayout->addWidget(ratesWidget);
    mainLayout->addWidget(servicesWidget);
    mainLayout->addWidget(dateButtonWidget);
    mainLayout->addWidget(timeButtonWidget);

    this->setLayout(mainLayout);
}


QWidget* RecordWash::createRatesWidget(QWidget* parent)
{
    QWidget* widget = new QWidget(parent);
    widget->setStyleSheet(
        "background-color: #696969;"
        "border-radius: 10px;"
    );
    widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setContentsMargins(15, 10, 15, 10);
    layout->setSpacing(20);

    // Заголовок
    layout->addWidget(createLabel("Выберите тариф"));

    // Чекбоксы тарифов
    QCheckBox* standardRate = createCheckBox(
        "Стандарт\nМойка ковриков, мойка кузова,\nсушка и протирка кузова,\nополаскивание кузова\n\n750",
        widget);

    QCheckBox* complexRate = createCheckBox(
        "Комплекс\nМойка ковриков, мойка кузова,\nсушка и протирка кузова,\nополаскивание кузова,\nпылесос салона, влажная уборка салона\n\n1350",
        widget);

    layout->addWidget(standardRate);
    layout->addWidget(complexRate);

    // Группируем чекбоксы
    QButtonGroup* group = new QButtonGroup(this);
    group->addButton(standardRate);
    group->addButton(complexRate);
    group->setExclusive(true);

    return widget;
}

QWidget* RecordWash::createServicesWidget(QWidget* parent)
{

    QWidget* widget = new QWidget(parent);
    widget->setStyleSheet(
        "background-color: #696969;"
        "border-radius: 10px;"
    );



    widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QVBoxLayout* mainLayout = new QVBoxLayout(widget);
    mainLayout->setContentsMargins(15, 10, 15, 10);
    mainLayout->setSpacing(20);

    // 1. Заголовок (всегда видимый)
    mainLayout->addWidget(createLabel("Дополнительные услуги"));

    // 2. Создаем контейнер для скрываемого содержимого
    QWidget* spoilerContent = new QWidget(widget);
    QVBoxLayout* spoilerLayout = new QVBoxLayout(spoilerContent);
    spoilerLayout->setContentsMargins(0, 0, 0, 0);
    spoilerLayout->setSpacing(20);

    // 3. Добавляем чекбоксы в спойлер
    const QList<QPair<QString, QString>> services = {
        {"Чернение шин(4шт)", "300"},
        {"Полировка панелей салона", "250"},
        {"Кондиционер кожи", "400"},
        {"Пылесос багажника", "250"},
        {"Покрытие воском", "350"}
    };

    for (const auto& service : services) {
        QString text = QString("%1\n%2 руб.").arg(service.first, service.second);
        spoilerLayout->addWidget(createCheckBox(text, spoilerContent));
    }

    // 4. Добавляем спойлер в основной layout
    mainLayout->addWidget(spoilerContent);
    spoilerContent->setVisible(false);  // Изначально скрыт

    // 5. Кнопка управления видимостью
    QPushButton* toggleButton = new QPushButton("Показать больше", widget);
    toggleButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #FFFF00;"
        "   border-radius: 5px;"
        "   padding: 8px;"
        "   color: #696969;"
        "}"
        "QPushButton:hover {"
        "   background-color: #FFD700;"
        "}"
    );
    mainLayout->addWidget(toggleButton);

    // 6. Подключение сигнала
    connect(toggleButton, &QPushButton::clicked, this, [toggleButton, spoilerContent]() {
        bool willBeVisible = !spoilerContent->isVisible();
        spoilerContent->setVisible(willBeVisible);
        toggleButton->setText(willBeVisible ? "Скрыть" : "Показать больше");
        spoilerContent->parentWidget()->adjustSize();
        });

    return widget;
}


template<typename T>
QWidget* RecordWash::createButtonGridWidget(
    QWidget* parent,
    const QString& title,
    const QList<T>& items,
    std::function<QString(const T&)> toStringFunc)
{
    // Создаем основной контейнер
    QWidget* container = new QWidget(parent);
    container->setStyleSheet(
        "background-color: #696969;"
        "border-radius: 10px;"
        
        
    );

    // Главный layout для контейнера
    QVBoxLayout* mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Добавляем заголовок
    QLabel* titleLabel = createLabel(title);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Создаем виджет для сетки кнопок
    QWidget* gridWidget = new QWidget();
    QGridLayout* gridLayout = new QGridLayout(gridWidget);
    gridLayout->setContentsMargins(15, 10, 15, 10);
    gridLayout->setSpacing(15);
    gridLayout->setVerticalSpacing(20);

    int row = 0;
    int col = 0;
    int buttonCount = 0;

    for (const auto& item : items) {
        QPushButton* button = createButton(toStringFunc(item));


        if (buttonCount < items.size()) {
            gridLayout->addWidget(button, row, col);
            col++;
            if (col > 1) {
                col = 0;
                row++;
            }
        }
        else {
            gridLayout->addWidget(button, row, 0, 1, 2);
        }

        buttonCount++;
    }

    // Создаем QScrollArea и настраиваем ее
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(gridWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    // Желто-серый стиль прокрутки
    scrollArea->setStyleSheet(
        "QScrollArea {"
        "    border: none;"
        "    background: transparent;"
        "}"
        "QScrollBar:vertical {"
        "    background: #505050;"  // Серый фон полосы прокрутки
        "    width: 10px;"
        "    margin: 0px;"
        "    border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #FFD700;"  // Желтый ползунок
        "    min-height: 30px;"
        "    border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #FFC000;"  // Более темный желтый при наведении
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "    background: none;"
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "    background: none;"
        "}"
    );

    // Добавляем scrollArea в основной layout
    mainLayout->addWidget(scrollArea);

    return container;
}

// Тогда специализированные методы будут выглядеть так:
QWidget* RecordWash::createDateWidget(QWidget* parent) {
    return createButtonGridWidget<QDate>(
        parent,
        "Дата для записи",
        *m_dates,
        [](const QDate& date) { return date.toString("dd.MM.yyyy"); }
    );
}

QWidget* RecordWash::createTimeWidget(QWidget* parent) {
    return createButtonGridWidget<QTime>(
        parent,
        "Время для записи",
        *m_times,
        [](const QTime& time) { return time.toString("hh:mm"); }
    );
}

//QWidget* RecordWash::createDateWidget(QWidget* parent) {
//
//    QWidget* widget = new QWidget(parent);
//    widget->setStyleSheet(
//        "background-color: #696969;"
//        "border-radius: 10px;"
//    );
//
//    // Используем QGridLayout вместо QVBoxLayout
//    QGridLayout* gridLayout = new QGridLayout(widget);
//    gridLayout->setContentsMargins(15, 10, 15, 10);
//    gridLayout->setSpacing(15);
//    gridLayout->setVerticalSpacing(20);
//
//    // Добавляем заголовок (занимает всю ширину)
//    QLabel* titleLabel = createLabel("Дата для записи");
//    gridLayout->addWidget(titleLabel, 0, 0, 1, 2, Qt::AlignCenter);
//
//    // Добавляем кнопки с датами в конфигурации 2-2-1
//    int row = 1;  // Начинаем с ряда после заголовка
//    int col = 0;
//    int buttonCount = 0;
//
//    for (const auto& date : *m_dates) {
//        QPushButton* button = createButton(date.toString("dd.MM.yyyy"));
//
//        if (buttonCount < 4) {
//            // Первые 4 кнопки - 2 в первом ряду, 2 во втором
//            gridLayout->addWidget(button, row, col);
//            col++;
//            if (col > 1) {
//                col = 0;
//                row++;
//            }
//        }
//        else {
//            // 5-я кнопка - по центру в третьем ряду
//            gridLayout->addWidget(button, row, 0, 1, 2);
//        }
//
//        buttonCount++;
//    }
//
//    return widget;
//}
//
//QWidget* RecordWash::createTimeWidget(QWidget* parent) {
//
//    QWidget* widget = new QWidget(parent);
//    widget->setStyleSheet(
//        "background-color: #696969;"
//        "border-radius: 10px;"
//    );
//
//    // Используем QGridLayout вместо QVBoxLayout
//    QGridLayout* gridLayout = new QGridLayout(widget);
//    gridLayout->setContentsMargins(15, 10, 15, 10);
//    gridLayout->setSpacing(15);
//    gridLayout->setVerticalSpacing(20);
//
//    // Добавляем заголовок (занимает всю ширину)
//    QLabel* titleLabel = createLabel("Дата для записи");
//    gridLayout->addWidget(titleLabel, 0, 0, 1, 2, Qt::AlignCenter);
//
//    // Добавляем кнопки с датами в конфигурации 2-2-1
//    int row = 1;  // Начинаем с ряда после заголовка
//    int col = 0;
//    int buttonCount = 0;
//
//    for (const auto& time : *m_times) {
//        QPushButton* button = createButton(time.toString("hh:mm"));
//
//        if (buttonCount < 4) {
//            // Первые 4 кнопки - 2 в первом ряду, 2 во втором
//            gridLayout->addWidget(button, row, col);
//            col++;
//            if (col > 1) {
//                col = 0;
//                row++;
//            }
//        }
//        else {
//            // 5-я кнопка - по центру в третьем ряду
//            gridLayout->addWidget(button, row, 0, 1, 2);
//        }
//
//        buttonCount++;
//    }
//
//    return widget;
//}

QPushButton* RecordWash::createButton(const QString& text) {

    QPushButton* button = new QPushButton(text);
    button->setStyleSheet(
        "QPushButton {"
        "   background-color: #FFFF00;"
        "   color: #696969;"
        "   border-radius: 5px;"
        "   padding: 8px;"
        "}"
        "QPushButton:hover { background-color: #FFD700; }"
    );
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    return button;
}


QLabel* RecordWash::createLabel(const QString& text)
{
    QLabel* label = new QLabel(text);
    label->setStyleSheet("font-weight: bold; color: white;");
    return label;
}

QCheckBox* RecordWash::createCheckBox(const QString& text, QWidget* parent)
{
    QCheckBox* checkBox = new QCheckBox(text, parent);

    // Создаем и настраиваем шрифт
    QFont checkboxFont;
    checkboxFont.setFamily("Segoe UI");       // Название шрифта
    checkboxFont.setPointSize(10);         // Размер шрифта
    checkboxFont.setWeight(QFont::Bold);   // Жирность

    checkBox->setFont(checkboxFont);       // Применяем шрифт

    checkBox->setStyleSheet(
        "QCheckBox {"
        "    color: #FFFFFF;"
        "    spacing: 8px;"
        "}"
        "QCheckBox::indicator {"
        "    width: 15px;"
        "    height: 15px;"
        "    border: 2px solid yellow;"
        "    border-radius: 3px;"
        "    background: transparent;"
        "}"
        "QCheckBox::indicator:checked {"
        "    background-color: yellow;"
        //"    image: url(:/checked.png);"
        "}"
        "QCheckBox::indicator:unchecked {"
        "    background: transparent;"
        "}"
        "QCheckBox::indicator:hover {"
        "    border: 2px solid #FFD700;"
        "}"
    );

    // Дополнительно: минимальная высота для правильного отображения
    checkBox->setMinimumHeight(30);

    return checkBox;
}

void RecordWash::alignWidgetsWidth(const QList<QWidget*>& widgets)
{
    if (widgets.isEmpty()) return;

    int maxWidth = 0;
    for (QWidget* widget : widgets) {
        maxWidth = std::max(maxWidth, widget->sizeHint().width());
    }

    for (QWidget* widget : widgets) {
        widget->setMinimumWidth(maxWidth);
    }
}


void RecordWash::printTimeSlots() {
    // Очищаем список перед заполнением
    m_times->clear();

    // Получаем текущее время
    QTime currentTime = QTime::currentTime();

    // Определяем минимальное (8:00) и максимальное (21:00) время
    const QTime minTime(8, 0);
    const QTime maxTime(21, 0);

    // Если текущее время раньше 8:00, начинаем с 8:00
    if (currentTime < minTime) {
        currentTime = minTime;
    }
    // Если текущее время позже 21:00, начинаем со следующего дня
    else if (currentTime > maxTime) {
        currentTime = minTime.addSecs(86400); // Добавляем сутки
    }

    // Округляем до следующего получаса
    int minutes = currentTime.minute();
    int roundedMinutes = ((minutes + 29) / 30) * 30; // Округление вверх до 30 минут
    if (roundedMinutes >= 60) {
        roundedMinutes = 0;
        currentTime = currentTime.addSecs(3600); // Добавляем час
    }

    QTime nextSlot(currentTime.hour(), roundedMinutes);

    // Корректируем, если вышли за границы после округления
    if (nextSlot < minTime) {
        nextSlot = minTime;
    }
    else if (nextSlot > maxTime) {
        nextSlot = minTime.addSecs(86400); // Следующий день
    }

    // Выводим доступные временные слоты
    qDebug() << "Доступные временные слоты:";

    QTime slot = nextSlot;
    while (slot >= minTime && slot <= maxTime) {
        // qDebug() << slot.toString("hh:mm");
        m_times->append(slot);

        // Добавляем 30 минут
        slot = slot.addSecs(1800);
       
        // Если перешли через полночь
        if (slot.hour() == 0 && slot.minute() == 0) {
            slot = minTime; // Начинаем с 8:00 следующего дня
        }

    }

    // Для отладки выводим результат
    qDebug() << "Сгенерированные временные слоты:";
    for (const QTime& time : *m_times) {
        qDebug() << time.toString("hh:mm");
    }
}


void RecordWash::printDateSlots() {

    m_dates->clear();

    QDate currentDate = QDate::currentDate();

    for (int i = 0; i < 5; ++i) {
        QDate newDate = currentDate.addDays(i);

        m_dates->append(newDate);

    }


    qDebug() << "Сгенерированные date слоты:";
    for (const QDate& d : *m_dates) {

        qDebug() << "Date current: " << d.toString("dd.MM.yyyy");

    }
}