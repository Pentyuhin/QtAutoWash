#include "CarInfoWidget.h"
#include <QVBoxLayout>
#include <QIcon>
#include <QDebug>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlQuery>
#include <QUuid>
#include <QMessageAuthenticationCode>


CarInfoWidget::CarInfoWidget(QWidget* parent)
    : QWidget(parent),
    m_modelEdit(new QLineEdit(this)),
    m_plateEdit(new QLineEdit(this)),
    m_colorCombo(new QComboBox(this)),
    m_validationIcon(new QLabel(this)),
    m_carImageLabel(new QLabel(this)),
    m_modelCombo(new QComboBox(this)),
    m_nameEdit(new QLineEdit(this)),
    m_emailEdit(new QLineEdit(this)),
    m_phoneEdit(new QLineEdit(this)),
    m_passHashEdit(new QLineEdit(this)),
    m_cleanBtn(new QPushButton()),
    m_saveBtn(new QPushButton())
   
{
    setupUI();
    setupValidators();

    connect(m_modelEdit, &QLineEdit::textChanged, this, &CarInfoWidget::validateInput);
    connect(m_plateEdit, &QLineEdit::textChanged, this, &CarInfoWidget::validateInput);
    connect(m_colorCombo, &QComboBox::currentIndexChanged, this, &CarInfoWidget::validateInput);

    connect(m_modelCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CarInfoWidget::updateCarImage);
    connect(m_cleanBtn, &QPushButton::clicked, this, &CarInfoWidget::clear);
    connect(m_saveBtn, &QPushButton::clicked, this, &CarInfoWidget::saveUserData);
}

