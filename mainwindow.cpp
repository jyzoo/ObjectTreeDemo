#include "mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

MainWindow::~MainWindow()
{
    qDebug() << "[DESTROY] MainWindow";
}

// Создание интерфейса
void MainWindow::setupUi()
{
    setObjectName("MainWindow");
    setWindowTitle("Qt Object Tree Demo");
    resize(520, 500);

    // Главный layout
    m_rootLayout = new QVBoxLayout(this);

    // Заголовок
    m_title = new LogLabel(
        "Дерево объектов Qt: владелец и потомки", this);

    m_rootLayout->addWidget(m_title.data());

    // Поле ввода
    m_input = new LogLineEdit(this);
    m_input->setPlaceholderText(
        "Виджет-ребёнок главного окна");

    m_rootLayout->addWidget(m_input.data());

    // Статус
    m_status = new LogLabel(
        "Статус: окно создано", this);

    m_status->setWordWrap(true);
    m_rootLayout->addWidget(m_status.data());

    // Панель-владелец
    m_panel = new LogPanel("Панель-владелец", this);
    m_rootLayout->addWidget(m_panel.data());

    // Layout панели
    m_panelLayout = new QVBoxLayout(m_panel.data());

    // Обычная кнопка внутри панели
    auto *staticButton = new LogButton(
        "Статическая кнопка внутри панели",
        m_panel.data());

    m_panelLayout->addWidget(staticButton);

    // Кнопка, которую можно перемещать
    m_movable = new LogButton(
        "Перемещаемый виджет (в панели)",
        m_panel.data());

    m_panelLayout->addWidget(m_movable.data());

    // Кнопка перемещения виджета
    m_moveButton = new LogButton(
        "Переместить виджет: панель <-> окно",
        this);

    connect(m_moveButton.data(),
            &QPushButton::clicked,
            this,
            &MainWindow::moveMovableWidget);

    m_rootLayout->addWidget(m_moveButton.data());

    // Кнопка удаления панели
    m_deleteButton = new LogButton(
        "Удалить панель (deleteLater)",
        this);

    connect(m_deleteButton.data(),
            &QPushButton::clicked,
            this,
            &MainWindow::deletePanel);

    m_rootLayout->addWidget(m_deleteButton.data());

    // Кнопка демонстрации setParent(nullptr)
    auto *detachButton = new LogButton(
        "Демо setParent(nullptr)",
        this);

    connect(detachButton,
            &QPushButton::clicked,
            this,
            &MainWindow::demoDetaching);

    m_rootLayout->addWidget(detachButton);

    m_rootLayout->addStretch();

    setStatus(
        "Готово. Все виджеты созданы с родителями. "
        "Удаление родителя автоматически удалит детей.");
}

// Изменение текста статуса
void MainWindow::setStatus(const QString &text)
{
    if (!m_status.isNull()) {
        m_status->setText(text);
    }
}

// Перемещение виджета между родителями
void MainWindow::moveMovableWidget()
{
    if (m_movable.isNull()) {
        setStatus("Перемещаемый виджет уже уничтожен.");
        return;
    }

    if (m_panel.isNull()) {
        setStatus(
            "Панель уже удалена. Вернуть виджет в неё невозможно.");
        return;
    }

    // Если кнопка находится внутри панели
    if (m_movable->parentWidget() == m_panel.data()) {

        // Удаляем кнопку из layout панели
        if (!m_panelLayout.isNull()) {
            m_panelLayout->removeWidget(m_movable.data());
        }

        // Меняем родителя на главное окно
        m_movable->setParent(this);

        // Добавляем кнопку в главный layout
        if (!m_rootLayout.isNull()) {
            m_rootLayout->addWidget(m_movable.data());
        }

        m_movable->setText(
            "Перемещаемый виджет (вне панели)");

        setStatus(
            "setParent(this) сделал главным окном новым "
            "владельцем. Панель больше не удалит этот "
            "виджет автоматически.");
    }
    else {
        // Убираем кнопку из главного layout
        if (!m_rootLayout.isNull()) {
            m_rootLayout->removeWidget(m_movable.data());
        }

        // Возвращаем кнопку в панель
        m_movable->setParent(m_panel.data());

        if (!m_panelLayout.isNull()) {
            m_panelLayout->addWidget(m_movable.data());
        }

        m_movable->setText(
            "Перемещаемый виджет (в панели)");

        setStatus(
            "Виджет снова стал ребёнком панели. "
            "Теперь он будет удалён вместе с панелью.");
    }

    m_movable->show();

    qDebug() << "[SET_PARENT] Новый владелец виджета:"
             << m_movable->parentWidget()->objectName();
}

// Удаление панели
void MainWindow::deletePanel()
{
    if (m_panel.isNull()) {
        setStatus("Панель уже удалена.");
        return;
    }

    if (!m_movable.isNull()
        && m_movable->parentWidget() == m_panel.data()) {

        setStatus(
            "Панель будет удалена. Перемещаемый виджет "
            "является её ребёнком, поэтому будет "
            "уничтожен автоматически.");
    }
    else {
        setStatus(
            "Панель будет удалена. Перемещаемый виджет "
            "не является её ребёнком, поэтому останется жив.");
    }

    // Отключаем кнопки, связанные с панелью
    if (!m_deleteButton.isNull()) {
        m_deleteButton->setEnabled(false);
    }

    if (!m_moveButton.isNull()) {
        m_moveButton->setEnabled(false);
    }

    qDebug() << "[DELETE_LATER] Запрошено удаление панели:"
             << m_panel->objectName();

    // Удаление после завершения текущей обработки событий
    m_panel->deleteLater();
}

// Демонстрация setParent(nullptr)
void MainWindow::demoDetaching()
{
    // Создаём временного родителя
    auto *tempOwner = new LogPanel("TempOwner", this);
    tempOwner->hide();

    // Создаём обычного ребёнка
    new LogLabel("Ребёнок TempOwner", tempOwner);

    // Создаём ребёнка, которого затем отвяжем
    auto *detachedChild = new LogLabel(
        "Отвязанный ребёнок TempOwner",
        tempOwner);

    qDebug() << "[DEMO] Количество детей tempOwner до detach:"
             << tempOwner->children().count();

    // Отвязываем второй объект от родителя
    detachedChild->setParent(nullptr);

    qDebug() << "[DEMO] Количество детей tempOwner после detach:"
             << tempOwner->children().count();

    setStatus(
        "Демо: одна метка осталась ребёнком TempOwner, "
        "другая отвязана через setParent(nullptr). "
        "При удалении TempOwner автоматически удалится "
        "только первая.");

    // Запрашиваем удаление обоих объектов
    tempOwner->deleteLater();
    detachedChild->deleteLater();
}