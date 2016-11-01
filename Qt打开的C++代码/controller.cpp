#include "controller.h"
#include "ui_controller.h"
#include <cmath>

#include <QDebug>
#include <QTime>
#include <QColor>
QStack<int> m_stack;
QMutex mutex;

#define SQRT2 1.414214
#define TASKMANAGER "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"
//向注册表中注入信息设置键盘钩子
QString a = "55aa000014010001ffffffff00000404";
QString tail = "12345678abcd";

QPointF D3toD2(double x, double y, double z) { //用于将三维坐标转化为2维以在模拟器中显示
    return QPointF(x + 200 + y*SQRT2, 300 + z - y*SQRT2);
}

QPointF D3toD2Coordi(Coordi temp) { //用于将三维坐标转化为2维以在模拟器中显示
    return QPointF(temp.x + 200 + temp.y*SQRT2, 300 + temp.z - temp.y*SQRT2);
}

double Distance(Coordi x, Coordi y) { //计算两点间的距离
    double xx = x.x - y.x;
    double xy = x.y - y.y;
    double xz = x.z - y.z;
    return sqrt(xx*xx + xy*xy + xz*xz);
}

void Controller::timerUpDate() { //发送报文
    QString temp = a; //a为六轴相对时间模式固定的报文头
    int tempInt[6];
    m_flag = true;
    for(int i = 0; i < 6; i++) {
        tempInt[i] = (m_length[i]-62)*10000; //算出电机实际的伸缩长度
        if(tempInt[i] < 0)
            m_flag = false;
    }
    if(!m_flag)
        return;
    temp += QString::number(50).sprintf("%08x",50); //时间码
    temp += QString::number(tempInt[0]).sprintf("%08x",tempInt[0]); //1到6号电机的长度码
    temp += QString::number(tempInt[1]).sprintf("%08x",tempInt[1]);
    temp += QString::number(tempInt[2]).sprintf("%08x",tempInt[2]);
    temp += QString::number(tempInt[5]).sprintf("%08x",tempInt[3]);
    temp += QString::number(tempInt[4]).sprintf("%08x",tempInt[4]);
    temp += QString::number(tempInt[3]).sprintf("%08x",tempInt[5]);
    temp += tail; //固定格式的尾码
    sendMessage(temp);
}

void Controller::stateUpDate() { //更新当前上平台状态
    int tempKey;
    mutex.lock(); //读写锁
    if(m_stack.size() == 0) { //当一个键都不按的时候复位
        plat.reSet();
    }
    for(int i = 0; i < m_stack.size(); i++) { //根据当前存储的按下的键位更新上平台状态
        tempKey = m_stack[i];
        switch (tempKey) {
        case VK_LEFT: {
            plat.rotateLeft(true); //绕x轴向左转
            break;
        }
        case VK_RIGHT: {
            plat.rotateLeft(false); //绕x轴向右转
            break;
        }
        case VK_UP: {
            plat.rotateUp(true); //绕y轴向上转
            break;
        }
        case VK_DOWN: { //绕y轴向下转
            plat.rotateUp(false);
            break;
        }
        case 'Q': { //绕z轴转动
            plat.rotateTurn(false);
            break;
        }
        case 'E': { //绕z轴转动
            plat.rotateTurn(true);
            break;
        }
        case 'A': { //向左右运动
            plat.left(true);
            break;
        }
        case 'D': { //向左右运动
            plat.left(false);
            break;
        }
        case '8': { //向下
            plat.up(false);
            break;
        }
        case '2': { //向上
            plat.up(true);
            break;
        }
        }
    }
    mutex.unlock(); //解除读写锁
    repaint();
}

HHOOK keyHook=NULL;
HHOOK mouseHook=NULL;


//键盘钩子过程
LRESULT CALLBACK keyProc(int nCode,WPARAM wParam,LPARAM lParam )
{
    LRESULT Result=CallNextHookEx(keyHook,nCode,wParam,lParam);
    //在WH_KEYBOARD_LL模式下lParam 是指向KBDLLHOOKSTRUCT类型地址
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
    //如果nCode等于HC_ACTION则处理该消息，如果小于0，则钩子子程就必须将该消息传递给 CallNextHookEx
    if(nCode == HC_ACTION){
        if(wParam == 0x100) { //如果当前事件是按下键，更新当前存储的键位（添加）
            if(m_stack.indexOf(pkbhs->vkCode) < 0)
                m_stack.push(pkbhs->vkCode);
        }
        if(wParam == 0x101) { //如果当前事件是释放键，更新当前存储的键位（删除）
            int k = m_stack.indexOf(pkbhs->vkCode);
            m_stack.removeAt(k);
        }

    }
    return CallNextHookEx(keyHook, nCode, wParam, lParam);
}

