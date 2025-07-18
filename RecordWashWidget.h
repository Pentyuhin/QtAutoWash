#ifndef RECORDWASHWIDGET_H
#define RECORDWASHWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QList>
#include <QString>
#include <QVector>
#include <QDate>
#include <QTime>




class RecordWash : public QWidget {

	Q_OBJECT

public:

	explicit RecordWash(QWidget * parent = nullptr);
	
	~RecordWash();
	
	void printTimeSlots();
	void printDateSlots();



	//void getDates();	// Получение даты
	//void getTimes();	// Получение времени 
	//void getServices();	// Получение услуги

private:

	// Основной метод настройки интерфейса
	void setupUI();

	// Методы создания виджетов
	QWidget* createRatesWidget(QWidget* parent);
	QWidget* createServicesWidget(QWidget* parent);

	// Вспомогательные методы создания элементов
	QLabel* createLabel(const QString& text);
	QCheckBox* createCheckBox(const QString& text, QWidget* parent); // Добавлено объявление

	// Метод для выравнивания ширины виджетов
	void alignWidgetsWidth(const QList<QWidget*>& widgets);

	// Члены класса для хранения UI-элементов (при необходимости)
	QButtonGroup* m_ratesGroup;

	//void setDates(QDate& date);	// Установка даты
	//void setTimes(QTime& time);	// Установка времени 
	//void setServices(QString services);	// Установка услуги

private:

	QList<QDate>* m_dates;
	QList<QTime>* m_times;

	QVector<QString>* m_listServices;

};



#endif // RECORDWASHWIDGET_H