#include "widget.h"
#include <QApplication>
#include <QPixmap>
#include <QSize>

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

    // 3. 尝试加载 GIF
    petMovie = new QMovie("D:/qt_project/DesktopPet/pet.gif", QByteArray(), this);

    if (petMovie->isValid()) {
        // 如果 GIF 加载成功，让它自适应大小播放
        petMovie->setScaledSize(QSize(300, 300));
        petLabel->setMovie(petMovie);
        petMovie->start(); // 开始播放！
    } else {
        // 如果没找到 GIF，退回到加载静态图或显示提示
        QPixmap pixmap("D:/qt_project/DesktopPet/pet.png");
        if (pixmap.isNull()) {
            petLabel->setText("找不到 pet.gif 或 pet.png");
            petLabel->setStyleSheet("color: red; font-weight: bold; background: transparent;");
        } else {
            petLabel->setPixmap(pixmap.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }

    // 关键：让鼠标事件穿透标签，保证拖拽正常
    petLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

Widget::~Widget()
{
    // 释放动画资源
    if (petMovie->isValid()) {
        petMovie->stop();
    }
}

// 下面是鼠标拖拽和右键菜单，和昨天完全一样，不需要动
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
        move(event->globalPosition().toPoint() - dragPosition);
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