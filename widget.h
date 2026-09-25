#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QMenu>
#include <QMovie>
#include <QSystemTrayIcon>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void moveEvent(QMoveEvent *event) override;

private:
    QLabel *petLabel;
    QMovie *petMovie;
    QPoint dragPosition;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    void initTray();
};

#endif // WIDGET_H