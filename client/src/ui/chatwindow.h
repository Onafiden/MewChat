#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QSoundEffect>
#include <QListWidget>
#include <QTextEdit>

class MewChat : public QWidget {
    Q_OBJECT

public:
    explicit MewChat(QWidget *parent = nullptr);
    ~MewChat() = default;

private:
    void createLoginScreen();
    void createChatScreen();
    
    void appendMessage(const QString &sender, const QString &text, bool isMe);

    QStackedWidget *m_stackedWidget;

    QWidget *m_loginPage;
    QWidget *m_chatPage;

    QLineEdit      *m_loginInput;
    QLineEdit      *m_passwordInput;
    QPushButton    *m_loginButton;
    QPushButton    *m_titleIcon;

    QListWidget    *m_roomsList;
    
    QListWidget    *m_chatHistory; 
    
    QLineEdit      *m_messageInput;
    QPushButton    *m_sendButton;
};