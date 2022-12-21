#ifndef G2048_H_
#define G2048_H_

#include <iostream>
#include <vector>
#include <time.h>

#define SIZE 4

//Usage: changeType::shift or ...
enum class changeType {shift, merge}; 

struct change
{
	// координаты до
	int row0;
	int col0;
	// кординаты после
	int row1;
	int col1;
	// значение ячейки (до)
	unsigned int val;
	// тип изменения: changeType::shift - сдвиг, changeType::merge - слияние
	changeType ct;
};

//параметры рандомно сгенерировавшегося квадрата
struct pop
{
	int row;
	int col;
	unsigned int val;
};

class game
{
public:
	
	/////////////////////////////////////////////////////////////////////
	// это набор результатов, который будет обрабатываться интерфейсом //
	std::vector<change> result;
	pop newSq;
	int side;
	/////////////////////////////////////////////////////////////////////
	
	game();	

	//вывод таблицы в консоль
	void print_board();

	//добавление нового числа на поле
	void insert_rand_num();

	//один ход
	void move(int side);

private:

	//TODO:
	//НЕТ проверки возможности хода
	//пользовательское задание SIZE
	//BDSIZE vs SIZE ????
	
	// выбираются случайным образом 2 квадрата и заполняются числами 2 или 4 (2 и 2, 2 и 4, 4 и 2, 4 и 4)
	// при движении квадраты (каждый) двигаются до столкновения с границей или другими квадратами
	// Движение происходит по очереди, по одному квадрату
	// при столкновении 2 квадрата проверяются на равенство между собой. Если равны, то сдвиг на 1 разряд (удвоение)
	// и слияние в один квадрат (позиция соответствует крайнему квадрату, например верхнему при движении вверх
	// Другой квадрат обнуляется
	// Если не равны, то сдвигаются друг к другу без слияния, становятся соседними.
	// при любом движении один случайно выбранный незаполненный квадрат заполняется 2-кой или 4-кой ПОСЛЕ ДВИЖЕНИЯ
	
	unsigned int squares [SIZE][SIZE];
	std::vector<change> swipe_up();
	std::vector<change> swipe_down();
	std::vector<change> swipe_left();
	std::vector<change> swipe_right();
	
	//определение случайной координаты ячейки
	int get_rand_coord() {return rand() % SIZE;}

	//задание значения ячейки
	//2 или 4 с заданной вероятностью
	int put2or4() {return (rand() % 100 < 80) ? 2 : 4;}

	//проверка заполненности поля
	int check_full();

	//сложение и сдвиг ячеек
	//!!!! возможно ли оптимизировать???
	void swipe(int side);

	//сдвиг ячеек на одну клетку
	int swap(unsigned int *base, unsigned int *next);

	//сложение ячеек
	int sum(unsigned int *base, unsigned int *next);
};

#endif //G2048_H_