#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>
#include <QStatusBar>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QPushButton>


#include "WeatherWidget.h"
#include "CarInfoWidget.h"
#include "AuthorizationsWidget.h"
#include "personalAccountWidget.h"
#include "TapBarBottomWidget.h"
#include "CalendarWidget.h"
//#include "SettingsWidget.h"

bool initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("carwash.db");

   
    if (!db.open()) {
        QMessageBox::critical(nullptr, "Database Error",
            "Cannot open database: " + db.lastError().text());
        return false;
    }

    // Проверяем существование таблиц
    QStringList tables = db.tables();
    if (!tables.contains("washes") || !tables.contains("users")) {
        QSqlQuery query;
        query.exec(R"(
            CREATE TABLE IF NOT EXISTS washes (
                id TEXT PRIMARY KEY,
                user_id INTEGER NOT NULL,
                car_model TEXT NOT NULL,
                car_plate TEXT NOT NULL,
                wash_type INTEGER NOT NULL,
                wash_time DATETIME NOT NULL,
                status INTEGER DEFAULT 0,
                FOREIGN KEY(user_id) REFERENCES users(id)
            )
        )");

        query.exec(R"(
            CREATE TABLE IF NOT EXISTS users (
                id TEXT PRIMARY KEY,
                name TEXT NOT NULL,
                phone TEXT NOT NULL,
                email TEXT NOT NULL,
                password_hash TEXT NOT NULL,
                car_model TEXT,
                car_num TEXT,
                car_color TEXT,
                car_body TEXT
            )
        )");
    }

    return true;
}

void setupMainWindow(QMainWindow& window) {
    // Центральный виджет с навигацией
    QStackedWidget* stackedWidget = new QStackedWidget;

    // Инициализация модулей
    AuthorizationsWidget* authorizationsWidget = new AuthorizationsWidget;
    WeatherWidget* weatherWidget = new WeatherWidget;
    CarInfoWidget* carInfoWidget = new CarInfoWidget;
    PersonalAccountWidget* personalAccountWidget = new PersonalAccountWidget;
    TapBarBottomWidget* tapBarBottomWidget = new TapBarBottomWidget;
    CalendarWidget* calendarWidget = new CalendarWidget;

    // Создаем виджет для объединения weather и account
    QWidget* weatherAndAccountWidget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout(weatherAndAccountWidget);

    // Настраиваем отступы и расстояние между виджетами
    mainLayout->setSpacing(10); // Расстояние между виджетами
    mainLayout->setContentsMargins(5, 5, 5, 5); // Отступы от краев

    // Добавляем виджеты
    mainLayout->addWidget(weatherWidget);
    mainLayout->addWidget(personalAccountWidget);
    mainLayout->addWidget(tapBarBottomWidget);

    // Добавляем все виджеты в стек
    stackedWidget->addWidget(authorizationsWidget);     // Индекс 0
    stackedWidget->addWidget(weatherAndAccountWidget);  // Индекс 1 (теперь содержит оба виджета)
    stackedWidget->addWidget(carInfoWidget);            // Индекс 2
    stackedWidget->addWidget(calendarWidget);           // Индекс 3

    // Установка stretch факторов (опционально)
    //mainLayout->setStretch(0, 1); // WeatherWidget будет занимать 1 часть
    //mainLayout->setStretch(1, 2); // PersonalAccountWidget - 2 части

    QObject::connect(authorizationsWidget, &AuthorizationsWidget::registrationRequested, [stackedWidget]() {
        stackedWidget->setCurrentIndex(2);  // Переход на CarInfoWidget
        });

    QObject::connect(authorizationsWidget, &AuthorizationsWidget::authorizationsSuccess, [stackedWidget]() {
        stackedWidget->setCurrentIndex(1);  // Переход на main
        });

    QObject::connect(authorizationsWidget, &AuthorizationsWidget::getUserId, personalAccountWidget, &PersonalAccountWidget::loadUserData);

    // Настройка главного окна

    window.setCentralWidget(stackedWidget);
    window.setWindowTitle("Автомойка 'Чистый путь'");
    window.setWindowIcon(QIcon(":/image/main_icon.png"));
    window.resize(350, 400);
    stackedWidget->setStyleSheet(
        "   background-color: #333333;"
        "   color: #f0f0f0;"
        "   font-size: 12px;"
        "   font-family: 'Roboto';"
    );

   

    // Статус бар с текущей датой
    QStatusBar* statusBar = window.statusBar();
    statusBar->showMessage(QDate::currentDate().toString("dd.MM.yyyy"));

    // Загружаем настройки
    QSettings settings;
    int lastPage = settings.value("ui/lastPage", 0).toInt();
    stackedWidget->setCurrentIndex(lastPage);
}

//void setupMainWindow(QMainWindow& window) {
//    // Центральный виджет с навигацией
//    QWidget* centralWidget = new QWidget;
//    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
//
//    QStackedWidget* stackedWidget = new QStackedWidget;
//
//    // Панель навигации
//    QHBoxLayout* navLayout = new QHBoxLayout;
//    QPushButton* prevButton = new QPushButton("Назад");
//    QPushButton* nextButton = new QPushButton("Вперед");
//    navLayout->addWidget(prevButton);
//    navLayout->addWidget(nextButton);
//
//    // Инициализация модулей
//    WeatherWidget* weatherWidget = new WeatherWidget;
//    CarInfoWidget* carInfoWidget = new CarInfoWidget;
//
//    stackedWidget->addWidget(weatherWidget);
//    stackedWidget->addWidget(carInfoWidget);
//
//    // Компоновка
//    mainLayout->addLayout(navLayout);
//    mainLayout->addWidget(stackedWidget);
//
//    // Подключение кнопок
//    QObject::connect(prevButton, &QPushButton::clicked, [stackedWidget]() {
//        int current = stackedWidget->currentIndex();
//        if (current > 0) stackedWidget->setCurrentIndex(current - 1);
//    });
//
//    QObject::connect(nextButton, &QPushButton::clicked, [stackedWidget]() {
//        int current = stackedWidget->currentIndex();
//        if (current < stackedWidget->count() - 1) stackedWidget->setCurrentIndex(current + 1);
//    });
//
//    window.setCentralWidget(centralWidget);
//    window.resize(800, 600);
//}

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);

    // Настройка организации для QSettings
    QCoreApplication::setOrganizationName("CleanPath");
    QCoreApplication::setApplicationName("CarWashApp");

    // Инициализация базы данных
    if (!initializeDatabase()) {
        return 1;
    }

    // Главное окно
    QMainWindow mainWindow;
    setupMainWindow(mainWindow);
    
    // Проверка авторизации (упрощенная версия)
    QSettings settings;
    if (!settings.contains("user/id")) {
        QMessageBox::information(&mainWindow, "Добро пожаловать",
            "Пожалуйста, заполните данные профиля");
        // Перенаправляем сразу в настройки
        qobject_cast<QStackedWidget*>(mainWindow.centralWidget())->setCurrentIndex(0);
    }

    mainWindow.show();
    return app.exec();
}