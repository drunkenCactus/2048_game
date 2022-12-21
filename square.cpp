#ifndef UNICODE
#define UNICODE
#endif 

#include "square.h"

#include <iostream>
#include <sstream>

#include "macro.h"


//конструктор
square::square(unsigned int val, int row, int col) : val(val)
{
	std::cout << "##### NEW SQUARE <" << val << "> (" << row << ", " << col << ')' << std::endl;
	
	x = col*SQSIZE;
	y = row*SQSIZE;

	//связываем поток ввода - вывода со строкой в памяти
	std::wstringstream ss;
	
	//формирование имени файла
	TCHAR fileName[15];
	ss << L"pic/";
	ss << val << L".bmp";
	ss >> fileName;
	
	//загрузка изображения из файла
	hBmp = (HBITMAP)LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}


//отрисовка квадрата в заданном контексте устройства
void square::draw(HDC workDC)
{
	//контекст устройства для изображения
	HDC bmpDC = CreateCompatibleDC(workDC);
	SelectObject(bmpDC, hBmp);
	
	//Функция BitBlt выполняет передачу битовых блоков данных о цвете,
	//соответствующих прямоугольнику пикселей из заданного исходного контекста устройства
	//в целевой контекст устройства
	BitBlt
	(
		workDC,			// дескриптор целевого DC
		x + SQSPACE,	// x-коорд. левого верхнего угла целевого прямоугольника
		y + SQSPACE,	// y-коорд. левого верхнего угла целевого прямоугольника
		SQSIZE,         // ширина целевого прямоугольника
		SQSIZE,         // высота целевого прямоугольника
		bmpDC,          // дескриптор исходного DC
		0,              // x-коорд. левого верхнего угла исходного прямоугольника
		0,              // y-коорд. левого верхнего угла исходного прямоугольника
		SRCCOPY         // код растровой операции
	);
	DeleteDC(bmpDC);
}

//приращение координат при анимации движения
void square::addCoords(int dx, int dy)
{
	x += dx;
	y += dy;
}

//деструктор
square::~square()
{
	std::cout << "**** DESTROY SQUARE <" << val << '>' << std::endl;
	DeleteObject(hBmp);
}