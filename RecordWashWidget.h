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
#include <QCheckBox>




class RecordWash : public QWidget {

	Q_OBJECT

public:

	explicit RecordWash(QWidget * parent = nullptr);
	
	~RecordWash();
	
	void printTimeSlots();
	void printDateSlots();

private:

	// Основной метод настройки интерфейса
	void setupUI();

	// Методы создания виджетов
	QWidget* createRatesWidget(QWidget* parent);
	QWidget* createServicesWidget(QWidget* parent);
	QWidget* createDateWidget(QWidget* parent);
	QWidget* createTimeWidget(QWidget* parent);

	// Объявление шаблонной функции
	template<typename T>
	QWidget* createButtonGridWidget(
		QWidget* parent,
		const QString& title,
		const QList<T>& items,
		std::function<QString(const T&)> toStringFunc);

	// Вспомогательные методы создания элементов
	QLabel* createLabel(const QString& text);
	QCheckBox* createCheckBox(const QString& text, QWidget* parent);
	QPushButton* createButton(const QString& text);

	// Метод для выравнивания ширины виджетов
	void alignWidgetsWidth(const QList<QWidget*>& widgets);

	// Члены класса для хранения UI-элементов (при необходимости)
	QButtonGroup* m_ratesGroup;


private:

	QList<QDate>* m_dates;
	QList<QTime>* m_times;

	QVector<QString>* m_listServices;

};



#endif // RECORDWASHWIDGET_H