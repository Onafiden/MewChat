#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>



class MewChat : public QWidget{
    Q_OBJECT

public:
    explicit MewChat(QWidget *parent = nullptr);
    ~MewChat() = default;


private:

    QLineEdit      *m_loginInput;
    QLineEdit      *m_passwordInput;
    QPushButton    *m_loginButton;
    QStackedWidget *m_screenLayers;
    QLabel         *m_titleLabel;

};