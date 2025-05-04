#include <iostream>
#include <type_traits>
#include <map>
#include <algorithm>

using namespace std;

template<typename CustomType, CustomType defaultValue, enable_if_t<is_integral_v<CustomType>, bool> = true>
class Matrix {
	map<pair<CustomType, CustomType>, CustomType> matrix; // эмуляция матрицы
public:
	// возвращает количество всех существующих ячеек
	size_t size() {
		return matrix.size();
	};

    /// @brief функция, возвращающая количество ячеек, которые заполнены НЕ ДЕФОЛТНЫМИ значениями
    /// @return 
    size_t used_size() {
        return std::count_if(matrix.begin(), matrix.end(), [&](pair<pair<CustomType, CustomType>, CustomType> elem) { return elem.second != defaultValue; });
    }

	// Прокси-класс для реализации matrix[a][b]
	class RowProxy {
		map<pair<CustomType, CustomType>, CustomType>& matrixRef;
		const CustomType row;
		const CustomType defValue;

	public:
		RowProxy(map<pair<CustomType, CustomType>, CustomType>& m, const CustomType r, const CustomType defValue)
			: matrixRef(m), row(r), defValue(defValue) {}

        CustomType& operator[](CustomType col) {
            auto key = std::make_pair(row, col);
            if (matrixRef.find(key) == matrixRef.end())
                matrixRef[key] = defaultValue;
            return matrixRef[key];
        }
	};

	RowProxy operator[](CustomType row) {
		return RowProxy(matrix, row, defaultValue);
	}
	
	typename map<pair<CustomType, CustomType>, CustomType>::iterator begin() {
		return matrix.begin();
	}

	typename map<pair<CustomType, CustomType>, CustomType>::iterator end() {
		return matrix.end();
	}
};