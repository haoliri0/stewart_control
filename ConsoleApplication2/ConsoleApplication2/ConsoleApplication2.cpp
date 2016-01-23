#include "stdafx.h"

//��ע����Ŀ¼������������
#define TASKMANAGER "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"

using namespace std;

#pragma warning

HHOOK keyHook = NULL;
HHOOK mouseHook = NULL;

volatile int key_state[10];	
//keystate��ʾ����״̬���ֱ��Ӧup,down,left,right,z,x,w,s����1��ʾ���£�0��ʾδ���¡�
//key_state[8]��key_state[9]����

class DMEngine {	//����matlab engine�����class

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

//���̹��ӹ���
LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	//��WH_KEYBOARD_LLģʽ��lParam ��ָ��KBDLLHOOKSTRUCT���͵�ַ
	KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *)lParam;
	//���nCode����HC_ACTION�������Ϣ�����С��0�������ӳ̾ͱ��뽫����Ϣ���ݸ� CallNextHookEx
	if (nCode == HC_ACTION) {
		//cerr << pkbhs->vkCode << endl;

		if (wParam == 0x100) {	//wParam����256��ʾ������
			if (pkbhs->vkCode == VK_UP)	//�ϼ�
			{ 
				key_state[0] = 1;
			}
			if (pkbhs->vkCode == VK_DOWN)	//�¼�
			{
				key_state[1] = 1;
			}
			if (pkbhs->vkCode == VK_LEFT)	//���
			{
				key_state[2] = 1;
			}
			if (pkbhs->vkCode == VK_RIGHT)	//�Ҽ�
			{
				key_state[3] = 1;
			}
			if (pkbhs->vkCode == 90)	//z����ʵ��Ϊ��дZ��Ӧ��ascii��
			{
				key_state[4] = 1;
			}
			if (pkbhs->vkCode == 88)	//x����ʵ��Ϊ��дX��Ӧ��ascii��
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
		else {	//��ʱ��ʾ������
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

		return 0;//����1��ʾ��ȡ��Ϣ���ٴ���,����0��ʾ��������,��Ϣ��������
	}
	return CallNextHookEx(keyHook, nCode, wParam, lParam);
}
//��깳�ӹ���
LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return 1;

}
//ж�ع���
void unHook()
{
	UnhookWindowsHookEx(keyHook);
	//  UnhookWindowsHookEx(mouseHook);

}
//��װ����,���øú�������װ����
void setHook()
{
	//�������ײ㹳��,��ҪDLL�Ϳ���ȫ��
	//�ײ���̹���
	keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, GetModuleHandle(NULL), 0);
	//�ײ���깳��
	//    mouseHook =SetWindowsHookEx( WH_MOUSE_LL,mouseProc,GetModuleHandle(NULL),0);
}

void controller() {

// �мǣ�Ӧ���ڸ�λ���ֶ�������(x, y, z) = (0, 0, 440) (a, b, c) = (0, 0, 0)����������������䣬�Ƚϰ�ȫ
	matlab_engine = new DMEngine();
	
	//���matlabִ��·����engine�У���·���´����main_stewart.m��play_visit.m��
	matlab_engine->Eval("addpath('G:\\control\\matlab')");

	int ct[6] = { 0,0,450,0,0,0 };	//ƽ̨�ĳ�ʼλ�ˣ�Ӧ���ڸ�λ���������״̬ (x,y,z) = (0,0,440) (a,b,c) = (0,0,0)
	int initial[6] = { 0,0,450,0,0,0 };
	
	//ƽ̨���е��ϡ����޶�
	int high_bound[6] = { 50,20,520,16,16,24 };	
	int low_bound[6] = { -50,-20,410,-16,-16,-24 };
	//int high_bound[6] = { 20,20,450,21,21,15 };
	//int low_bound[6] = { -20,-20,350,-21,-21,-15 };

	int delta_foward[6] = { 0,0,0,2,0,0 };	//��up����λ�˵�����down�����෴
	int delta_left[6] = { 0,0,0,0,2,0 };  //��left����λ�˵�����right�����෴
	int delta_z[6] = { 0,0,0,0,0,3 };	//��z����λ�˵�����x�����෴
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
		int rt = matlab_engine->Eval(cmd);	//����matlab��ָ��Ϊcmd
		cout << "return " << rt << endl;

		Sleep(500);
		cmd[20] = '5';
		cout << "run command: " << cmd << endl;
		rt = matlab_engine->Eval(cmd);	//����matlab��ָ��Ϊcmd
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
			int rt = matlab_engine->Eval(cmd);	//����matlab��ָ��Ϊcmd
			cout << "return " << rt << endl;
		}
		/*
		����ע�͵�Ϊ�Զ���λ�Ĵ��룬ò�ƻ��е����⣬�Ƚϼ򵥣���������ʵ��
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

	thread th_controller(controller);	//���������߳�������matlab

	setHook();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{

	}
}
