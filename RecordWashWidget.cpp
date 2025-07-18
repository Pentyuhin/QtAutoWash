#include "RecordWashWidget.h"
#include <QCheckBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QButtonGroup>

#include <QDebug> // Удалить после тестов 

RecordWash::RecordWash(QWidget* parent) :
    QWidget(parent),
    m_times(new QList<QTime>()),
    m_dates(new QList<QDate>())
{


	setupUI();
    printTimeSlots();
    printDateSlots();

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

    // Выравниваем ширину виджетов
    alignWidgetsWidth({ ratesWidget, servicesWidget });

    // Добавляем виджеты в основной layout
    mainLayout->addWidget(ratesWidget);
    mainLayout->addWidget(servicesWidget);

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
        "background-color: #698585;"
        "border-radius: 10px;"
    );
    widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setContentsMargins(15, 10, 15, 10);
    layout->setSpacing(20);

    // Заголовок
    layout->addWidget(createLabel("Дополнительные услуги"));

    // Чекбоксы услуг
    const QList<QPair<QString, QString>> services = {
        {"Чернение шин(4шт)", "300"},
        {"Полировка панелей салона", "250"},
        {"Кондиционер кожи", "400"},
        {"Пылесос багажника", "250"},
        {"Покрытие воском", "350"}
    };

    for (const auto& service : services) {
        QString text = QString("%1\n\n%2").arg(service.first, service.second);
        layout->addWidget(createCheckBox(text, widget));
    }

    return widget;
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
    checkBox->setStyleSheet("color: white;");
    //checkBox->setWordWrap(true);
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


//void RecordWash::setupUI()
//{
//    // Создаем основной контейнер для выравнивания
//    QWidget* mainContainer = new QWidget(this);
//    QVBoxLayout* mainLayout = new QVBoxLayout(mainContainer);
//    mainLayout->setAlignment(Qt::AlignCenter);  // Центрируем содержимое
//
//    // Первый виджет (тарифы)
//    QWidget* recordContainerWidget = new QWidget(mainContainer);
//    recordContainerWidget->setStyleSheet(
//        "background-color: #696969;"
//        "border-radius: 10px;"
//    );
//
//    // Второй виджет (доп. услуги)
//    QWidget* recordContainerWidgetDU = new QWidget(mainContainer);
//    recordContainerWidgetDU->setStyleSheet(
//        "background-color: #698585;"
//        "border-radius: 10px;"
//    );
//
//    // Устанавливаем одинаковую политику размеров
//    recordContainerWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
//    recordContainerWidgetDU->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
//
//    // Создаем layout для первого виджета
//    QVBoxLayout* containerLayout = new QVBoxLayout(recordContainerWidget);
//    containerLayout->setContentsMargins(15, 10, 15, 10);
//    containerLayout->setSpacing(20);
//
//    // Создаем layout для второго виджета
//    QVBoxLayout* containerLayoutDu = new QVBoxLayout(recordContainerWidgetDU);
//    containerLayoutDu->setContentsMargins(15, 10, 15, 10);
//    containerLayoutDu->setSpacing(20);
//
//    // Добавляем элементы в первый виджет
//    QLabel* titleTr = new QLabel("Выберите тариф");
//    QCheckBox* checkBoxSt = new QCheckBox("Стандарт\n Мойка ковриков, мойка кузова,\n сушка и протирка кузова,\n ополаскивание кузова \n\n 750", recordContainerWidget);
//    QCheckBox* checkBoxKm = new QCheckBox("Комплекс\n Мойка ковриков, мойка кузова,\n сушка и протирка кузова,\n ополаскивание кузова,\n пылесос салона, влажная уборка салона \n\n 1350", recordContainerWidget);
//
//    // Добавляем элементы во второй виджет
//    QLabel* titleDu = new QLabel("Дополнительные услуги");
//    QCheckBox* checkBoxTires = new QCheckBox("Чернение шин(4шт) \n\n 300", recordContainerWidgetDU);
//    QCheckBox* checkBoxPolishing = new QCheckBox("Полировка панелей салона \n\n 250", recordContainerWidgetDU);
//    QCheckBox* checkBoxLeather = new QCheckBox("Кондиционер кожи \n\n 400", recordContainerWidgetDU);
//    QCheckBox* checkBoxTrunk = new QCheckBox("Пылесос багажника \n\n 250", recordContainerWidgetDU);
//    QCheckBox* checkBoxWax = new QCheckBox("Покрытие воском \n\n 350", recordContainerWidgetDU);
//
//    // Группируем чекбоксы тарифов
//    QButtonGroup* buttonGroup = new QButtonGroup(this);
//    buttonGroup->addButton(checkBoxSt);
//    buttonGroup->addButton(checkBoxKm);
//    buttonGroup->setExclusive(true);
//
//    // Добавляем виджеты в layouts
//    containerLayout->addWidget(titleTr);
//    containerLayout->addWidget(checkBoxSt);
//    containerLayout->addWidget(checkBoxKm);
//
//    containerLayoutDu->addWidget(titleDu);
//    containerLayoutDu->addWidget(checkBoxTires);
//    containerLayoutDu->addWidget(checkBoxPolishing);
//    containerLayoutDu->addWidget(checkBoxLeather);
//    containerLayoutDu->addWidget(checkBoxTrunk);
//    containerLayoutDu->addWidget(checkBoxWax);
//
//    // Добавляем оба контейнера в основной layout
//    mainLayout->addWidget(recordContainerWidget);
//    mainLayout->addWidget(recordContainerWidgetDU);
//
//    // Устанавливаем минимальную ширину равной максимальной ширине из двух виджетов
//    int maxWidth = std::max(
//        recordContainerWidget->sizeHint().width(),
//        recordContainerWidgetDU->sizeHint().width()
//    );
//    recordContainerWidget->setMinimumWidth(maxWidth);
//    recordContainerWidgetDU->setMinimumWidth(maxWidth);
//
//    // Устанавливаем основной layout
//    this->setLayout(mainLayout);
//}


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