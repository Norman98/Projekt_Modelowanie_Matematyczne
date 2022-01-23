// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <math.h>
#include <cmath>
#include <windows.h>
#include <stdlib.h> 
#include <sstream>
#include <cstring>
#include <string.h>
#include <conio.h>
#include <string>
#include <cstdio>


#define MAX_LOADSTRING 100
#define TMR_1 1

// Global Variables:
const double H = 0.001;     // krok
const double L = 2.0;      // iloœæ cykli
const double M = 20.0;     // amplituda
const double Pi = 3.1416;  // pi
double total = 1 / H ;     // ilosc próbek
double A=5;                // wspó³czynniki równania ró¿niczkowego
double a=5;
double b=5;
double skalaWe = 6.0;

//const char* filename = "  ";         // nazwa pliku
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

INT value;

// buttons
HWND hwndButton;
// textbox
HWND hwndTextbox;


// sent data
std::vector<Point> u0;
std::vector<Point> ypoint;
std::vector<double> y;         
std::vector<double> u;          
std::vector<double> yprim;


RECT drawArea1 = { 0, 0, 1500, 2000 };
RECT drawArea2 = { 50, 4000, 6500, 4200};

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);


void clear()                                            // czyszczenie wektorów
{
	y.clear();
	yprim.clear();
	u.clear();
	u0.clear();
	ypoint.clear();
}

void osie(HDC hdc) {                                    //wykres osi uk³adu wspó³rzêdnych

	Graphics graphics(hdc);
	Pen pen4(Color(255, 0, 0, 0));
	
	graphics.DrawLine(&pen4, 10, 250, 1180, 250);
	graphics.DrawLine(&pen4, 10, 20, 10, 480);
	graphics.DrawLine(&pen4, 8, 22, 12, 22);
	graphics.DrawLine(&pen4, 9, 21, 11, 21);
	graphics.DrawLine(&pen4, 7, 23, 13, 23);
	graphics.DrawLine(&pen4, 6, 24, 14, 24);
	graphics.DrawLine(&pen4, 1176, 246, 1176, 254);
	graphics.DrawLine(&pen4, 1177, 247, 1177, 253);
	graphics.DrawLine(&pen4, 1178, 248, 1178, 252);
	graphics.DrawLine(&pen4, 1179, 249, 1179, 251);
}

void algorytm(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 255));
	Pen pen2(Color(255, 0, 150, 0));
	Pen pen3(Color(255, 200, 0, 0));

	osie(hdc);

	y.push_back(0);               // warunki pocz¹tkowe
	yprim.push_back(0);           // warunki pocz¹tkowe

	for (int i = 1; i < total; i++)
	{
		y.push_back(y[i - 1] + H*yprim[i - 1] + H*H/2* (u[i - 1] - b*yprim[i - 1] - A*sqrt(abs(a*y[i - 1])) )); //rozwiniêcie Taylora  y[i+1] = y[i] + h*y'[i] + h*h/2 * f(y'[i], y[i], u[i])
		yprim.push_back(yprim[i - 1] + H*(u[i - 1] - b*yprim[i - 1] - A*sqrt(abs(a*y[i - 1]))));                //rozwiniêcie Taylora  y'[i+1] = y'[i] + h * f(y'[i], y[i], u[i])
	}

		for (int i = 0; i < total; i++)
		{
			ypoint.push_back(Point(i, y[i]));
		}

		for (int i = 1; i < total; i++)
		{
			graphics.DrawLine(&pen3, u0[i - 1].X + 10, -u0[i - 1].Y + 250, u0[i].X + 10, -u0[i].Y + 250);                //rysowanie sygna³u wejœciowego
			graphics.DrawLine(&pen2, ypoint[i - 1].X + 10, -ypoint[i - 1].Y + 250, ypoint[i].X + 10, -ypoint[i].Y + 250);//rysowanie sygna³u wyjœciowego
		}
	
}

void MyOnPaint(HDC hdc)         // sinus
{
	clear();          //czyszczenie wektorów

	for (int i = 0; i < total; i++)                           
	{
		u0.push_back(Point(i, skalaWe* M*sin(2.0*Pi*i*H*L)));     //próbki sygna³u wejœciowego  
	}

	for (int i = 0; i < total; i++)
	{
		u.push_back(M*M*M*sin(2.0*Pi*i*H*L)*sin(2.0*Pi*i*H*L)*sin(2.0*Pi*i*H*L));        //próbki u^3
	}

	algorytm(hdc);        // algorytm rozwi¹zywania równania

}

