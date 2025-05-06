#include "matrix.hpp"

int main(int, char **) {
	Matrix<int, 0> matrix;
	
	// заполнение основной и побочной диагоналей
	{
		const size_t minValue = 0;
		const size_t maxValue = 9;
		size_t rIndex = maxValue;
		for (size_t index = minValue; index <= maxValue; ++index)
		{
			matrix[index][index] = index;
			matrix[index][rIndex] = rIndex;
			--rIndex; 
		}

		for (size_t firstIndex = minValue + 1; firstIndex < maxValue; ++firstIndex)
		{
			for (size_t secondIndex = minValue + 1; secondIndex < maxValue; ++secondIndex)
			{
				std::cout << matrix[firstIndex][secondIndex] << " ";
			}
			std::cout << endl;
		}
	}

	std::cout << matrix.size() << endl; // сайз вообще всех ячеек
	std::cout << matrix.used_size() << endl; // сайз только заполненных кастомными значениями

	for(auto c: matrix)
	{
		int x = c.first.first;
		int y = c.first.second;
		int v = c.second;

		std::cout << "[" << x << "," << y << "] = " << v << "; ";
	}
	std::cout << endl;

	((matrix[100][100] = 314) = 0) = 217;
	std::cout << "The sentense: ((matrix[100][100] = 314) = 0) = 217 is " << matrix[100][100] << endl;

	return 0;
}
