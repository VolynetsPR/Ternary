#ifndef RAM_H
#define RAM_H

#include "Word.h"

struct RAM
{
	unsigned long long* memory;
	size_t size;
	RAM(size_t Size);
	//~RAM();
	//// "подсмотреть"
	//unsigned long long peek(unsigned long long const& address);

	//// "ткнуть"
	//void poke(unsigned long long const& adr, unsigned long long const& num);

	// Обнулить память
	void setZero();
};

#endif