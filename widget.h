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
    void mouseReleaseEvent(QMouseEvent *event) override; // 新增：鼠标松开（用来判断是点击还是拖拽）
    void contextMenuEvent(QContextMenuEvent *event) override;
    void moveEvent(QMoveEvent *event) override;

private:
    QLabel *petLabel;
    QMovie *petMovie;
    QPoint dragPosition;
    QPoint pressPos;         // 新增：记录鼠标按下的位置
    float currentScale;      // 新增：当前缩放比例

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    void initTray();
    void resizePet(float factor); // 新增：缩放桌宠的方法
};

#endif // WIDGET_H