#ifndef BOARD_H_
#define BOARD_H_

#include <windows.h>
#include "square.h"
#include "2048game.h"

#define BDSIZE 4	//сторона поля в количестве квадратов
#define BDNULLX 10	//х - координата левого верхнего угла поля
#define BDNULLY 10	//у - координата левого верхнего угла поля
#define PIXINKR 4	//приращение пикселей при движении (обязательно делит SQSIZE нацело!)

#define UNKNOWN 0
#define SIDEL 1
#define SIDER 2
#define SIDEU 3
#define SIDED 4


//поле
class board
{
public:
	
	board(HWND hWnd) : hWnd(hWnd) {};
	
	//отрисовка поля вместе с квадратами
	void DrawInMemDC(int side = 1);
	
	//применение изменений
	void getChanges(game *g);

private:

	HWND hWnd;
	
	//массив указателей на квадраты
	square *squares[BDSIZE][BDSIZE] = {NULL};
	
	//добавление нового квадрата
	void addSq(unsigned int val, int row, int col);
	
	//удаление квадрата
	void delSq(int row, int col);
	
	//отрисовка поля
	void drawBoard(HDC workDC);

	//отрисовка всех квадратов
	void drawSquares(HDC workDC, int side);	
};

#endif //BOARD_H_