#include "2048game.h"

game::game()
{
	for(int i=0; i<SIZE; i++)
		for(int j=0; j<SIZE; j++)
			squares[i][j] = 0;
}

//вывод таблицы в консоль
void game::print_board()
{
	for(int i=0; i<SIZE; i++)
	{
		for(int j=0; j<SIZE; j++)
		{
			std::cout.width(4); //задает ширину поля
			std::cout << squares[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//добавление нового числа на поле
void game::insert_rand_num()
{
	//проверка заполненности поля
	if(check_full()) return;
	
	//установка начальной точки рандомайзера
	srand(time(NULL));
	int a;
	int b;
	for(;;)
	{
		//определение случайных координат ячейки
		a = get_rand_coord();
		b = get_rand_coord();
		//задание значения ячейки 2 или 4
		if(!squares[a][b])
		{
			newSq.val = squares[a][b] = put2or4();
			newSq.row = a;
			newSq.col = b;
			
			std::cout << "Pop: " << newSq.val << "\t";
			std::cout << '(' << newSq.row << ", " << newSq.col << ')' << std::endl;
			return;
		}
	}	
}

//один ход
void game::move(int side)
{
	this->side = side;
	
	switch(side)
	{
		case 1:
		std::cout << "swipe left" << std::endl;
		result = swipe_left();
		for (auto& ch : result)
		{
			std::cout << "Change: ";
			std::cout << '(' << ch.row0 << ", " << ch.col0 << ')' << " -> ";
			std::cout << '(' << ch.row1 << ", " << ch.col1 << ')' << ", val = ";
			std::cout << ch.val << ", action = ";
			std::cout << (ch.ct == changeType::shift ? "shift" : "merge") << std::endl;
		}
		break;
		case 2:
		std::cout << "swipe right" << std::endl;
		result = swipe_right();
		for (auto& ch : result)
		{
			std::cout << "Change: ";
			std::cout << '(' << ch.row0 << ", " << ch.col0 << ')' << " -> ";
			std::cout << '(' << ch.row1 << ", " << ch.col1 << ')' << ", val = ";
			std::cout << ch.val << ", action = ";
			std::cout << (ch.ct == changeType::shift ? "shift" : "merge") << std::endl;
		}
		break;
		case 3:
		std::cout << "swipe up" << std::endl;
		result = swipe_up();
		for (auto &ch : result)
		{
			std::cout << "Change: ";
			std::cout << '(' << ch.row0 << ", " << ch.col0 << ')' << " -> ";
			std::cout << '(' << ch.row1 << ", " << ch.col1 << ')' << ", val = ";
			std::cout << ch.val << ", action = ";
			std::cout << (ch.ct == changeType::shift ? "shift" : "merge") << std::endl;
		}
		break;
		case 4:
		std::cout << "swipe down" << std::endl;
		result = swipe_down();
		for (auto& ch : result)
		{
			std::cout << "Change: ";
			std::cout << '(' << ch.row0 << ", " << ch.col0 << ')' << " -> ";
			std::cout << '(' << ch.row1 << ", " << ch.col1 << ')' << ", val = ";
			std::cout << ch.val << ", action = ";
			std::cout << (ch.ct == changeType::shift ? "shift" : "merge") << std::endl;
		}
		break;
	}
	swipe(side);
	insert_rand_num();
}

std::vector<change> game::swipe_up()
{
	std::vector<change> result;
	change ch;
	for (int col = 0; col < SIZE; col++)
	{
		// позиция текущей границы
		// изначально первый квадрат в колонке
		// движение сверху вниз, запоминаем позицию в колонке (номер ряда)
		int border_pos = 0;
		unsigned int border_val = squares[border_pos][col];
		for (int row = 1; row < SIZE; row++)
		{
			// пропускаем пустые ячейки
			if (!squares[row][col])
			{
				continue;
			}
			// обрабатываем остальные
			// 3 варианта для граничной ячейки:
			// 1) пусто - замещаем ячейку, 
			if (!border_val)
			{
				border_val = squares[row][col];
				ch = {
					// координаты до
					row, col,
					// координаты после
					border_pos, col,
					// значение
					squares[row][col],
					// тип действия
					changeType::shift
				};
				result.push_back(ch);
			}
			// 2) равна текущей - слияние, 
			else
			if (border_val == squares[row][col])
			{
				ch = {
					// координаты до
					row, col,
					// координаты после
					border_pos, col,
					// значение
					squares[row][col],
					// тип действия
					changeType::merge
				};
				result.push_back(ch);
				border_pos++;
				border_val = 0;
			}
			// 3) не равны - сдвиг границы на 1
			else
			{
				border_pos++;
				if (border_pos == SIZE - 1) break;
				//border_val = squares[border_pos][col];
				border_val = squares[row][col];
				if (border_pos == row) continue;
				ch = {
					// координаты до
					row, col,
					// координаты после
					border_pos, col,
					// значение
					squares[row][col],
					// тип действия
					changeType::shift
				};
				result.push_back(ch);
			}
		}
	}
	return result;
}

std::vector<change> game::swipe_down()
{
	std::vector<change> result;
	change ch;
	for (int col = 0; col < SIZE; col++)
	{
		// позиция текущей границы
		// изначально последний квадрат в колонке
		// движение снизу вверх, запоминаем позицию в колонке (номер ряда)
		int border_pos = SIZE - 1;
		unsigned int border_val = squares[border_pos][col];
		for (int row = SIZE - 2; row >= 0; row--)
		{
			// пропускаем пустые ячейки
			if (!squares[row][col])
			{
				continue;
			}
			// обрабатываем остальные
			// 3 варианта для граничной ячейки:
			// 1) пусто - замещаем ячейку, 
			if (!border_val)
			{
				border_val = squares[row][col];
				ch = {
					// координаты до
					row, col,
					// координаты после
					border_pos, col,
					// значение
					squares[row][col],
					// тип действия
					changeType::shift
				};
				result.push_back(ch);
			}
			// 2) равна текущей - слияние, 
			else
				if (border_val == squares[row][col])
				{
					ch = {
						// координаты до
						row, col,
						// координаты после
						border_pos, col,
						// значение
						squares[row][col],
						// тип действия
						changeType::merge
					};
					result.push_back(ch);
					border_pos--;
					border_val = 0;
				}
			// 3) не равны - сдвиг границы на 1
				else
				{
					border_pos--;
					if (border_pos == 0) break;
					//border_val = squares[border_pos][col];
					border_val = squares[row][col];
					if (border_pos == row) continue;
					ch = {
						// координаты до
						row, col,
						// координаты после
						border_pos, col,
						// значение
						squares[row][col],
						// тип действия
						changeType::shift
					};
					result.push_back(ch);
				}
		}
	}
	return result;
}

std::vector<change> game::swipe_left()
{
	std::vector<change> result;
	change ch;
	for (int row = 0; row < SIZE; row++)
	{
		// позиция текущей границы
		// изначально первый квадрат в ряду
		// движение слева направо, запоминаем позицию в ряду (номер колонки)
		int border_pos = 0;
		unsigned int border_val = squares[row][border_pos];
		for (int col = 1; col < SIZE; col++)
		{
			// пропускаем пустые ячейки
			if (!squares[row][col])
			{
				continue;
			}
			// обрабатываем остальные
			// 3 варианта для граничной ячейки:
			// 1) пусто - замещаем ячейку, 
			if (!border_val)
			{
				border_val = squares[row][col];
				ch = {
					// координаты до
					row, col,
					// координаты после
					row, border_pos,
					// значение
					squares[row][col],
					// тип действия
					changeType::shift
				};
				result.push_back(ch);
			}
			// 2) равна текущей - слияние, 
			else
				if (border_val == squares[row][col])
				{
					ch = {
						// координаты до
						row, col,
						// координаты после
						row, border_pos,
						// значение
						squares[row][col],
						// тип действия
						changeType::merge
					};
					result.push_back(ch);
					border_pos++;
					border_val = 0;
				}
			// 3) не равны - сдвиг границы на 1
				else
				{
					border_pos++;
					if (border_pos == SIZE - 1) break;
					//border_val = squares[row][border_pos];
					border_val = squares[row][col];
					if (border_pos == col) continue;
					ch = {
						// координаты до
						row, col,
						// координаты после
						row, border_pos,
						// значение
						squares[row][col],
						// тип действия
						changeType::shift
					};
					result.push_back(ch);
				}
		}
	}
	return result;
}

std::vector<change> game::swipe_right()
{
	std::vector<change> result;
	change ch;
	for (int row = 0; row < SIZE; row++)
	{
		// позиция текущей границы
		// изначально последний квадрат в ряду
		// движение справа налево, запоминаем позицию в ряду (номер колонки)
		int border_pos = SIZE - 1;
		unsigned int border_val = squares[row][border_pos];
		for (int col = SIZE - 2; col >= 0; col--)
		{
			// пропускаем пустые ячейки
			if (!squares[row][col])
			{
				continue;
			}
			// обрабатываем остальные
			// 3 варианта для граничной ячейки:
			// 1) пусто - замещаем ячейку, 
			if (!border_val)
			{
				border_val = squares[row][col];
				ch = {
					// координаты до
					row, col,
					// координаты после
					row, border_pos,
					// значение
					squares[row][col],
					// тип действия
					changeType::shift
				};
				result.push_back(ch);
			}
			// 2) равна текущей - слияние, 
			else
				if (border_val == squares[row][col])
				{
					ch = {
						// координаты до
						row, col,
						// координаты после
						row, border_pos,
						// значение
						squares[row][col],
						// тип действия
						changeType::merge
					};
					result.push_back(ch);
					border_pos--;
					border_val = 0;
				}
			// 3) не равны - сдвиг границы на 1
				else
				{
					border_pos--;
					if (border_pos == 0) break;
					//border_val = squares[row][border_pos];
					border_val = squares[row][col];
					if (border_pos == col) continue;
					ch = {
						// координаты до
						row, col,
						// координаты после
						row, border_pos,
						// значение
						squares[row][col],
						// тип действия
						changeType::shift
					};
					result.push_back(ch);
				}
		}
	}
	return result;
}

//проверка заполненности поля
int game::check_full()
{
	for(int i=0; i<SIZE; i++)
		for(int j=0; j<SIZE; j++)
			if(!squares[i][j]) return 0;
	return 1;
}

//сложение и сдвиг ячеек
void game::swipe(int side)//!!!! возможно ли оптимизировать???
{
	int a0;
	int b0;
	int ia;
	int ib;
	int a;
	int b;
	//перебор строк при сдвиге влево/вправо
	//     столбцов при сдвиге вверх/вниз
	for(int i=0; i<SIZE; i++)
	{
		//определение индексов ячеек для соответствующего направления сдвига
		switch(side)
		{
			case 1: //left
				a0 = i;
				b0 = 0;
				ia = 0;
				ib = 1;
				break;
			case 2: //right
				a0 = i;
				b0 = SIZE-1;
				ia = 0;
				ib = -1;
				break;
			case 3: //up
				a0 = 0;
				b0 = i;
				ia = 1;
				ib = 0;
				break;
			case 4: //down
				a0 = SIZE-1;
				b0 = i;
				ia = -1;
				ib = 0;
				break;
		}
		unsigned int *base;
		unsigned int *next;
		
		//последовательное сложение ячеек текущей  строки  при сдвиге влево/вправо
		//                          ячеек текущего столбца при сдвиге вверх/вниз
		a = a0;
		b = b0;
		//крайняя ячейка - для проверки выхода за границу поля
		unsigned int *last = &squares[a0+(SIZE-1)*ia][b0+(SIZE-1)*ib];
		for(int j=0; j<SIZE-1; j++)
		{
			//текущая ячейка
			base = &squares[a][b];
			if(*base)
				for(int k=1;;k++)
				{
					//следующая ячейка
					next = &squares[a+k*ia][b+k*ib];
					//если следующая ячейка заполнена и не равна текущей
					if(*next && *next != *base) break;
					//если текущая и следующая ячейки равны, то складываем их
					if(sum(base, next)) break;
					//если достигли края поля
					if(next == last) break;
				}
			a += ia;
			b += ib;
		}
		//сдвиг ячеек в соответствующую сторону
		
		int flag = 0;
		do
		{
			a = a0;
			b = b0;
			flag = 0;
			//сдвиг ячеек на одну клетку
			for(int j=0; j<SIZE-1; j++)
			{
				flag += swap(&squares[a][b], &squares[a+ia][b+ib]);
				a += ia;
				b += ib;
			}
		}
		while(flag); //flag = 0, если уже некуда сдвигать
	}
}

//сдвиг ячеек на одну клетку
int game::swap(unsigned int *base, unsigned int *next)
{
	if(!*base && *next)
	{
		*base = *next;
		*next = 0;
		return 1;
	}
	return 0;
}

//сложение ячеек
int game::sum(unsigned int *base, unsigned int *next)
{
	//std::cout << *base << "+" << *next << std::endl;
	//если ячейки равны, то ячейка со стороны сдвига прибавляет себе сдвигаемую ячейку,
	//которая в свою очередь обнуляется
	if(*base == *next)
	{
		*base += *next;
		*next = 0;
		return 1;
	}
	return 0;
}