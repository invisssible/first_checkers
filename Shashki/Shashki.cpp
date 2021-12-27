// Shashki.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "Shashki.h"
#include "GamePainter.h"
#include "Game.h"

#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;								// ������� ���������
TCHAR szTitle[MAX_LOADSTRING];					// ����� ������ ���������
TCHAR szWindowClass[MAX_LOADSTRING];			// ��� ������ �������� ����
HWND hWnd;

GamePainter * gp ; // ������ ���������
Game * game ; // ������ ����
STEP step ; // ������� ���
int w ;
int h ;
bool gameover ; // ����� ����� ����

// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���������� ��� �����.
	MSG msg;
	HACCEL hAccelTable;

	// ������������� ���������� �����
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SHASHKI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ��������� ������������� ����������:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SHASHKI));

	// ���� ��������� ���������:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �������: MyRegisterClass()
//
//  ����������: ������������ ����� ����.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHASHKI));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SHASHKI);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

// ���������� �����
void refreshForm() {
	InvalidateRect(hWnd,NULL,true) ;
	UpdateWindow(hWnd) ;
}

// ���������� ����
void updateStep() {
	if (gameover) 
		SetWindowTextW(hWnd,L"��� ����������") ;
	else {
		if (step==sHuman1) SetWindowTextW(hWnd,L"ճ� ������ 1") ;
		else SetWindowTextW(hWnd,L"ճ� ������ 2") ;		
	}
}

// ����� ����
void NewGame() {
	// �������� ��������
	gp = new GamePainter() ;
	game = new Game() ;
	gp->setGame(game) ;
	// ������ ��� ������ 1
	step = sHuman1 ;
	gameover = false ;
	updateStep() ;
}

//
//   �������: InitInstance(HINSTANCE, int)
//
//   ����������: ��������� ��������� ���������� � ������� ������� ����.
//
//   �����������:
//
//        � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//        ��������� � ��������� �� ����� ������� ���� ���������.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   
   hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   
      // ���������������� ���� �� ������
   int sw = GetSystemMetrics(SM_CXSCREEN); 
   int sh = GetSystemMetrics(SM_CYSCREEN); 

   // ��������� �����
   w = 600 ;
   h = 600 ;

   MoveWindow(hWnd, (sw-w)/2,(sh-h)/2,w,h,true) ;
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   updateStep() ;

   return TRUE;
}

//
//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����������:  ������������ ��������� � ������� ����.
//
//  WM_COMMAND	- ��������� ���� ����������
//  WM_PAINT	-��������� ������� ����
//  WM_DESTROY	 - ������ ��������� � ������ � ���������.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	char msg[255] ;
	CLICKRESULT cr ;
	bool win ;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// ��������� ����� � ����:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_START:
			NewGame() ;
			refreshForm() ;
			break ;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		// ��������� �����
		hdc = BeginPaint(hWnd, &ps);
		gp->drawTo(hdc,w,h) ;
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		if (gameover) break ;
		
		// �������� ������ ����
		cr = gp->doClick(LOWORD(lParam),HIWORD(lParam),step,msg) ;
		// ���� �������� ���������, �������
		if (cr==crMsg) MessageBoxA(hWnd,msg,"OK",0) ;
		// ���� �������� ���
		if (cr==crFinish) {			
			// ��������, �� ����� �� ����
			if (game->testWin(&win,step)) {
				gameover=true ;
				if (win) MessageBoxA(hWnd,"��������!","OK",0) ; 
				else MessageBoxA(hWnd,"�������!","OK",0) ; 
			}			
			else {
				if (step==sHuman1) step=sHuman2 ; else step=sHuman1 ;
				gp->setRotated(step==sHuman2) ;
				updateStep() ;				
			}
		}
		refreshForm() ;		
		break;
	case WM_CREATE:
		NewGame() ;
		break ;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���������� ��������� ��� ���� "� ���������".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
