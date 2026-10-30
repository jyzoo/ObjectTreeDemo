#ifndef WIDGETS_H
#define WIDGETS_H

#include <QDebug>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

// Метка с отслеживанием создания и уничтожения
class LogLabel : public QLabel
{
public:
    explicit LogLabel(const QString &text, QWidget *parent = nullptr)
        : QLabel(text, parent)
    {
        qDebug() << "[CREATE] LogLabel" << text;
    }

    ~LogLabel() override
    {
        qDebug() << "[DESTROY] LogLabel" << text();
    }
};

// Кнопка с отслеживанием создания и уничтожения
class LogButton : public QPushButton
{
public:
    explicit LogButton(const QString &text, QWidget *parent = nullptr)
        : QPushButton(text, parent)
    {
        qDebug() << "[CREATE] LogButton" << text;
    }

    ~LogButton() override
    {
        qDebug() << "[DESTROY] LogButton" << text();
    }
};

// Поле ввода с отслеживанием создания и уничтожения
class LogLineEdit : public QLineEdit
{
public:
    explicit LogLineEdit(QWidget *parent = nullptr)
        : QLineEdit(parent)
    {
        qDebug() << "[CREATE] LogLineEdit";
    }

    ~LogLineEdit() override
    {
        qDebug() << "[DESTROY] LogLineEdit";
    }
};

// Панель с отслеживанием создания и уничтожения
class LogPanel : public QFrame
{
public:
    explicit LogPanel(const QString &title, QWidget *parent = nullptr)
        : QFrame(parent)
    {
        setObjectName(title);
        setFrameShape(QFrame::StyledPanel);

        qDebug() << "[CREATE] LogPanel" << title;
    }

    ~LogPanel() override
    {
        qDebug() << "[DESTROY] LogPanel" << objectName();
    }
};

#endif // WIDGETS_H