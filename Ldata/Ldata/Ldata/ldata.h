#ifndef LDATA_H
#define LDATA_H

#include <dinput.h>  
#pragma comment(lib, "dinput8.lib")  
#pragma comment(lib, "dxguid.lib") 

#include <QtWidgets/QMainWindow>
#include <QThread>
#include "qdebug.h"
#include "qmessagebox.h"
#include "qmutex.h"
#include "ui_ldata.h"
#include <iostream>

class Ldata : public QMainWindow
{
	Q_OBJECT

public:
	Ldata(QWidget *parent = 0);
	~Ldata();

private:
	Ui::LdataClass ui;
};

class CG27 :
	public QThread
{
	Q_OBJECT
public:
	CG27(void);
	~CG27(void);
public:
	bool InitializeG27();
protected:
	void run();
signals:
	void GetG27Info();
};

#endif // LDATA_H