void CarInfoWidget::setupUI()
{
    QWidget* containerWeather = new QWidget(this);
    containerWeather->setStyleSheet(
        "   background-color: #696969;"
        "   border-radius: 10px;"
        "   margin-right: 10px;"
        "   margin-left: 10px;"
        "   margin-bottom: 10px;"
    );

 
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QFormLayout* formLayout = new QFormLayout(containerWeather);


    m_saveBtn = new QPushButton("Сохранить");
    m_saveBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #333333;"
        "   border: none;"
        "   color: #f0e9e9;"
        "   padding: 10px 20px;"
        "   font-size: 16px;"
        "   transition-duration: 0.4s;"
        "   border: 2px solid #F8D500;"
        "   margin-right: 20px;"
        "   margin-left: 20px;"
        "   text-decoration: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: #F8D500;"
        "   color: #030202;"
        "}"
    );

    m_cleanBtn = new QPushButton("Очистить форму");
    m_cleanBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #333333;"
        "   border: none;"
        "   color: #f0e9e9;"
        "   padding: 10px 20px;"
        "   font-size: 16px;"
        "   transition-duration: 0.4s;"
        "   border: 2px solid #F8D500;"
        "   margin-top: 5px;"
        "   margin-right: 20px;"
        "   margin-left: 20px;"
        "   margin-bottom: 20px;"
        "   text-decoration: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: #F8D500;"
        "   color: #030202;"
        "}"
    );

    // Настройка QLabel для отображения изображения
    m_carImageLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_carImageLabel->setWindowFlags(Qt::FramelessWindowHint);
    m_carImageLabel->setAlignment(Qt::AlignCenter);
    m_carImageLabel->setFixedSize(350, 230);
    m_carImageLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    //m_carImageLabel->setStyleSheet("background-color: yellow; border-radius: 10px; margin: 10, 10, 10, 10;");


    mainLayout->addWidget(m_carImageLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(containerWeather, 0, Qt::AlignCenter);
    mainLayout->addWidget(m_saveBtn);
    mainLayout->addWidget(m_cleanBtn);

    // Настройка элементов ввода 
    m_modelEdit->setPlaceholderText("Toyota Camry");
    m_plateEdit->setPlaceholderText("А123БВ777");
    m_nameEdit->setPlaceholderText("Иван");
    m_emailEdit->setPlaceholderText("googl@mail.com");
    m_phoneEdit->setPlaceholderText("+79000000000");
    m_passHashEdit->setPlaceholderText("************");



    // Заполнение списка цветов
    QStringList colors;
    colors << "Белый" << "Чёрный" << "Серый"
        << "Красный" << "Синий" << "Зелёный"
        << "Жёлтый" << "Другой";
    m_colorCombo->addItems(colors);

    QStringList models;
    models << "Седан" << "Джип" << "Кроссовер" << "Универсал" << "Картина не выбрана";
    m_modelCombo->addItems(models);


    // Настройка иконки валидации
    m_validationIcon->setPixmap(QIcon(":/icons/warning.png").pixmap(48, 48));
    m_validationIcon->setVisible(false);
    m_validationIcon->setToolTip("Заполните все поля корректно");


    // Компоновка формы
    formLayout->addRow(new QLabel("Имя:"), m_nameEdit);
    formLayout->addRow(new QLabel("Телефон:"), m_phoneEdit);
    formLayout->addRow(new QLabel("@email:"), m_emailEdit);
    formLayout->addRow(new QLabel("Password:"), m_passHashEdit);
    formLayout->addRow(new QLabel("Модель автомобиля:"), m_modelEdit);
    formLayout->addRow(new QLabel("Гос. номер:"), m_plateEdit);
    formLayout->addRow(new QLabel("Цвет автомобиля:"), m_colorCombo);
    formLayout->addRow(new QLabel("Кузов авто:"), m_modelCombo);
    formLayout->addRow(new QLabel(""), m_validationIcon);

    QString lineEditStyle =

        // Стиль для QLabel
        "QLabel {"
        "   color: white;"
        "   font-size: 10px;"
        "   min-width: 25px;"  // Фиксированная ширина для выравнивания
        "   padding-right: 10px;"
        "}"

        // Стиль для QLineEdit
        "QLineEdit {"
        "   background: white;"
        "   color: #7d7c7c;"
        "   border: 1px solid #333333;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   font-size: 10px;"
        "   min-height: 20px;"
        "}"
        "QLineEdit:hover {"
        "   border: 2px solid #F8D500;"
        "}"
        "QLineEdit:focus {"
        "   border: 1px solid #F8D500;"
        "   background: #E0FFFF;"
        "}"

        // Стиль для QComboBox
        "QComboBox {"
        "   color: #7d7c7c;"
        "   background: white;"
        "   border: 2px solid #F8D500;"
        "   border-radius: 5px;"
        "   padding: 6px 8px;"
        "   min-width: 25px;"
        "}"
        "QComboBox::drop-down {"
        "   width: 25px;"
        "   border-left: 2px solid #F8D500;"
        "}"
        "QComboBox::down-arrow {"
        "   image: url(:/icons/arrow-down.png);"
        "   width: 10px;"
        "   height: 10px;"
        "}"
        "QComboBox QAbstractItemView {"
        "   border: 2px solid #F8D500;"
        "   selection-background-color: #F8D500;"
        "}";
      
 
    // Дополнительные настройки для специфичных полей
    m_phoneEdit->setInputMask("+7 (999) 999-99-99");
    //m_plateEdit->setInputMask(">L LL 999 DDD"); // Пример маски для российского номера
    m_passHashEdit->setEchoMode(QLineEdit::Password);


    m_nameEdit->setStyleSheet(lineEditStyle);
    m_phoneEdit->setStyleSheet(lineEditStyle);
    m_emailEdit->setStyleSheet(lineEditStyle);
    m_passHashEdit->setStyleSheet(lineEditStyle);
    m_modelEdit->setStyleSheet(lineEditStyle);
    m_plateEdit->setStyleSheet(lineEditStyle);
    m_modelCombo->setStyleSheet(lineEditStyle);
    m_colorCombo->setStyleSheet(lineEditStyle);


    formLayout->setSpacing(10);
    formLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    


    setMinimumWidth(350);

    // Установка начального изображения
    updateCarImage(m_modelCombo->currentIndex());
}

void CarInfoWidget::updateCarImage(int index) {
    QString imagePath;

    switch (index) {
    case 0: imagePath = ":/image/car/sedan.png"; break;
    case 1: imagePath = ":/image/car/jeep.png"; break;
    case 2: imagePath = ":/image/car/crossover.png"; break;
    case 3: imagePath = ":/image/car/universal.png"; break;
    default: imagePath = ":/image/car/default.png";
    }

    qDebug() << "Trying to load image from:" << imagePath;

    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        qDebug() << "Image loaded successfully. Original size:" << pixmap.size();

        // Масштабируем с сохранением пропорций
        QPixmap scaled = pixmap.scaled(m_carImageLabel->size(),
            Qt::KeepAspectRatioByExpanding,  // Заполняет всю область с обрезкой краёв
            Qt::SmoothTransformation);
        m_carImageLabel->setAlignment(Qt::AlignCenter);
        m_carImageLabel->setAttribute(Qt::WA_TranslucentBackground);
        m_carImageLabel->setPixmap(scaled);
       
      
    }
    else {
        qDebug() << "Failed to load image!";
        showImageError(imagePath);
    }
}


