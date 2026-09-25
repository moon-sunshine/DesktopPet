#include "widget.h"
#include <QApplication>
#include <QPixmap>
#include <QSize>
#include <QDir>
#include <QScreen>
#include <QFile>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // 1. 窗口设置
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    resize(300, 300);

    // 2. 创建显示标签
    petLabel = new QLabel(this);
    petLabel->resize(300, 300);
    petLabel->setAlignment(Qt::AlignCenter);

    // 3. 加载素材（从 assets 文件夹加载）
    QString appDir = QCoreApplication::applicationDirPath();
    QString assetsDir = appDir + "/assets";
    QString gifPath = assetsDir + "/pet.gif";
    QString pngPath = assetsDir + "/pet.png";

    petMovie = new QMovie(gifPath, QByteArray(), this);
    if (petMovie->isValid()) {
        petMovie->setScaledSize(QSize(300, 300));
        petLabel->setMovie(petMovie);
        petMovie->start();
    } else {
        QPixmap pixmap(pngPath);
        if (pixmap.isNull()) {
            petLabel->setText("找不到 pet.gif 或 pet.png\n请把素材放到 exe 同目录");
            petLabel->setStyleSheet("color: red; font-weight: bold; background: transparent;");
        } else {
            petLabel->setPixmap(pixmap.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    petLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    // 4. 初始化系统托盘
    initTray();
}

Widget::~Widget()
{
    if (petMovie->isValid()) petMovie->stop();
}

void Widget::initTray()
{
    // 1. 创建托盘菜单
    trayMenu = new QMenu(this);
    QAction *showAction = trayMenu->addAction("显示/隐藏桌宠");
    QAction *quitAction = trayMenu->addAction("退出");

    // 2. 【关键修复】把菜单动作连接上对应的功能
    connect(showAction, &QAction::triggered, this, [this]() {
        this->setVisible(!this->isVisible());
    });
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    // 3. 从 assets 文件夹加载托盘图标
    QString appDir = QCoreApplication::applicationDirPath();
    QString iconPath = appDir + "/assets/icon.png";

    trayIcon = new QSystemTrayIcon(this);

    if (QFile::exists(iconPath)) {
        trayIcon->setIcon(QIcon(iconPath));
    } else {
        trayIcon->setIcon(QIcon::fromTheme("applications-system"));
    }

    trayIcon->setToolTip("桌面宠物");
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    // 4. 左键点击托盘图标切换显示/隐藏
    connect(trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            this->setVisible(!this->isVisible());
        }
    });
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint newPos = event->globalPosition().toPoint() - dragPosition;
        QScreen *screen = QApplication::screenAt(event->globalPosition().toPoint());
        if (screen) {
            QRect screenRect = screen->availableGeometry();
            int x = qBound(screenRect.left() - width() + 50, newPos.x(), screenRect.right() - 50);
            int y = qBound(screenRect.top() - height() + 50, newPos.y(), screenRect.bottom() - 50);
            move(x, y);
        } else {
            move(newPos);
        }
        event->accept();
    }
}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    QAction *quitAction = menu.addAction("退出桌宠");
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    menu.exec(event->globalPos());
}

void Widget::moveEvent(QMoveEvent *event)
{
    QWidget::moveEvent(event);
}