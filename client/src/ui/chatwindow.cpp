#include "chatwindow.h"
#include <QVBoxLayout>


MewChat::MewChat(QWidget *parent) : QWidget(parent){

    m_titleLabel = new QLabel("Welcome to MewChat", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_loginInput = new QLineEdit(this);
    m_loginInput->setPlaceholderText("Введите ваш никнейм...");

    m_passwordInput = new QLineEdit(this);
    m_passwordInput->setPlaceholderText("Введите пароль...");
    m_passwordInput->setEchoMode(QLineEdit::Password);

    m_loginButton = new QPushButton("Войти", this);




    QVBoxLayout *authLayout = new QVBoxLayout(this);

    authLayout->addWidget(m_titleLabel);
    authLayout->addWidget(m_loginInput);
    authLayout->addWidget(m_passwordInput);
    authLayout->addWidget(m_loginButton);

    authLayout->setContentsMargins(50, 50, 50, 50);
    authLayout->setSpacing(15);

    setLayout(authLayout);



    connect(m_loginButton, &QPushButton::clicked, [this]() {
        QString username = m_loginInput->text().trimmed();
        QString password = m_passwordInput->text();
    

        if (username.isEmpty()) {
            m_titleLabel->setText("Ошибка: Никнейм не может быть пустым!");
            m_titleLabel->setStyleSheet("color: #bf616a;");
            return;
        }
        
    });


}