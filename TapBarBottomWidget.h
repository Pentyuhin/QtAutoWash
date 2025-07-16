#ifndef TAPBARBOTTOMWIDGET_H
#define TAPBARBOTTOMWIDGET_H

#include <QWidget>
#include <QLabel> 
#include <QString>
#include <QPushButton>


class TapBarBottomWidget : public QWidget {
	Q_OBJECT

public:

	explicit TapBarBottomWidget(QWidget* parent = nullptr);

	void setupUI();
	void setupButton(QPushButton* button, const QString& iconPath);

private:

	/*void setupButton(QPushButton* button, const QString &iconPath);*/

	QPushButton* m_settingsBtn;
	QPushButton* m_personalAccountBtn;
	QPushButton* m_mapsBtn;
	QPushButton* m_recordBtn;


};





#endif // TAPBARBOTTOMWIDGET_H
