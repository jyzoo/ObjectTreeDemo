#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPointer>
#include <QVBoxLayout>
#include <QWidget>

#include "widgets.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void moveMovableWidget();
    void deletePanel();
    void demoDetaching();

private:
    void setupUi();
    void setStatus(const QString &text);

    // Основной layout и layout панели
    QPointer<QVBoxLayout> m_rootLayout;
    QPointer<QVBoxLayout> m_panelLayout;

    // Виджеты главного окна
    QPointer<LogLabel> m_title;
    QPointer<LogLineEdit> m_input;
    QPointer<LogLabel> m_status;

    // Панель и перемещаемая кнопка
    QPointer<LogPanel> m_panel;
    QPointer<LogButton> m_movable;

    // Кнопки управления
    QPointer<LogButton> m_moveButton;
    QPointer<LogButton> m_deleteButton;
};

#endif // MAINWINDOW_H