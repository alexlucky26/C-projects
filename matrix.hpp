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

	// чтобы реализовать присваивание не добавляя в мапу элементы с дефолтными значениями
	class CellProxy {
		map<pair<CustomType, CustomType>, CustomType>& matrixRef;
		pair<CustomType, CustomType> key;
		CustomType defValue;
	
	public:
		CellProxy(map<pair<CustomType, CustomType>, CustomType>& m, pair<CustomType, CustomType> k, CustomType defValue)
			: matrixRef(m), key(k), defValue(defValue) {}
	
		CellProxy& operator=(CustomType value) {
			if (value == defValue) {
				matrixRef.erase(key); // Удаляем, если присвоили defaultValue
			} else {
				matrixRef[key] = value;
			}
			return *this;
		}
	
		operator CustomType() const {
			auto it = matrixRef.find(key);
			if (it != matrixRef.end()) {
				return it->second;
			}
			return defValue;
		}
	};

	// Прокси-класс для реализации matrix[a][b]
	class RowProxy {
		map<pair<CustomType, CustomType>, CustomType>& matrixRef;
		const CustomType row;
		const CustomType defValue;

	public:
		RowProxy(map<pair<CustomType, CustomType>, CustomType>& m, const CustomType r, const CustomType defValue)
			: matrixRef(m), row(r), defValue(defValue) {}

		CellProxy operator[](CustomType col) {
			auto key = std::make_pair(row, col);
			return CellProxy(matrixRef, key, defValue);
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