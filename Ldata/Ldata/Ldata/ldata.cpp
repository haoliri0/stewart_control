#include "ldata.h"

//extern DIJOYSTATE2 g_G27State;  
//extern QMutex g_mutexG27;
DIJOYSTATE2 g_G27State;  
QMutex g_mutexG27;

Ldata::Ldata(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

Ldata::~Ldata()
{

}
  
LPDIRECTINPUT8          g_pDI = NULL;  
LPDIRECTINPUTDEVICE8    g_pJoystick = NULL;  
  
  
CG27::CG27(void)  
{  
    //  InitializeG27();  
	InitializeG27(); 
	qDebug() << "initializing";
	run();
}  
  
CG27::~CG27(void)  
{  
}  
  
  
//-----------------------------------------------------------------------------  
// Name: EnumJoysticksCallback()  
// Desc: Called once for each enumerated joystick. If we find one, create a  
//       device interface on it so we can play with it.  
//-----------------------------------------------------------------------------  
BOOL CALLBACK  
enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)  
{  
    HRESULT hr;  
  
    // Obtain an interface to the enumerated joystick.  
    hr = g_pDI->CreateDevice(instance->guidInstance, &g_pJoystick, NULL);  
  
    // If it failed, then we can't use this joystick. (Maybe the user unplugged  
    // it while we were in the middle of enumerating it.)  
    if (FAILED(hr)) {   
        return DIENUM_CONTINUE;  
    }  
  
    // Stop enumeration. Note: we're just taking the first joystick we get. You  
    // could store all the enumerated joysticks and let the user pick.  
    //目前只考虑一个方向盘  
    return DIENUM_STOP;  
}  
  
  
BOOL CALLBACK  
enumAxesCallback(const DIDEVICEOBJECTINSTANCE* instance, VOID* context)  
{  
    HWND hDlg = (HWND)context;  
  
    DIPROPRANGE propRange;   
    propRange.diph.dwSize       = sizeof(DIPROPRANGE);   
    propRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);   
    propRange.diph.dwHow        = DIPH_BYID;   
    propRange.diph.dwObj        = instance->dwType;  
    if (instance->guidType == GUID_XAxis )//方向盘  
    {  
        propRange.lMin              = -4500;   
        propRange.lMax              = +4500;   
    }  
    else if(instance->guidType == GUID_Slider )//离合  
    {  
        propRange.lMin              = 0;   
        propRange.lMax              = 1000;   
    }  
    else if(instance->guidType == GUID_RzAxis )//刹车  
    {  
        propRange.lMin              = 0;   
        propRange.lMax              = +1000;   
    }  
    else if(instance->guidType == GUID_YAxis )//油门  
    {  
        propRange.lMin              = 0;   
        propRange.lMax              = +1000;   
    }  
    else  
    {  
        propRange.lMin              = -1000;   
        propRange.lMax              = +1000;   
    }  
  
  
  
    // Set the range for the axis  
    if (FAILED(g_pJoystick->SetProperty(DIPROP_RANGE, &propRange.diph))) {  
        return DIENUM_STOP;  
    }  
  
    return DIENUM_CONTINUE;  
}  
  
bool CG27::InitializeG27()  
{  
    HRESULT hr;  
  
    // Register with the DirectInput subsystem and get a pointer to a IDirectInput interface we can use.  
    // Create a DInput object  
  
    //initialize directinput library  
    if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION,  
        IID_IDirectInput8, ( VOID** )&g_pDI, NULL ) ) )  
        return false;  
  
  
    LPDIRECTINPUTDEVICE8 joystick;  
  
    // Look for the first simple joystick we can find.  
    if (FAILED(hr = g_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, enumCallback,  
        NULL, DIEDFL_ATTACHEDONLY))) {  
            return false;  
    }  
  
    if (g_pJoystick == NULL)    
    {  
//        ShowMessageBox("g27 not found, please check the connection, exiting........");  
		qDebug() << "fucklrl";
        return false;  
    }  
  
    if (FAILED(hr = g_pJoystick->SetDataFormat(&c_dfDIJoystick2)))  
    {  
//        ShowMessageBox(" set g27 data format error, exiting.......");  
		qDebug() << "fuckld";
        return false;  
    }  
  
    g_pJoystick->SetCooperativeLevel(NULL, DISCL_EXCLUSIVE|DISCL_FOREGROUND);  
  
    DIDEVCAPS capabilities;  
    capabilities.dwSize = sizeof(DIDEVCAPS);  
  
    g_pJoystick->GetCapabilities(&capabilities);  
  
    if (FAILED(hr=g_pJoystick->EnumObjects(enumAxesCallback, NULL, DIDFT_AXIS)))  
    {  
  
    }  
	qDebug() << "initializing succeeded";
    return true;  
}  
  
void CG27::run()  
{  
  
    HRESULT hr;  
    if (g_pJoystick == NULL)   
    {  
        return;  
    }  
  
  
    while(1)  
    {  
        hr = g_pJoystick->Poll();  
        g_pJoystick->Acquire();  
        g_mutexG27.lock();  
        hr = g_pJoystick->GetDeviceState( sizeof( DIJOYSTATE2 ), &g_G27State);  
        emit(GetG27Info());  
        
		qDebug() << g_G27State.lX;
	/*	qDebug() << g_G27State.lY;
		qDebug() << g_G27State.lZ;
		qDebug() << g_G27State.lRx;
		qDebug() << g_G27State.lRy;
		qDebug() << g_G27State.lRz;
		qDebug() << g_G27State.lVX;
		qDebug() << g_G27State.lVY;
		qDebug() << g_G27State.lVZ;
		qDebug() << g_G27State.lAX;
		qDebug() << g_G27State.lAY;
		qDebug() << g_G27State.lAZ;
		qDebug() << " ";
	*/
		qDebug() << " ";
		qDebug() << " ";
        
		g_mutexG27.unlock();  
		usleep(5000);  
    }  
  
    return;  
  
}