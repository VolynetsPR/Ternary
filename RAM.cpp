#include "RAM.h"


RAM::RAM(size_t Size) {
	memory = new unsigned long long[Size];
	size = Size;
	setZero();
}

//RAM::~RAM() {
//	delete[] memory;
//}
// �������� ������
void RAM::setZero() {
	for (int i = 0; i < size; i++) {
		memory[i] = 0;
	}
}
//// "�����������"
//unsigned long long RAM::peek(unsigned long long const& address) {
//    unsigned long long adr = 0;
//    for (int i = 0; i < 30; i += 2) {
//        adr *= 3;
//        if (address & (2ULL << i)) {
//            adr += 2;
//            continue;
//        }
//        if (!(address & (1ULL << i))) {
//            adr += 1;
//        }
//    }
//    return memory[adr];
//}
//
//// "������"
//void RAM::poke(unsigned long long const& address, unsigned long long const& num) {
//    unsigned long long adr = 0;
//    for (int i = 0; i < 30; i += 2) {
//        adr *= 3;
//        if (address & (2ULL << i)) {
//            adr += 2;
//            continue;
//        }
//        if (!(address & (1ULL << i))) {
//            adr += 1;
//        }
//    }
//    memory[adr] = num;
//}