void MyOnPaint2(HDC hdc)          //skok
{

	clear();          //czyszczenie wektorów

	for (int i = 0; i < total; i++)
	{
		u0.push_back(Point(i, skalaWe*M ));         //próbki sygna³u wejœciowego 
	}

	for (int i = 0; i < total; i++)
	{
		u.push_back(M*M*M);        //próbki u^3
	}

	algorytm(hdc);        // algorytm rozwi¹zywania równania

}
	


void MyOnPaint3(HDC hdc)            //prostok¹t
{

	clear();        //czyszczenie wektorów

	for (int i = 0; i < total; i++)
	{
		u0.push_back(Point(i, copysign(skalaWe*M, sin(2.0*Pi*i*H*L))));         //próbki sygna³u wejœciowego 
	}


	for (int i = 0; i < total; i++)
	{
		u.push_back(copysign(M*M*M, sin(2.0*Pi*i*H*L)));   //próbki u^3
	}

	algorytm(hdc);        // algorytm rozwi¹zywania równania

}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	if (drawArea==NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	MyOnPaint(hdc);
	EndPaint(hWnd, &ps);
}
void repaintWindow2(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	if (drawArea == NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE);  //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	MyOnPaint2(hdc);
	EndPaint(hWnd, &ps);
}
void repaintWindow3(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	if (drawArea == NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	MyOnPaint3(hdc);
	EndPaint(hWnd, &ps);
}

void WczytajParametry(HWND hWnd, HDC &hdc)
{
	char a_str[10];

	GetDlgItemTextA(hWnd, TextOut, a_str, sizeof(a_str));
	b = atof(a_str);
	//std::wostringstream parab;
	//parab << b;
	//MessageBox(NULL, parab.str().c_str(), L"dodano parametr b", MB_OK);

	GetDlgItemTextA(hWnd, TextOut1, a_str, sizeof(a_str));
	A = atof(a_str);
	//std::wostringstream paramA;
	//paramA << A;
	//MessageBox(NULL, paramA.str().c_str(), L"dodano parametr A", MB_OK);

	GetDlgItemTextA(hWnd, TextOut2, a_str, sizeof(a_str));
	a = atof(a_str);
	//std::wostringstream parama;
	//parama << a;
	//MessageBox(NULL, parama.str().c_str(), L"dodano parametr a", MB_OK);
  

}

void inputData()                         // plik              
{	
	
}


int OnCreate(HWND window)
{
	inputData();
	return 0;
}


// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	value = 0;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;


	hInst = hInstance; // Store instance handle (of exe) in our global variable

	// main window
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	// create button and store the handle  

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("sinus"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		400, 450,                                  // the left and top co-ordinates
	    160, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON1,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("skok"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		240, 450,                                  // the left and top co-ordinates
		160, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON2,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("prostok¹t"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		80, 450,                                  // the left and top co-ordinates
		160, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON3,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndTextbox = CreateWindow(TEXT("EDIT"),                  
		TEXT("b=5"),                  
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		650, 480, 50, 20,
		hWnd,                                 
		(HMENU)TextOut,                   
		hInstance,                          
		NULL);  


	hwndTextbox = CreateWindow(TEXT("EDIT"),
		TEXT("A=5"),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		700, 480, 50, 20,
		hWnd,
		(HMENU)TextOut1,
		hInstance,
		NULL);


	hwndTextbox = CreateWindow(TEXT("EDIT"),
		TEXT("a=5"),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		750, 480, 50, 20,
		hWnd,
		(HMENU)TextOut2,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("zapisz parametry"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		900, 450,                                  // the left and top co-ordinates
		160, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON4,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	
	hwndTextbox = CreateWindow(TEXT("EDIT"),
		TEXT("y''+b*y'+A*sqrt(abs(a*y))=u^3"),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		650, 450, 200, 20,
		hWnd,
		(HMENU)TextOut3,
		hInstance,
		NULL);


	
	// create button and store the handle                                                       

	OnCreate(hWnd);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window (low priority)
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// MENU & BUTTON messages
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON1 :
			repaintWindow(hWnd, hdc, ps, NULL);
			break;
		case ID_BUTTON2 :
			repaintWindow2(hWnd, hdc, ps, NULL);
			break;
		case ID_BUTTON3:
			repaintWindow3(hWnd, hdc, ps, NULL);
			break;	
		case ID_BUTTON4:
			WczytajParametry(hWnd,hdc);

			break;
	//	case ID_RBUTTON1:
	//		SetTimer(hWnd, TMR_1, 25, 0);
	//		break;
	//	case ID_RBUTTON2:
	//		KillTimer(hWnd, TMR_1);
	//		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here (not depend on timer, buttons)
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			//force window to repaint
			repaintWindow(hWnd, hdc, ps, &drawArea2);
			value++;
			break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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
