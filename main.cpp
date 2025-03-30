#include "allocator.h"
#include <iostream>
#include <map>

using namespace std;

int main(int, char **) {
	map<int, int, less<int>, custom_allocator<pair<const int, int>>> map;
	map[0] = 0;
	int mul = 1;
	for (size_t key = 1; key < MAX_ELEMS; ++key) {
		map[key] = mul *= key;
		cout << key << " " << map[key] << "\n";
	}
	cout << "-----------------\n";

    custom_vector<int> default_user_vector(MAX_ELEMS);
    for (size_t vecIdx = 0; vecIdx < MAX_ELEMS; ++vecIdx) {
        default_user_vector.push_back(vecIdx);
    }
    
    int* elem = default_user_vector.GetFirst();
    while (elem != nullptr) {
        std::cout << *elem << " ";
        elem = default_user_vector.GetNext(elem);
    }
    std::cout << "\n-----------------\n";

    custom_vector<int, custom_allocator<int>> user_vector(MAX_ELEMS);
    for (size_t vecIdx = 0; vecIdx < MAX_ELEMS; ++vecIdx) {
        user_vector.push_back(vecIdx);
    }
    
    int* user_elem = user_vector.GetFirst();
    while (user_elem != nullptr) {
        std::cout << *user_elem << " ";
        user_elem = user_vector.GetNext(user_elem);
    }
    std::cout << "\n-----------------\n";

	return 0;
}