void CarInfoWidget::setupValidators()
{
    // Валидатор для гос. номера (российский стандарт)
    QRegularExpression plateRegex("^[АВЕКМНОРСТУХавекмнорстух]{1}\\d{3}[АВЕКМНОРСТУХавекмнорстух]{2}\\d{2,3}$");
    QRegularExpressionValidator* plateValidator = new QRegularExpressionValidator(plateRegex, this);
    m_plateEdit->setValidator(plateValidator);

    // Валидатор для модели автомобиля
    QRegularExpression modelRegex("^[а-яА-ЯёЁa-zA-Z0-9\\s\\-]+$");
    QRegularExpressionValidator* modelValidator = new QRegularExpressionValidator(modelRegex, this);
    m_modelEdit->setValidator(modelValidator);
}

QString CarInfoWidget::carModel() const
{
    return m_modelEdit->text().trimmed();
}

QString CarInfoWidget::licensePlate() const
{
    return m_plateEdit->text().trimmed().toUpper();
}

QString CarInfoWidget::carColor() const
{
    return m_colorCombo->currentText();
}

bool CarInfoWidget::isValid() const
{
    return !carModel().isEmpty() &&
        m_plateEdit->hasAcceptableInput() &&
        m_colorCombo->currentIndex() >= 0;
}

void CarInfoWidget::validateInput()
{
    bool valid = isValid();
    m_validationIcon->setVisible(!valid);
    emit dataChanged();
}

void CarInfoWidget::clear()
{
    m_modelEdit->clear();
    m_plateEdit->clear();
    m_nameEdit->clear();
    m_emailEdit->clear();
    m_phoneEdit->clear();
    m_passHashEdit->clear();
    m_colorCombo->setCurrentIndex(0);
    m_modelCombo->setCurrentIndex(0); // Если нужно очистить и этот комбобокс
    m_validationIcon->setVisible(false);

    // Если у вас есть изображение автомобиля, можно сбросить его
    updateCarImage(m_modelCombo->currentIndex());
}

QString CarInfoWidget::userPhone() const {

    return m_phoneEdit->text();
}

QString CarInfoWidget::userName() const {

    return m_nameEdit->text();

}

QString CarInfoWidget::userEmail() const {

    return m_emailEdit->text();

}

void CarInfoWidget::showImageError(const QString& path) {
    qWarning() << "Failed to load image:" << path;
    m_carImageLabel->setText(QStringLiteral("Ошибка загрузки:\n%1").arg(path));
    m_carImageLabel->setStyleSheet("color: red; font-weight: bold;");
}


void CarInfoWidget::saveUserData() {


    if (!QSqlDatabase::database().isOpen()) {
    
        QMessageBox::critical(
            this, 
            tr("Ошибка базы данных"),
            tr("Нет подключения к базе данных"));
        return;
    }

    // Генерация нового UUID
    QString userId = QUuid::createUuid().toString(QUuid::WithoutBraces);

    //Получаем данные из формы
    QString name = m_nameEdit->text();
    QString phone = m_phoneEdit->text();
    QString email = m_emailEdit->text();
    QString password = m_passHashEdit->text();
    QString carModel = m_modelEdit->text();
    QString carPlate = m_plateEdit->text();
    QString carColor = m_colorCombo->currentText();
    QString carBody = m_modelCombo->currentText();

    // Проверяем заполненность обязательных полей
    if (carModel.isEmpty() || carPlate.isEmpty()) {
    
        QMessageBox::warning(this, "Ошибка", "Заполните все обязательные поля");
        return;
    }

    // Подготавливаем SQL-запрос
    QSqlQuery query;
    query.prepare("INSERT INTO users(id, name, email, phone, password_hash, car_model, car_num, car_color, car_body)"
        "VALUES (:id, :name, :email, :phone, :password_hash, :car_model, :car_num, :car_color, :car_body)");
    
    QString password_hash = hashPasswordPbkdf2(password);

    // Привязываем значения
    query.bindValue(":id", userId);
    query.bindValue(":name", name);
    query.bindValue(":phone", phone);
    query.bindValue(":email", email);
    query.bindValue(":password_hash", password_hash);
    query.bindValue(":car_model", carModel);
    query.bindValue(":car_num", carPlate);
    query.bindValue(":car_color", carColor);
    query.bindValue(":car_body", carBody);

    // Выполняем запрос
    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка базы данных",
            "Не удалось сохранить данные: ");
    }
    else {
    
        QMessageBox::information(this, "Успех", "Данные успешно сохранены");
        // clear(); // Очищаем форму после сохранения
    }
}

QString CarInfoWidget::hashPasswordPbkdf2(const QString& password) {
    QByteArray salt = "randomsalt123";   // Должна быть уникальной для каждого пользователя
   

    QByteArray hash = QMessageAuthenticationCode::hash(
        password.toUtf8(),
        salt,
        QCryptographicHash::Sha256
    );

    return QString(hash.toHex());
}