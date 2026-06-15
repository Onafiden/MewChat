#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QSoundEffect>
#include <QStackedWidget>
#include <QListWidget>
#include <QTextEdit>



class MewChat : public QWidget{
    Q_OBJECT

public:
    explicit MewChat(QWidget *parent = nullptr);
    ~MewChat() = default;


private:

    void createLoginScreen();
    void createChatScreen();

    QStackedWidget *m_stackedWidget;

    QWidget *m_loginPage;
    QWidget *m_chatPage;


    QPushButton    *m_logoButton;
    QLineEdit      *m_loginInput;
    QLineEdit      *m_passwordInput;
    QPushButton    *m_loginButton;
    QStackedWidget *m_screenLayers;
    QPushButton    *m_titleIcon;



    QListWidget *m_roomsList;
    QTextEdit *m_chatHistory;
    QLineEdit *m_messageInput;
    QPushButton *m_sendButton;

};