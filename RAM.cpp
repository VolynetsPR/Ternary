#include "RAM.h"


RAM::RAM(size_t Size) {
	memory = new unsigned long long[Size];
	size = Size;
	setZero();
}

//RAM::~RAM() {
//	delete[] memory;
//}
// Обнулить память
void RAM::setZero() {
	for (int i = 0; i < size; i++) {
		memory[i] = 0;
	}
}