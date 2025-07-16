#ifndef CALENDARWIDGET_H
#define	CALENDARWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
class CalendarWidget : public QWidget {
	Q_OBJECT

public:
	explicit CalendarWidget(QWidget* parent = nullptr);


	void setupUI();

	QVBoxLayout* containerTimeUI();

private:





};


#endif CALENDARWIDGET_H
