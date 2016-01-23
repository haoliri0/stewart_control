#include "stdafx.h"

//在注册表该目录下增加新内容
#define TASKMANAGER "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"

using namespace std;

#pragma warning

HHOOK keyHook = NULL;
HHOOK mouseHook = NULL;

volatile int key_state[10];	
//keystate表示按键状态，分别对应up,down,left,right,z,x,w,s键。1表示按下，0表示未按下。
//key_state[8]和key_state[9]待定

class DMEngine {	//调用matlab engine的相关class

	Engine *ep;

public:
	DMEngine() {
		int statusmat = 0;
		static bool firstOpen = false;
		if (firstOpen)
			ep = engOpen("");
		else
			ep = engOpenSingleUse(NULL, NULL, &statusmat);
		firstOpen = false;
		if (!ep) {
			std::cerr << "\nCan't start MATLAB engine\n" << std::endl;
		}
	}

	~DMEngine() {
		engClose(ep);
	}

	int Eval(std::string s) {
		return engEvalString(ep, s.c_str());
	}

	int PutVariable(std::string var_name, const mxArray *ap) {
		return engPutVariable(ep, var_name.c_str(), ap);
	}

	mxArray* GetVariable(std::string var_name) {
		return engGetVariable(ep, var_name.c_str());
	}

	void Pause() { system("pause"); }
} *matlab_engine;

//键盘钩子过程
LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	//在WH_KEYBOARD_LL模式下lParam 是指向KBDLLHOOKSTRUCT类型地址
	KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *)lParam;
	//如果nCode等于HC_ACTION则处理该消息，如果小于0，则钩子子程就必须将该消息传递给 CallNextHookEx
	if (nCode == HC_ACTION) {
		//cerr << pkbhs->vkCode << endl;

		if (wParam == 0x100) {	//wParam等于256表示键按下
			if (pkbhs->vkCode == VK_UP)	//上键
			{ 
				key_state[0] = 1;
			}
			if (pkbhs->vkCode == VK_DOWN)	//下键
			{
				key_state[1] = 1;
			}
			if (pkbhs->vkCode == VK_LEFT)	//左键
			{
				key_state[2] = 1;
			}
			if (pkbhs->vkCode == VK_RIGHT)	//右键
			{
				key_state[3] = 1;
			}
			if (pkbhs->vkCode == 90)	//z键，实际为大写Z对应的ascii码
			{
				key_state[4] = 1;
			}
			if (pkbhs->vkCode == 88)	//x键，实际为大写X对应的ascii码
			{
				key_state[5] = 1;
			}
			if (pkbhs->vkCode == 87)// 'W'
			{
				key_state[6] = 1;
			}
			if (pkbhs->vkCode == 83)// 'S'
			{
				key_state[7] = 1;
			}
			//cout << pkbhs->vkCode << endl;
		}
		else {	//此时表示键弹起
			if (pkbhs->vkCode == VK_UP)
			{
				key_state[0] = 0;
			}
			if (pkbhs->vkCode == VK_DOWN)
			{
				key_state[1] = 0;
			}
			if (pkbhs->vkCode == VK_LEFT)
			{
				key_state[2] = 0;
			}
			if (pkbhs->vkCode == VK_RIGHT)
			{
				key_state[3] = 0;
			}
			if (pkbhs->vkCode == 90)// 'Z'
			{
				key_state[4] = 0;
			}
			if (pkbhs->vkCode == 88)// 'X'
			{
				key_state[5] = 0;
			}
			if (pkbhs->vkCode == 87)// 'W'
			{
				key_state[6] = 0;
			}
			if (pkbhs->vkCode == 83)// 'S'
			{
				key_state[7] = 0;
			}
		}
		if (pkbhs->vkCode == VK_F12)
		{
			void unHook();
		}

		return 0;//返回1表示截取消息不再传递,返回0表示不作处理,消息继续传递
	}
	return CallNextHookEx(keyHook, nCode, wParam, lParam);
}
//鼠标钩子过程
LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return 1;

}
//卸载钩子
void unHook()
{
	UnhookWindowsHookEx(keyHook);
	//  UnhookWindowsHookEx(mouseHook);

}
//安装钩子,调用该函数即安装钩子
void setHook()
{
	//这两个底层钩子,不要DLL就可以全局
	//底层键盘钩子
	keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, GetModuleHandle(NULL), 0);
	//底层鼠标钩子
	//    mouseHook =SetWindowsHookEx( WH_MOUSE_LL,mouseProc,GetModuleHandle(NULL),0);
}

