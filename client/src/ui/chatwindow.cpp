#include "chatwindow.h"
#include <QVBoxLayout>


MewChat::MewChat(QWidget *parent) : QWidget(parent){



    m_titleIcon = new QPushButton(this);

    m_titleIcon->setIcon(QIcon(":/img/mainlogo.png"));
    m_titleIcon->setObjectName("mainIcon");
    m_titleIcon->setIconSize(QSize(256, 256));
    m_titleIcon->setFixedSize(QSize(256, 256));


    m_loginInput = new QLineEdit(this);
    m_loginInput->setObjectName("authInput");
    m_loginInput->setFixedWidth(320);
    m_loginInput->setPlaceholderText("Введите ваш логин...");

    m_passwordInput = new QLineEdit(this);
    m_passwordInput->setPlaceholderText("Введите пароль...");
    m_passwordInput->setObjectName("authInput");
    m_passwordInput->setFixedWidth(320);
    m_passwordInput->setEchoMode(QLineEdit::Password);

    m_loginButton = new QPushButton("Войти", this);
    m_loginButton->setObjectName("authButton");
    m_loginButton->setFixedWidth(320);

    QLabel *errorLabel = new QLabel("Неверный логин или пароль");
    errorLabel->setObjectName("authErrorLabel");
    errorLabel->hide();



    QSoundEffect *mewSound = new QSoundEffect(this);
    mewSound->setSource(QUrl("qrc:/sound/mew.wav"));
    mewSound->setVolume(0.75);



    QVBoxLayout *authLayout = new QVBoxLayout(this);


    authLayout->addStretch();

    authLayout->addWidget(m_titleIcon,0, Qt::AlignCenter);
    authLayout->addWidget(m_loginInput,0, Qt::AlignCenter);
    authLayout->addWidget(m_passwordInput,0, Qt::AlignCenter);
    authLayout->addWidget(errorLabel,0, Qt::AlignCenter);
    authLayout->addWidget(m_loginButton,0, Qt::AlignCenter);

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


    connect(m_loginInput, &QLineEdit::textChanged, [this, errorLabel]() {
        errorLabel->hide();
    });

    connect(m_passwordInput, &QLineEdit::textChanged, [this, errorLabel]() {
        errorLabel->hide();
    });


}