#ifndef PERSONEACCOUNTWIDGET_H
#define PERSONEACCOUNTWIDGET_H


#include <QWidget>
#include <QLabel> 
#include <QLineEdit>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlError>

class PersonalAccountWidget : public QWidget {

	Q_OBJECT

public:

	explicit PersonalAccountWidget(QWidget * parent = nullptr);

public slots:
	void loadUserData(const QString &user_id);
private:

	
	void setupUI();
	void displayUserInfo(const QString &info);

	void updateCarImage(const QString& carName);

	void loadDefaultImage();

	void loadCarImage(const QString &imagePath);

	// UI элементы
	QLabel* m_carImageLabel;
	QLabel* m_userInfoLabel;

	// База данных
	QSqlDatabase m_db;

};


#endif // PERSONEACCOUNTWIDGET_H