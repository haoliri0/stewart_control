#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include <QUdpSocket>
#include <QByteArray>
#include <QString>
#include <QStack>
#include <Windows.h>
#include <QTimer>
#include <QMutex>
#include <QProcess>
#include "matrix.h"
#include "coordi.h"
#include "platform.h"

#include "mpoint.h"

namespace Ui {
class Controller;
}

class Controller : public QMainWindow //控制器类
{
    Q_OBJECT

public:
    explicit Controller(QWidget *parent = 0);
    ~Controller();

private slots:
    void timerUpDate(); //槽函数 向控制器发送报文
    void stateUpDate(); //槽函数 更新当前平台状态

private:
    Ui::Controller *ui;
    void paintEvent(QPaintEvent *); //用于显示模拟的平台状态
    void formatString(QString &org, int n=2, const QChar &ch=QChar(' ')); //将16进制字符串重新格式化
    void sendMessage(QString); //发送报文
    QByteArray hexStringtoByteArray(QString hex); //将格式化后的16进制字符串转化为报文
    Coordi *coordiChange(); //完成上平台坐标变换

    HWND task; //键盘钩子实现
    MPoint* points; //存储上平面六个点相对上平面的坐标
    QUdpSocket *commu; //UDP通信端口
    platform plat; //存储上平台姿态
    Coordi fixPoint[6]; //存储下平面六个点的绝对坐标
    QTimer *m_timer, *m_ti; //计时器
    bool m_flag; //防止机器运动超出范围
    double m_length[6]; //六个支撑杆的长度
};

#endif // CONTROLLER_H

