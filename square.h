#ifndef SQUARE_H_
#define SQUARE_H_

#include <windows.h>

#define SQSIZE 64	//длина стороны квадрата в пикселях
#define SQSPACE 2	//отступ стороны квадрата от линий сетки в пикселях

//квадрат
class square
{
public:
	
	square(unsigned int val, int row, int col);

	//отрисовка квадрата в заданном контексте устройства
	void draw(HDC workDC);

	//приращение координат при анимации движения
	void addCoords(int dx, int dy);

	~square();

private:
	
	HBITMAP hBmp;
	const unsigned int val;
	int x;
	int y;
};

#endif //SQUARE_H_