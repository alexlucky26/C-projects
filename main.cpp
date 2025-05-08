#include "matrix.hpp"

int main(int, char **) {
	Matrix<int, 0> matrix;
	
	{
		/* 1. При запуске программы необходимо создать матрицу с пустым значением 0, заполнить главную диагональ матрицы (от [0,0] до [9,9]) значениями от 0 до 9.
		2. Второстепенную диагональ (от [0,9] до [9,0]) значениями от 9 до 0."*/
		const size_t minValue = 0;
		const size_t maxValue = 9;
		size_t rIndex = maxValue;
		for (size_t index = minValue; index <= maxValue; ++index)
		{
			matrix[index][index] = index; // от [0,0] до [9,9] зачения от 0 до 9
			matrix[index][rIndex] = rIndex; // от [0,9] до [9,0] зачения от 9 до 0
			--rIndex; 
		}
		
		// Необходимо вывести фрагмент матрицы от [1,1] до [8,8]. Между столбцами пробел. Каждая строка матрицы на новой строке консоли.
		std::cout << "The fragment from [1.1] to [8.8]: " << endl; 
		for (size_t firstIndex = minValue + 1; firstIndex < maxValue; ++firstIndex)
		{
			for (size_t secondIndex = minValue + 1; secondIndex < maxValue; ++secondIndex)
			{
				std::cout << matrix[firstIndex][secondIndex] << " ";
			}
			std::cout << endl;
		}
	}

	std::cout << "Amount of used elements (there are not default):" << endl; 
	// Вывести количество занятых ячеек.
	std::cout << matrix.size() << endl;

	// Вывести все занятые ячейки вместе со своими позициями.
	std::cout << "All used elements:" << endl; 
	for(auto c: matrix)
	{
		int x = c.first.first;
		int y = c.first.second;
		int v = c.second;

		std::cout << "[" << x << "," << y << "] = " << v << "; ";
	}
	std::cout << endl;

	// Опционально реализовать каноническую форму оператора =, допускающую выражения 
	((matrix[100][100] = 314) = 0) = 217;
	std::cout << "The sentense: ((matrix[100][100] = 314) = 0) = 217 is " << matrix[100][100] << endl;

	return 0;
}
