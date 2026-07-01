#include "chatwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QTime>

MewChat::MewChat(QWidget *parent) : QWidget(parent) {
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
    m_chatPage = new QWidget(this);
    QHBoxLayout *chatMainLayout = new QHBoxLayout(m_chatPage);

    // Список комнат
    m_roomsList = new QListWidget(m_chatPage);
    m_roomsList->setFixedWidth(200);
    m_roomsList->setObjectName("roomsList");
    m_roomsList->addItem("Общий чат");
    m_roomsList->addItem("Комната флуда");

    QWidget *rightWidget = new QWidget(m_chatPage);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    // Инициализируем историю как QListWidget
    m_chatHistory = new QListWidget(m_chatPage);
    m_chatHistory->setObjectName("chatHistory");
    m_chatHistory->setSelectionMode(QAbstractItemView::NoSelection); // Отключаем выделение элементов строк
    m_chatHistory->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel); // Плавный скролл

    // Поле ввода и кнопка
    QHBoxLayout *inputLayout = new QHBoxLayout();
    m_messageInput = new QLineEdit(m_chatPage);
    m_messageInput->setObjectName("messageInput");
    m_messageInput->setPlaceholderText("Напишите сообщение...");
    
    m_sendButton = new QPushButton("Отправить", m_chatPage);
    m_sendButton->setObjectName("sendButton");

    inputLayout->addWidget(m_messageInput);
    inputLayout->addWidget(m_sendButton);

    rightLayout->addWidget(m_chatHistory);
    rightLayout->addLayout(inputLayout);

    chatMainLayout->addWidget(m_roomsList);
    chatMainLayout->addWidget(rightWidget);

    // Лямбда отправки сообщения
    auto sendMessage = [this]() {
        QString text = m_messageInput->text().trimmed();
        if (!text.isEmpty()) {
            // Вызываем наш новый метод рендеринга сообщений (isMe = true)
            appendMessage("Вы", text, true); 
            m_messageInput->clear();
        }
    };

    connect(m_sendButton, &QPushButton::clicked, sendMessage);
    connect(m_messageInput, &QLineEdit::returnPressed, sendMessage);
}

// Новый метод для отрисовки красивых тем сообщений
void MewChat::appendMessage(const QString &sender, const QString &text, bool isMe) {
    // 1. Создаем базовый виджет-контейнер для одной строки списка
    QWidget *container = new QWidget(m_chatHistory);
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(10, 5, 10, 5);

    // 2. Создаем "пузырь" сообщения
    QFrame *bubble = new QFrame(container);
    QVBoxLayout *bubbleLayout = new QVBoxLayout(bubble);
    bubbleLayout->setContentsMargins(12, 8, 12, 8);
    bubbleLayout->setSpacing(4);

    // Имя отправителя
    QLabel *senderLabel = new QLabel(sender, bubble);
    senderLabel->setObjectName("msgSender");

    // Текст сообщения
    QLabel *textLabel = new QLabel(text, bubble);
    textLabel->setObjectName("msgText");
    textLabel->setWordWrap(true); // Перенос длинных строк

    // Время отправки
    QString currentTime = QTime::currentTime().toString("HH:mm");
    QLabel *timeLabel = new QLabel(currentTime, bubble);
    timeLabel->setObjectName("msgTime");
    timeLabel->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    bubbleLayout->addWidget(senderLabel);
    bubbleLayout->addWidget(textLabel);
    bubbleLayout->addWidget(timeLabel);

    // 3. Стилизация в зависимости от того, кто отправил (Вы или собеседник)
    if (isMe) {
        bubble->setObjectName("outgoingBubble");
        senderLabel->hide(); // Себе имя можно не показывать
        
        layout->addStretch();  // Пружина слева сдвигает облачко ВПРАВО
        layout->addWidget(bubble);
    } else {
        bubble->setObjectName("incomingBubble");
        
        layout->addWidget(bubble);
        layout->addStretch();  // Пружина справа сдвигает облачко ВЛЕВО
    }

    // Ограничиваем максимальную ширину пузыря, чтобы он не растягивался на весь экран
    bubble->setMaximumWidth(450);

    // 4. Заталкиваем кастомный виджет в QListWidget
    QListWidgetItem *item = new QListWidgetItem(m_chatHistory);
    // Говорим элементу списка принять размер нашего кастомного виджета
    item->setSizeHint(container->sizeHint()); 
    
    m_chatHistory->addItem(item);
    m_chatHistory->setItemWidget(item, container);
    
    // Автоматический скролл вниз к новому сообщению
    m_chatHistory->scrollToBottom();
}