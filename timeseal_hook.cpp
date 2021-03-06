#include "stdafx.h"
#pragma comment(lib, "detours.lib")
 
#undef UNICODE
#include <cstdio>
#include <windows.h>
#include <detours.h> 
#include <process.h>
#include <iostream>
#include <string>
 
using std::cin;
using std::cout;
#pragma pack(1)

int Time = 0;
int offset = 480;
int randmod = 1;
 
#define MYMENU_EXIT         (WM_APP + 101)
#define MYMENU_MESSAGEBOX   (WM_APP + 102) 
 
HINSTANCE  inj_hModule;          //Injected Modules Handle
HWND       prnt_hWnd;            //Parent Window Handle
 
//WndProc for the new window
LRESULT CALLBACK DLLWindowProc (HWND, UINT, WPARAM, LPARAM);
 
//Register our windows Class
BOOL RegisterDLLWindowClass(wchar_t szClassName[])
{
    WNDCLASSEX wc;
    wc.hInstance =  inj_hModule;
    wc.lpszClassName = (LPCWSTR)L"InjectedDLLWindowClass";
    wc.lpszClassName = (LPCWSTR)szClassName;
    wc.lpfnWndProc = DLLWindowProc;
    wc.style = CS_DBLCLKS;
    wc.cbSize = sizeof (WNDCLASSEX);
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.lpszMenuName = NULL;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    if (!RegisterClassEx (&wc))
        return 0;
}
 
#define ONE_PLUS_ZERO 600
#define ZERO_PLUS_ONE 601
#define MINIMUM 602
#define BIGTIME 603
#define TIMELOOSE 604
//Creating our windows Menu
HMENU CreateDLLWindowMenu()
{
    HMENU hMenu;
    hMenu = CreateMenu();
    HMENU hMenuPopup;
    if(hMenu==NULL)
        return FALSE;
    hMenuPopup = CreatePopupMenu();
    AppendMenu (hMenuPopup, MF_STRING, MYMENU_EXIT, TEXT("Exit"));
    AppendMenu (hMenu, MF_POPUP, (UINT_PTR) hMenuPopup, TEXT("Exit")); 
 
    hMenuPopup = CreatePopupMenu();
    AppendMenu (hMenuPopup, MF_STRING, ONE_PLUS_ZERO, TEXT("Preset '1 0'")); 
    AppendMenu (hMenuPopup, MF_STRING, ZERO_PLUS_ONE, TEXT("Preset '0 1'")); 
    AppendMenu (hMenuPopup, MF_STRING, MINIMUM, TEXT("MINIMUM")); 
    AppendMenu (hMenuPopup, MF_STRING, BIGTIME, TEXT("BIGTIME")); 
    AppendMenu (hMenuPopup, MF_STRING, TIMELOOSE, TEXT("TIMELOOSE")); 
    AppendMenu (hMenu, MF_POPUP, (UINT_PTR) hMenuPopup, TEXT("TimeSeal")); 
    return hMenu;
}
 
//The new thread
DWORD WINAPI ThreadProc( LPVOID lpParam )
{
    MSG messages;
    wchar_t *pString = reinterpret_cast<wchar_t * > (lpParam);
    HMENU hMenu = CreateDLLWindowMenu();
    RegisterDLLWindowClass(L"InjectedDLLWindowClass");
    prnt_hWnd = FindWindow(L"Window Injected Into ClassName", L"Window Injected Into Caption");
    HWND hwnd = CreateWindowEx (0, L"InjectedDLLWindowClass", pString, WS_EX_PALETTEWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, prnt_hWnd, hMenu,inj_hModule, NULL );
    ShowWindow (hwnd, SW_SHOWNORMAL);
    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return 1;
}
//Our new windows proc
LRESULT CALLBACK DLLWindowProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        switch(wParam)
        {
        case MYMENU_EXIT:
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        case ONE_PLUS_ZERO:
            offset = 502;
            randmod = 1320; // 6 Moves per Second
            break;
        case ZERO_PLUS_ONE:
            offset = 502;
            randmod = 1320; // 6 Moves per Second
            break;
        case MINIMUM:
            offset = 102;
            randmod = 200;
            break;
        case BIGTIME:
            offset = 2000;
            randmod = 3000;
            break;
        case TIMELOOSE:
            offset = 100000;
            randmod = 30000;
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage (0);
        break;
    default:
        return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
 
void startup()
{
    CreateThread(0, NULL, ThreadProc, (LPVOID)L"Timeseal Cheat", NULL, NULL);
    __asm mov eax, 0x00401400
    __asm call eax
    __asm mov Time, eax
}
 
int __cdecl Hooked_GetTimeForLag()
{
    int random = 0;
    __asm mov eax, 0x004021F0
    __asm call eax
    __asm mov random, eax
    Time = Time + (random%randmod);
    Time = Time + offset;
    return Time;
}
 
HMODULE libraryHandle;
_declspec(dllexport) BOOL WINAPI GetUserNameA(LPSTR input, LPDWORD buffer)
{
    typedef BOOL (WINAPI* CFunction) (LPSTR input, LPDWORD buffer);
    CFunction getUserName = (CFunction)GetProcAddress(libraryHandle, "GetUserNameA");
    return getUserName(input, buffer);
}
 
BOOL WINAPI DllMain(HMODULE module,DWORD action,LPVOID reserved)
{
    libraryHandle = LoadLibraryA("ADVAPI32.dll");
 
    switch(action)
    {
    case DLL_PROCESS_ATTACH:
        {
            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            startup();
            DetourAttach(&(PVOID&)GetTimeForLag, Hooked_GetTimeForLag);
            DetourTransactionCommit();
            break;
        }
        break;
    case DLL_THREAD_ATTACH:
        // Shutdown Functions 
        break;
    }
    return true;
}
