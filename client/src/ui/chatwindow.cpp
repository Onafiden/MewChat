#include "chatwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QTime>
#include <QJsonDocument>
#include <QJsonObject>

MewChat::MewChat(QWidget *parent) 
    : QWidget(parent), m_webSocket(new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this)), m_currentRoomId(1) {
    
    m_stackedWidget = new QStackedWidget(this);

    createLoginScreen();
    createChatScreen();

    m_stackedWidget->addWidget(m_loginPage);
    m_stackedWidget->addWidget(m_chatPage);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_stackedWidget);

    m_stackedWidget->setCurrentIndex(0);


    connect(m_webSocket, &QWebSocket::connected, this, &MewChat::onWebSocketConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &MewChat::onWebSocketDisconnected);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &MewChat::onMessageReceived);
}

void MewChat::createLoginScreen() {
    m_loginPage = new QWidget(this);
    QVBoxLayout *authLayout = new QVBoxLayout(m_loginPage);
    authLayout->setAlignment(Qt::AlignCenter);
    authLayout->setSpacing(15);

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
    m_passwordInput->setObjectName("authInput");
    m_passwordInput->setFixedWidth(320);
    m_passwordInput->setEchoMode(QLineEdit::Password);
    m_passwordInput->setPlaceholderText("Введите ваш пароль...");

    m_loginButton = new QPushButton("Войти в аккаунт", m_loginPage);
    m_loginButton->setObjectName("authButton");
    m_loginButton->setFixedWidth(320);

    authLayout->addWidget(m_titleIcon, 0, Qt::AlignCenter);
    authLayout->addWidget(m_loginInput, 0, Qt::AlignCenter);
    authLayout->addWidget(m_passwordInput, 0, Qt::AlignCenter);
    authLayout->addWidget(m_loginButton, 0, Qt::AlignCenter);

    connect(m_loginButton, &QPushButton::clicked, this, &MewChat::handleLogin);
    connect(m_loginInput, &QLineEdit::returnPressed, this, &MewChat::handleLogin);
}

void MewChat::handleLogin() {
    m_username = m_loginInput->text().trimmed();
    if (m_username.isEmpty()) {
        m_username = "Анонимный Кот";
    }

    m_stackedWidget->setCurrentIndex(1);
    

    connectToRoom(1); 
}

void MewChat::createChatScreen() {
    m_chatPage = new QWidget(this);
    QHBoxLayout *chatMainLayout = new QHBoxLayout(m_chatPage);


    m_roomsList = new QListWidget(m_chatPage);
    m_roomsList->setFixedWidth(200);
    m_roomsList->setObjectName("roomsList");

    QListWidgetItem *generalRoom = new QListWidgetItem("Общий чат", m_roomsList);
    generalRoom->setData(Qt::UserRole, 1);

    QListWidgetItem *floodRoom = new QListWidgetItem("Комната флуда", m_roomsList);
    floodRoom->setData(Qt::UserRole, 2);

    m_roomsList->addItem(generalRoom);
    m_roomsList->addItem(floodRoom);
    m_roomsList->setCurrentItem(generalRoom);


    connect(m_roomsList, &QListWidget::currentItemChanged, this, &MewChat::onRoomChanged);

    QWidget *rightWidget = new QWidget(m_chatPage);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    m_chatHistory = new QListWidget(m_chatPage);
    m_chatHistory->setObjectName("chatHistory");
    m_chatHistory->setSelectionMode(QAbstractItemView::NoSelection);
    m_chatHistory->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

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

    auto sendMessage = [this]() {
        QString text = m_messageInput->text().trimmed();
        if (!text.isEmpty() && m_webSocket->isValid()) {
            QJsonObject msgObj;
            msgObj["sender"] = m_username;
            msgObj["text"] = text;
            msgObj["room_id"] = m_currentRoomId;

            QJsonDocument doc(msgObj);
            m_webSocket->sendTextMessage(doc.toJson(QJsonDocument::Compact));
            
            m_messageInput->clear();
        }
    };

    connect(m_sendButton, &QPushButton::clicked, sendMessage);
    connect(m_messageInput, &QLineEdit::returnPressed, sendMessage);
}

void MewChat::onRoomChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    Q_UNUSED(previous);
    if (!current) return;

    int roomId = current->data(Qt::UserRole).toInt();
    connectToRoom(roomId);
}

void MewChat::connectToRoom(int roomId) {
    m_currentRoomId = roomId;
    
    if (m_webSocket->isValid()) {
        m_webSocket->close();
    }

    m_chatHistory->clear();

    QString urlStr = QString("ws://192.168.100.5:8080/ws?room_id=%1").arg(m_currentRoomId);
    m_webSocket->open(QUrl(urlStr));
}

void MewChat::onWebSocketConnected() {
    appendMessage("Система", QString("Успешно подключено к комнате №%1").arg(m_currentRoomId), false);
}

void MewChat::onWebSocketDisconnected() {
    appendMessage("Система", "Соединение разорвано сервером (Доступ запрещен или комната не существует).", false);
}


void MewChat::onMessageReceived(const QString &message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isNull() && doc.isObject()) {
        QJsonObject obj = doc.object();
        QString sender = obj["sender"].toString();
        QString text = obj["text"].toString();

        bool isMe = (sender == m_username);
        appendMessage(sender, text, isMe);
    }
}


void MewChat::appendMessage(const QString &sender, const QString &text, bool isMe) {
    QWidget *container = new QWidget(m_chatHistory);
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(10, 5, 10, 5);

    QFrame *bubble = new QFrame(container);
    QVBoxLayout *bubbleLayout = new QVBoxLayout(bubble);
    bubbleLayout->setContentsMargins(12, 8, 12, 8);
    bubbleLayout->setSpacing(4);

    QLabel *senderLabel = new QLabel(sender, bubble);
    senderLabel->setObjectName("msgSender");

    QLabel *textLabel = new QLabel(text, bubble);
    textLabel->setObjectName("msgText");
    textLabel->setWordWrap(true);

    QString currentTime = QTime::currentTime().toString("HH:mm");
    QLabel *timeLabel = new QLabel(currentTime, bubble);
    timeLabel->setObjectName("msgTime");
    timeLabel->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    bubbleLayout->addWidget(senderLabel);
    bubbleLayout->addWidget(textLabel);
    bubbleLayout->addWidget(timeLabel);

    if (isMe) {
        bubble->setObjectName("outgoingBubble");
        senderLabel->hide();
        layout->addStretch();
        layout->addWidget(bubble);
    } else {
        bubble->setObjectName("incomingBubble");
        if (sender == "Система") {
            senderLabel->setStyleSheet("color: #e74c3c;");
        }
        layout->addWidget(bubble);
        layout->addStretch();
    }

    bubble->setMaximumWidth(450);

    QListWidgetItem *item = new QListWidgetItem(m_chatHistory);
    item->setSizeHint(container->sizeHint());
    
    m_chatHistory->addItem(item);
    m_chatHistory->setItemWidget(item, container);
    m_chatHistory->scrollToBottom();
}