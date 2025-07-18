#ifndef TAPBARBOTTOMWIDGET_H
#define TAPBARBOTTOMWIDGET_H

#include <QWidget>
#include <QLabel> 
#include <QString>
#include <QPushButton>
#include <QStackedWidget>

class TapBarBottomWidget : public QWidget {
	Q_OBJECT

public:

	explicit TapBarBottomWidget(QWidget* parent = nullptr);

	void setupUI();
	void setupButton(QPushButton* button, const QString& iconPath);


private slots:
	void handleButtonClick();

signals:
	void buttonClicked(int index);

private:

	QPushButton* m_settingsBtn;
	QPushButton* m_personalAccountBtn;
	QPushButton* m_mapsBtn;
	QPushButton* m_recordBtn;

	// Храним соответствие кнопок и индексов
	QHash<QPushButton*, int> m_buttonIndices;


};





#endif // TAPBARBOTTOMWIDGET_H