void controller() {

// 切记，应当在复位后手动调整至(x, y, z) = (0, 0, 440) (a, b, c) = (0, 0, 0)，这样不会出现跳变，比较安全
	matlab_engine = new DMEngine();
	
	//添加matlab执行路径到engine中，该路径下存放了main_stewart.m和play_visit.m等
	matlab_engine->Eval("addpath('G:\\control\\matlab')");

	int ct[6] = { 0,0,450,0,0,0 };	//平台的初始位姿，应当在复位后调整至该状态 (x,y,z) = (0,0,440) (a,b,c) = (0,0,0)
	int initial[6] = { 0,0,450,0,0,0 };
	
	//平台运行的上、下限度
	int high_bound[6] = { 50,20,520,16,16,24 };	
	int low_bound[6] = { -50,-20,410,-16,-16,-24 };
	//int high_bound[6] = { 20,20,450,21,21,15 };
	//int low_bound[6] = { -20,-20,350,-21,-21,-15 };

	int delta_foward[6] = { 0,0,0,2,0,0 };	//按up键的位姿调整，down键则相反
	int delta_left[6] = { 0,0,0,0,2,0 };  //按left键的位姿调整，right键则相反
	int delta_z[6] = { 0,0,0,0,0,3 };	//按z键的位姿调整，x键则相反
	int  delta_up[6] = { 0,0,15,0,0,0 };
	int delta_shift_1[6] = { 5,0,0,0,0,0 };
	int delta_shift_2[6] = { 0,3,0,0,0,0 };
	//int delta_temp[6] = { 3,3,4,3,3,3 };
	bool flag = false;

	int temp = -1;

	//Reset
	{
		stringstream ss;
		ss << "Reset";
		string cmd = ss.str();
		cout << "run command: " << cmd << endl;
		//int rt = matlab_engine->Eval(cmd);
		//cout << "return " << rt << endl;
		ss.clear();
		ss.str("");

		int temp_a[6] = { 0,0,420,0,0,0 };
		ss << "main_stewart(";
		for (int i = 0; i < 5; i++) ss << temp_a[i] << ", ";
		ss << ct[5];
		ss << ")";
		cmd = ss.str();
		cout << "run command: " << cmd << endl;
		int rt = matlab_engine->Eval(cmd);	//调用matlab，指令为cmd
		cout << "return " << rt << endl;

		Sleep(500);
		cmd[20] = '5';
		cout << "run command: " << cmd << endl;
		rt = matlab_engine->Eval(cmd);	//调用matlab，指令为cmd
		cout << "return " << rt << endl;

	}
	
	while (1) {
		Sleep(20);
		flag = false;
		if (key_state[0] == 1) {
			if (ct[2] + delta_up[2] <= high_bound[2] && ct[3] + delta_up[3] <= high_bound[3]) {
				ct[2] += delta_up[2];
				ct[3] += delta_up[3];
			}
			flag = true;
			temp = 0;
		}
		if (key_state[1] == 1) {
			if (ct[2] - delta_up[2] >= low_bound[2] && ct[3] - delta_up[3] >= low_bound[3]) {
				ct[2] -= delta_up[2];
				ct[3] -= delta_up[3];
			}
			flag = true;
			temp = 1;
		}
		if (key_state[2] == 1) {
			if (ct[2] + delta_left[2] <= high_bound[2] && ct[4] + delta_left[4] <= high_bound[4]) {
				ct[2] += delta_left[2];
				ct[4] += delta_left[4];
			}
			flag = true;
			temp = 2;
		}
		if (key_state[3] == 1) {
			if (ct[2] - delta_left[2] >= low_bound[2] && ct[4] - delta_left[4] >= low_bound[4]) {
				ct[2] -= delta_left[2];
				ct[4] -= delta_left[4];
			}
			flag = true;
			temp = 3;
		}
		if (key_state[4] == 1) {
			if (ct[5] + delta_z[5] <= high_bound[5]) {
				ct[5] += delta_z[5];
			}
			flag = true;
			temp = 4;
		}
		if (key_state[5] == 1) {
			if (ct[5] - delta_z[5] >= low_bound[5]) {
				ct[5] -= delta_z[5];
			}
			flag = true;
			temp = 5;
		}
		if (key_state[6] == 1) {
			if (ct[0] + delta_shift_1[0] <= high_bound[0]) {
				ct[0] += delta_shift_1[0];
			}
			flag = true;
			temp = 6;
		}
		if (key_state[7] == 1) {
			if (ct[0] - delta_shift_1[0] >= low_bound[0]) {
				ct[0] -= delta_shift_1[0];
			}
			flag = true;
			temp = 7;
		}


		stringstream ss;
		ss << "main_stewart(";
		for (int i = 0; i < 5; i++) ss << ct[i] << ", ";
		ss << ct[5];
		ss << ")";
		string cmd = ss.str();
		if (flag == true) {
			cout << "run command: " << cmd << endl;
			int rt = matlab_engine->Eval(cmd);	//调用matlab，指令为cmd
			cout << "return " << rt << endl;
		}
		/*
		下面注释的为自动复位的代码，貌似还有点问题，比较简单，建议自主实现
		else if (ct[temp] != initial[temp]) {
			if (temp % 2 == 0)
				if (ct[delta_temp[temp]] - 2 <= high_bound[delta_temp[temp]] && ct[delta_temp[temp]] - 2 >= low_bound[delta_temp[temp]])
					ct[delta_temp[temp]] = ct[delta_temp[temp]] - 2;
			else
				if (ct[delta_temp[temp]] + 2 <= high_bound[delta_temp[temp]] && ct[delta_temp[temp]] + 2 >= low_bound[delta_temp[temp]])
					ct[delta_temp[temp]] = ct[delta_temp[temp]] + 2;
		}
		*/
	
	}
	delete matlab_engine;
}

int main()
{
	for (int i = 0; i < 4; i++) key_state[i] = 0;

	thread th_controller(controller);	//创建独立线程来调用matlab

	setHook();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{

	}
}
