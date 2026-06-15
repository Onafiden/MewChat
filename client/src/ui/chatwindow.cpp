#include "chatwindow.h"
#include <QVBoxLayout>


MewChat::MewChat(QWidget *parent) : QWidget(parent){

    m_stackedWidget = new QStackedWidget(this);

    createLoginScreen();
    createChatScreen();

    m_stackedWidget->addWidget(m_loginPage);
    m_stackedWidget->addWidget(m_chatPage);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_stackedWidget);

    m_stackedWidget->setCurrentIndex(0);


    


}






void MewChat::createLoginScreen() {

    m_loginPage = new QWidget(this);
    

    QVBoxLayout *authLayout = new QVBoxLayout(m_loginPage);

 
    m_titleIcon = new QPushButton(m_loginPage);
    m_titleIcon->setIcon(QIcon(":/img/mainlogo.png"));
    m_titleIcon->setObjectName("mainIcon");
    m_titleIcon->setIconSize(QSize(256, 256));
    m_titleIcon->setFixedSize(QSize(256, 256));

    m_loginInput = new QLineEdit(m_loginPage);
    m_loginInput->setObjectName("authInput");
    m_loginInput->setFixedWidth(320);
    m_loginInput->setPlaceholderText("Введите ваш логин...");

    m_passwordInput = new QLineEdit(m_loginPage);
    m_passwordInput->setPlaceholderText("Введите пароль...");
    m_passwordInput->setObjectName("authInput");
    m_passwordInput->setFixedWidth(320);
    m_passwordInput->setEchoMode(QLineEdit::Password);

    m_loginButton = new QPushButton("Войти", m_loginPage);
    m_loginButton->setObjectName("authButton");
    m_loginButton->setFixedWidth(320);

    QLabel *errorLabel = new QLabel("Неверный логин или пароль", m_loginPage);
    errorLabel->setObjectName("authErrorLabel");
    errorLabel->hide();


    QSoundEffect *mewSound = new QSoundEffect(this);
    mewSound->setSource(QUrl("qrc:/sound/mew.wav"));
    mewSound->setVolume(0.75);


    authLayout->addStretch();
    authLayout->addWidget(m_titleIcon, 0, Qt::AlignCenter);
    authLayout->addWidget(m_loginInput, 0, Qt::AlignCenter);
    authLayout->addWidget(m_passwordInput, 0, Qt::AlignCenter);
    authLayout->addWidget(errorLabel, 0, Qt::AlignCenter);
    authLayout->addWidget(m_loginButton, 0, Qt::AlignCenter);
    authLayout->addStretch();

    authLayout->setContentsMargins(50, 50, 50, 50);
    authLayout->setSpacing(15);



    connect(m_titleIcon, &QPushButton::clicked, [this, mewSound](){
        mewSound->play();
    });

    connect(m_loginButton, &QPushButton::clicked, [this, errorLabel]() {
        QString username = m_loginInput->text().trimmed();
        QString password = m_passwordInput->text();

        if (username.isEmpty()) {
            errorLabel->show();
        } else {
            m_stackedWidget->setCurrentIndex(1);
        }
    });

    connect(m_loginInput, &QLineEdit::textChanged, [this, errorLabel]() {
        errorLabel->hide();
    });

    connect(m_passwordInput, &QLineEdit::textChanged, [this, errorLabel]() {
        errorLabel->hide();
    });
}









void MewChat::createChatScreen() {
    // 1. Создаем страницу-контейнер для чата
    m_chatPage = new QWidget(this);
    
    // Главная горизонтальная сетка (Сайдбар слева, чат справа)
    QHBoxLayout *chatMainLayout = new QHBoxLayout(m_chatPage);

    // 2. ЛЕВАЯ ПАНЕЛЬ: Список комнат
    m_roomsList = new QListWidget(m_chatPage);
    m_roomsList->setFixedWidth(200); // Ограничиваем ширину сайдбара
    m_roomsList->setObjectName("roomsList");
    m_roomsList->addItem("Общий чат");
    m_roomsList->addItem("Комната флуда");

    // 3. ПРАВАЯ ПАНЕЛЬ: Контейнер для истории и ввода
    QWidget *rightWidget = new QWidget(m_chatPage);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    // Окно истории сообщений
    m_chatHistory = new QTextEdit(m_chatPage);
    m_chatHistory->setObjectName("chatHistory");
    m_chatHistory->setReadOnly(true); // Запрещаем юзеру стирать историю руками

    // Нижняя строка: Поле ввода + Кнопка
    QHBoxLayout *inputLayout = new QHBoxLayout();
    
    m_messageInput = new QLineEdit(m_chatPage);
    m_messageInput->setObjectName("messageInput");
    m_messageInput->setPlaceholderText("Напишите сообщение...");
    
    m_sendButton = new QPushButton("Отправить", m_chatPage);
    m_sendButton->setObjectName("sendButton");

    // Собираем нижнюю строчку ввода
    inputLayout->addWidget(m_messageInput);
    inputLayout->addWidget(m_sendButton);

    // Вертикально складываем историю и ввод в правую панель
    rightLayout->addWidget(m_chatHistory);
    rightLayout->addLayout(inputLayout);

    // Горизонтально объединяем левый сайдбар и правую часть
    chatMainLayout->addWidget(m_roomsList);
    chatMainLayout->addWidget(rightWidget);
}