Controller::Controller(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Controller),
    plat(0, 0, 69.5, 0, 0, 0)
{
    m_flag = true;
    m_timer = new QTimer();
    m_ti = new QTimer();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timerUpDate())); //链接信号和槽
    connect(m_ti,SIGNAL(timeout()),this,SLOT(stateUpDate()));

    keyHook =SetWindowsHookEx(WH_KEYBOARD_LL, keyProc,GetModuleHandle(NULL),0); //键盘钩子的设置

    commu = new QUdpSocket(this);
    commu->bind(8410); //绑定UDP端口
    ui->setupUi(this);

    for(int i = 0; i < 6; i++)
        m_length[i] = 0x0; //初始化六个杆的长度

    fixPoint[0].set(17.598, -24.48, 0); //设定下平面六定点的坐标
    fixPoint[1].set(12.4, -27.48, 0);
    fixPoint[2].set(-30, -10, 0);
    fixPoint[3].set(-30, 10, 0);
    fixPoint[4].set(12.4, 27.48, 0);
    fixPoint[5].set(17.598, 24.48, 0);

    points = new MPoint[6];
    points[0].set(25, -5, 0); //设定上平面六定点的坐标
    points[1].set(-10.335, -22.9, 0);
    points[2].set(-14.665, -20.4, 0);
    points[3].set(-14.665, 20.4, 0);
    points[4].set(-10.335, 22.9, 0);
    points[5].set(25, 5, 0);
    sendMessage(""); //电机复位
    m_timer->start(10); //设置发报时间间隔，单位ms
    m_ti->start(20); //设置更新上平面状态间隔，单位ms
}

Controller::~Controller() {
    if(ui)
        delete ui;
    delete commu;
    delete m_ti;
    delete m_timer;
    delete[] points;
}

void Controller::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    p.drawLine(0, 300, 400, 300);
    p.drawLine(D3toD2(0, -30, 0), D3toD2(0, 200, 0));
    p.drawLine(200, 350, 200, 0);
    p.drawLine(QPointF(200, 300), D3toD2(0, 0, 0));
    QPolygonF platPosi, platPosi2;

    Coordi* tempP = coordiChange(); //进行坐标变换，获得上平台六个点的绝对坐标

    //图像绘制
    platPosi << D3toD2Coordi(tempP[0]) << D3toD2Coordi(tempP[1]) << D3toD2Coordi(tempP[2]) << D3toD2Coordi(tempP[3])
            << D3toD2Coordi(tempP[4]) << D3toD2Coordi(tempP[5]) << D3toD2Coordi(tempP[0]);
    platPosi2 << D3toD2Coordi(fixPoint[0]) << D3toD2Coordi(fixPoint[1]) << D3toD2Coordi(fixPoint[2]) << D3toD2Coordi(fixPoint[3])
             << D3toD2Coordi(fixPoint[4]) << D3toD2Coordi(fixPoint[5]) << D3toD2Coordi(fixPoint[0]);

    p.drawLine(D3toD2Coordi(fixPoint[3]), D3toD2Coordi(tempP[3]));
    p.drawLine(D3toD2Coordi(fixPoint[4]), D3toD2Coordi(tempP[4]));
    p.setPen(Qt::gray);
    p.drawLine(D3toD2Coordi(fixPoint[5]), D3toD2Coordi(tempP[5]));
    p.setPen(Qt::blue);
    p.drawLine(D3toD2Coordi(fixPoint[0]), D3toD2Coordi(tempP[0]));
    p.setPen(Qt::yellow);
    p.drawLine(D3toD2Coordi(fixPoint[1]), D3toD2Coordi(tempP[1]));
    p.setPen(Qt::white);
    p.drawLine(D3toD2Coordi(fixPoint[2]), D3toD2Coordi(tempP[2]));

    //计算六个柱的长度
    for(int i = 0; i < 6; i++) {
        m_length[i] = Distance(fixPoint[i], tempP[i]);
    }

    p.setPen(QColor(Qt::green));
    p.drawPolyline(platPosi);
    p.setPen(QColor(Qt::red));
    p.drawPolyline(platPosi2);
}

