#ifndef UNICODE
#define UNICODE
#endif 

#include "board.h"

#include "macro.h"


//отрисовка поля
void board::drawBoard(HDC workDC)
{
	//Функция SelectObject выбирает объект в заданный контекст устройства
	//Функция GetStockObject извлекает дескриптор одного из
	//предопределенных (стандартных) перьев, кистей, шрифтов или палитр
	SelectObject(workDC, GetStockObject(DC_BRUSH));
	SetDCBrushColor(workDC, RGB(255, 255, 255));
	
	SelectObject(workDC, GetStockObject(DC_PEN));
	SetDCPenColor(workDC, RGB(0, 0, 0));
	
	//отрисовка квадрата: граница ручкой, заливка кистью
	Rectangle(workDC, 0, 0, BDSIZE*SQSIZE, BDSIZE*SQSIZE);
}

//добавление нового квадрата
void board::addSq(unsigned int val, int row, int col)
{
	if(squares[row][col] != NULL)
		return;
	squares[row][col] = new square(val, row, col);
}

//удаление квадрата
void board::delSq(int row, int col)
{
	if(squares[row][col] == NULL)
		return;
	
	delete squares[row][col];
	squares[row][col] = NULL;
}

//отрисовка всех квадратов
//последовательность сонаправлена с направлением сдвига
void board::drawSquares(HDC workDC, int side)
{
	switch(side)
	{
		case SIDEL:
		
			for(int i=0; i<BDSIZE; i++)
				for(int j=BDSIZE-1; j>=0; j--)
					if(squares[i][j] != NULL)
						squares[i][j]->draw(workDC);
			break;
		
		case SIDER:
		
			for(int i=0; i<BDSIZE; i++)
				for(int j=0; j<BDSIZE; j++)
					if(squares[i][j] != NULL)
						squares[i][j]->draw(workDC);
			break;
			
		case SIDEU:
		
			for(int j=0; j<BDSIZE; j++)
				for(int i=0; i<BDSIZE; i++)
					if(squares[i][j] != NULL)
						squares[i][j]->draw(workDC);
			break;
			
		case SIDED:
			
			for(int j=0; j<BDSIZE; j++)
				for(int i=0; i<BDSIZE; i++)
					if(squares[i][j] != NULL)
						squares[i][j]->draw(workDC);
			break;
	}
}

//отрисовка поля вместе с квадратами
void board::DrawInMemDC(int side)
{
	//Функция GetDC извлекает дескриптор дисплейного контекста устройства (DC)
	HDC wndDC = GetDC(hWnd);
	
	//создание контекста устройства в памяти:
	//необходимо для устранения мигания - квадраты будут рисоваться по очереди
	//в контекст в памяти, затем все поле с квадратами сразу отрисуется в контекст окна.
	//если рисовать квадраты по очереди сразу в контекст окна, то будет мигание
	HDC memDC = CreateCompatibleDC(wndDC);

	//создание точечного рисунка указанных размеров
	HBITMAP memBM = CreateCompatibleBitmap
	(
		wndDC,
		BDSIZE*SQSIZE,
		BDSIZE*SQSIZE
	);

	//вибираем созданный рисунок в созданный контекст
	SelectObject(memDC, memBM);

	//рисуем поле с квадратами в этом контексте в памяти
	drawBoard(memDC);
	drawSquares(memDC, side);

	//копируем картинку из контекста в памяти в контекст окна
	BitBlt
	(
		wndDC,				// дескриптор целевого DC		
		BDNULLX,			// x-коорд. левого верхнего угла целевого прямоугольника
		BDNULLY,			// y-коорд. левого верхнего угла целевого прямоугольника
		BDSIZE*SQSIZE,		// ширина целевого прямоугольника
		BDSIZE*SQSIZE,  	// высота целевого прямоугольника	       
		memDC,          	// дескриптор исходного DC	
		0,              	// x-коорд. левого верхнего угла исходного прямоугольника	
		0,              	// y-коорд. левого верхнего угла исходного прямоугольника	
		SRCCOPY         	// код растровой операции	
	);

	//удаление элементов
	DeleteDC(memDC);
	DeleteObject(memBM);
	ReleaseDC(hWnd, wndDC);
}

//применение изменений
void board::getChanges(game *g)
{
	//перебор шагов приращений
	//число шагов одинаково для всех движущихся квадратов
	for(int i=0; i<SQSIZE/PIXINKR; i++)
	{
		//перебор сдвигающихся квадратов и задание для них приращений координат
		for(auto &ch : g->result)
			squares[ch.row0][ch.col0]->addCoords((ch.col1 - ch.col0)*PIXINKR, (ch.row1 - ch.row0)*PIXINKR);
		
		//пауза и отрисовка с новым приращением
		Sleep(1);
		DrawInMemDC(g->side);
	}
	
	//перебор сдвигающихся квадратов
	//порядок важен! - именно как в векторе result
	for(auto &ch : g->result)
	{
		//сдвигающиеся квадраты:
		//1) скопировать указатель на новое место
		//2) на предыдущем месте обнулить указатель
		if(ch.ct == changeType::shift)
		{
			squares[ch.row1][ch.col1] = squares[ch.row0][ch.col0];
			squares[ch.row0][ch.col0] = NULL;
		}

		//складывающиеся квадраты:
		//1) удалить квадрат, который находился ближе по направлению сдвига
		//2) добавить на это место квадрат с удвоенным значением
		//3) удалить квадрат, который находился дальше по направлению сдвига
		if(ch.ct == changeType::merge)
		{
			delSq(ch.row1, ch.col1);
			addSq(ch.val << 1, ch.row1, ch.col1);
			delSq(ch.row0, ch.col0);
		}
	}
	
	//отрисовка поля вместе с квадратами
	DrawInMemDC(g->side);

	//задержка появления нового квадрата
	Sleep(100);

	//добавление нового квадрата в соответствии со структурой класса game
	addSq(g->newSq.val, g->newSq.row, g->newSq.col);

	//отрисовка поля вместе с квадратами
	DrawInMemDC(g->side);
	
	//вывод в консоль поля класса game
	g->print_board();
}