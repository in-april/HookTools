// HookMod.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "HookMod.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CHookModApp, CWinApp)
END_MESSAGE_MAP()


// CHookModApp 构造

CHookModApp::CHookModApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CHookModApp 对象

CHookModApp theApp;


// CHookModApp 初始化
HHOOK keyHook;
LRESULT CALLBACK KeyCallBack(int nCode, WPARAM wParam, LPARAM lParam);
BOOL CHookModApp::InitInstance()
{
	CWinApp::InitInstance();
	
	keyHook = SetWindowsHook(WH_KEYBOARD, KeyCallBack);
	return TRUE;
}
LRESULT CALLBACK KeyCallBack(int nCode, WPARAM wParam, LPARAM lParam)
{
	if ((lParam & 0x80000000) == 0) //按下
	{
		switch (wParam)
		{
		case VK_HOME: // home键
		{
			theApp.m_dlgMain.Create(IDD_DIALOG_MAIN);
			theApp.m_dlgMain.ShowWindow(SW_SHOW);
		}
		default:
			break;
		}
	}
	return CallNextHookEx(keyHook, nCode, wParam, lParam);
}
