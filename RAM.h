#ifndef RAM_H
#define RAM_H

#include "Word.h"

struct RAM
{
	unsigned long long* memory;
	size_t size;
	RAM(size_t Size);
	//~RAM();
	//// "�����������"
	//unsigned long long peek(unsigned long long const& address);

	//// "������"
	//void poke(unsigned long long const& adr, unsigned long long const& num);

	// �������� ������
	void setZero();
};

#endif