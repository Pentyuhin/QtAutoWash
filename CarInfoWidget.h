#ifndef CARINFOWIDGET_H
#define CARINFOWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QRegularExpressionValidator>
#include <QPushButton> 
#include <QSqlDatabase>
#include <QSqlQuery> 


class CarInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CarInfoWidget(QWidget* parent = nullptr);

    QString carModel() const;
    QString licensePlate() const;
    QString carColor() const;
    QString userName() const;
    QString userPhone() const;
    QString userEmail() const;
    bool isValid() const;
    void clear();

    QString hashPasswordPbkdf2(const QString& password);

signals:
    void dataChanged();

private slots:
    void validateInput();
    void saveUserData();
   
protected:
    

private:
    void setupUI();
    void setupValidators();
    void updateCarImage(int index);

    void showImageError(const QString& path);

    QLineEdit* m_modelEdit;
    QLineEdit* m_plateEdit;
    QLineEdit* m_nameEdit;
    QLineEdit* m_emailEdit;
    QLineEdit* m_phoneEdit;
    QLineEdit* m_passHashEdit;
    QComboBox* m_colorCombo;
    QComboBox* m_modelCombo;
    QLabel* m_validationIcon;
    QLabel* m_carImageLabel;
    QPushButton* m_cleanBtn;
    QPushButton* m_saveBtn;
};
    

#endif // CARINFOWIDGET_H