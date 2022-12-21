#ifndef UNICODE
#define UNICODE
#endif 

#include <iostream>
#include <windows.h>
#include <vector>

#include "macro.h"
#include "board.h"


board *bd;
game *gm;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //создание оконного класса

    WNDCLASS wc;
	const wchar_t className[] = L"class";

    //заполнение оконного класса

    wc.style			= CS_OWNDC;								// стиль окна									
    wc.lpfnWndProc		= WindowProc;							// указатель  на оконную процедуру		
    wc.cbClsExtra		= 0;									// дополнительные байты после класса. Всегда ставьте 0
    wc.cbWndExtra		= 0;									// дополнительные байты после экземпляра окна. Всегда ставьте 0
    wc.hInstance		= hInstance;							// экземпляр приложения. Передаётся  в виде параметра  в WinMain		
    wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);		// иконка приложения							
    wc.hCursor			= LoadCursor(NULL, IDC_HAND);			// курсор приложения						
    wc.hbrBackground	= (HBRUSH)(6);							// цвет фона		
    wc.lpszMenuName		= NULL;									// имя меню
    wc.lpszClassName	= className;							// имя класса		

    //регистрация класса окна
    RegisterClass(&wc);


    //создание окна

    //Функция возвращает описатель (дескриптор) окна.
    //Описатели окон используются чтобы отличать окна друг от друга.
    //Нам нужна переменная, которая будет хранить описатель созданного окна, для обработки сообщений.

	HWND hWnd = CreateWindow
	(
		className,				// имя класса
		L"2048",				// имя окна (отображается  в заголовке)
		WS_OVERLAPPEDWINDOW,	// стиль окна
		150,					// координата  по горизонтали  от левого края экрана
		100,					// координата  по вертикали  от верхнего края экрана
		500,					// ширина окна
		400,					// высота окна
		NULL,					// родительское окно
		NULL,					// описатель меню
		hInstance,				// экземпляр приложения	
		NULL					// параметр; всегда ставьте NULL
	);
    
    //показ окна

    //Первый аргумент - описатель созданного нами окна, второй - четвёртый аргумент WinMain.
    //В данной функции мы говорим созданному окну (hWnd) как оно должно отображаться (через nCmdShow).
	ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

	//экземпляр сообщения
	MSG msg;
	
	
	//экземпляр поля
	bd = new board(hWnd);
	
	//экземпляр игры
	gm = new game;
	
	//предварительное добавление на поле двух квадратов
	gm->insert_rand_num();
	bd->getChanges(gm);
	
	gm->insert_rand_num();
	bd->getChanges(gm);
	
	//цикл обработки сообщений
	for(;;)
	{
		if(PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			bd->DrawInMemDC();
		}
	}

    return 0;
}

//обработка нажатия кнопок
int pButton(WPARAM wParam)
{
	int side;
	
	switch(wParam) 
	{ 
		case VK_LEFT: 
			
			CV("left")
			side = SIDEL;
			break; 
	
		case VK_RIGHT: 
			
			CV("right")
			side = SIDER;
			break; 
	
		case VK_UP: 
			
			CV("up")
			side = SIDEU;
			break; 
	
		case VK_DOWN: 
			
			CV("down")
			side = SIDED;
			break;
			
		default:
			side = UNKNOWN;
			break; 
	}
	
	return side;
}


//оконная процедура
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int side = UNKNOWN;
	switch (uMsg)
    {
		//сообщение о закрытии окна (Х)
		case WM_CLOSE:
			//CV("WM_CLOSE")

			//уничтожение окна
			//по видимому, вызывается заново WindowProc с сообщением WM_DESTROY
        	DestroyWindow(hWnd);
        	break;
		
		//сообщение об уничтожении окна
		case WM_DESTROY:
        	//CV("WM_DESTROY")

			//функция указывает системе, что поток сделал запрос на то, чтобы завершить свою работу (завершение работы приложения)
			//генерирует сообщение WM_QUIT, которое никак не связанос потоком данного окна
			//WM_QUIT можно поймать только из общего потока
			PostQuitMessage(0);
        	break;
		
		//сообщение о нажатии клавиши
		//тип нажатой клавиши содержится в wParam
		case WM_KEYDOWN:
			//CV("WM_KEYDOWN")
		
			side = pButton(wParam);
			if (side != UNKNOWN)
			{
				gm->move(side);
				bd->getChanges(gm);
			}
			break;
			
		default:
        	return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}
