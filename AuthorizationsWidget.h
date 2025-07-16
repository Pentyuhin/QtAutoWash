#ifndef AUTHORIZATIONSWIDGET_H
#define AUTHORIZATIONSWIDGET_H

#include <QWidget>
#include <QLabel> 
#include <QLineEdit>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlError>

#include "CarInfoWidget.h"
#include "personalAccountWidget.h"

class AuthorizationsWidget : public QWidget {

	Q_OBJECT

public: 

	explicit AuthorizationsWidget(QWidget* parent = nullptr);


private:

	CarInfoWidget* m_carInfoWidget;
	PersonalAccountWidget* m_persAcWidget;

	void setupUI();
	/*bool authenticateUser(const QString &login, const QString &password);*/
	//void loadSaveData();
	//void saveUserData();
	void onOkClicked();
	bool authenticateUser(const QString& phone, const QString& password);

signals:
	void registrationRequested(); // Сигнал для запроса регистрации
	void authorizationsSuccess();
	void getUserId(const QString user_id);
	

protected:

	QLabel* m_titleWindow;
	QLineEdit* m_userName;
	QLineEdit* m_userPassword;
	QPushButton* m_enterAppBtn;
	QPushButton* m_exitAppBtn;
	QPushButton* m_registrationBtn;

	

};



#endif // AUTHORIZATIONSWIDGET_H