void Controller::sendMessage(QString temp){
    //发送报文
    if(temp.length() == 0) {
        QString temp1 = a;
        temp1 += QString::number(400).sprintf("%08x",400);
        temp1 += QString::number(100000).sprintf("%08x",100000);
        temp1 += QString::number(100000).sprintf("%08x",100000);
        temp1 += QString::number(100000).sprintf("%08x",100000);
        temp1 += QString::number(100000).sprintf("%08x",100000);
        temp1 += QString::number(100000).sprintf("%08x",100000);
        temp1 += QString::number(100000).sprintf("%08x",100000);
        temp1 += tail;
        QByteArray ba = hexStringtoByteArray(temp1);
        commu->writeDatagram(ba, ba.length(),
                             QHostAddress::Broadcast, 7408); //广播发送至7408端口
        QTime t1;
        t1.start();
        while(t1.elapsed()<300);
    }
    else {
        QByteArray ba = hexStringtoByteArray(temp);
        commu->writeDatagram(ba, ba.length(),
                             QHostAddress::Broadcast, 7408);
    }
}

Coordi* Controller::coordiChange()
{
    Coordi* temp = new Coordi[6];
    PlState tempSt = plat.dump();
    Matrix vectorP1(4, 1, points[0].dump());
    Matrix vectorP2(4, 1, points[1].dump());
    Matrix vectorP3(4, 1, points[2].dump());
    Matrix vectorP4(4, 1, points[3].dump());
    Matrix vectorP5(4, 1, points[4].dump());
    Matrix vectorP6(4, 1, points[5].dump());

    //四个变换矩阵
    double xyzChange[16] =
    {1, 0, 0, tempSt.x,
     0, 1, 0, tempSt.y,
     0, 0, 1, tempSt.z,
     0, 0, 0, 1};
    double cChange[16] =
    {cos(tempSt.c), -sin(tempSt.c), 0, 0,
     sin(tempSt.c),  cos(tempSt.c), 0, 0,
                 0,              0, 1, 0,
                 0,              0, 0, 1};
    double aChange[16] =
    {1,              0,             0, 0,
     0,  cos(tempSt.a), sin(tempSt.a), 0,
     0, -sin(tempSt.a), cos(tempSt.a), 0,
     0,              0,             0, 1};
    double bChange[16] =
    { cos(tempSt.b), 0, -sin(tempSt.b), 0,
                  0, 1,              0, 0,
      sin(tempSt.b), 0,  cos(tempSt.b), 0,
                  0, 0,              0, 1};

    Matrix changeA(4, 4, aChange);
    Matrix changeB(4, 4, bChange);
    Matrix changeC(4, 4, cChange);
    Matrix changeXYZ(4, 4, xyzChange);
    double *tempP[7];

    //坐标变换后得到六个点的绝对坐标
    tempP[0] = changeXYZ.multiply(changeC.multiply(changeB.multiply(changeA.multiply(vectorP1)))).dump();
    tempP[1] = changeXYZ.multiply(changeC.multiply(changeB.multiply(changeA.multiply(vectorP2)))).dump();
    tempP[2] = changeXYZ.multiply(changeC.multiply(changeB.multiply(changeA.multiply(vectorP3)))).dump();
    tempP[3] = changeXYZ.multiply(changeC.multiply(changeB.multiply(changeA.multiply(vectorP4)))).dump();
    tempP[4] = changeXYZ.multiply(changeC.multiply(changeB.multiply(changeA.multiply(vectorP5)))).dump();
    tempP[5] = changeXYZ.multiply(changeC.multiply(changeB.multiply(changeA.multiply(vectorP6)))).dump();

    for(int i = 0; i < 6; i++) {
        temp[i].x = tempP[i][0];
        temp[i].y = tempP[i][1];
        temp[i].z = tempP[i][2];
        delete[] tempP[i];
    }
    return temp;
}

QByteArray Controller::hexStringtoByteArray(QString hex) { //对十六进制字符串进行格式化
    QByteArray ret;
    hex=hex.trimmed();
    formatString(hex,2,' ');
    QStringList sl=hex.split(" ");
    foreach(QString s,sl)
        if(!s.isEmpty())
            ret.append(s.toInt(0,16)&0xFF);
    return ret;
}

void Controller::formatString(QString &org, int n, const QChar &ch) { //将格式化好的十六进制字符串转化为报文
    int size= org.size();
    int space= qRound(size*1.0/n+0.5)-1;
    if(space<=0)
        return;
    for(int i=0,pos=n;i<space;++i,pos+=(n+1)) {
        org.insert(pos,ch);
    }
}
