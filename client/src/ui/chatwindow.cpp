#include "chatwindow.h"
#include <QVBoxLayout>


MewChat::MewChat(QWidget *parent) : QWidget(parent){



    m_titleIcon = new QPushButton(this);

    m_titleIcon->setIcon(QIcon(":/img/mainlogo.png"));
    m_titleIcon->setObjectName("mainIcon");
    m_titleIcon->setIconSize(QSize(256, 256));
    m_titleIcon->setFixedSize(QSize(256, 256));


    m_loginInput = new QLineEdit(this);
    m_loginInput->setPlaceholderText("Введите ваш никнейм...");

    m_passwordInput = new QLineEdit(this);
    m_passwordInput->setPlaceholderText("Введите пароль...");
    m_passwordInput->setEchoMode(QLineEdit::Password);

    m_loginButton = new QPushButton("Войти", this);


    QLabel *errorLabel = new QLabel("Неверный логин или пароль");
    errorLabel->setObjectName("authErrorLabel");
    errorLabel->hide();



    QSoundEffect *mewSound = new QSoundEffect(this);
    mewSound->setSource(QUrl("qrc:/sound/mew.wav"));
    mewSound->setVolume(0.75);



    QVBoxLayout *authLayout = new QVBoxLayout(this);


    authLayout->addStretch();

    authLayout->addWidget(m_titleIcon,0, Qt::AlignCenter);
    authLayout->addWidget(m_loginInput);
    authLayout->addWidget(m_passwordInput);
    authLayout->addWidget(errorLabel);
    authLayout->addWidget(m_loginButton);

    authLayout->addStretch();



    authLayout->setContentsMargins(50, 50, 50, 50);
    authLayout->setSpacing(15);

    setLayout(authLayout);

    connect(m_titleIcon, &QPushButton::clicked, [this, mewSound](){
        mewSound->play();
    });

    connect(m_loginButton, &QPushButton::clicked, [this, errorLabel]() {
        QString username = m_loginInput->text().trimmed();
        QString password = m_passwordInput->text();
    

        if (username.isEmpty()) {
            errorLabel->show();
        }
        
    });


}