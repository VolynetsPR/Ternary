#ifndef CPU_H
#define CPU_H

#include "Word.h"
#include "RAM.h"
#include "Trit.h"
#include <fstream>

struct CPU
{
	Word K; // (Address(8t) + Code operation(4t)), ��� � ������
	Word C; // Command Address(8t), ��� � ������

	Word S; // Sum
	Word A; // Addition

	Word M; // Multiply(���������)
	Word F; // Factor(���������)

	Word P; // Stack Pointer
	Trit W;
	//Word R; // Register for calculations

	std::ifstream input;
	std::ofstream output;

	CPU(RAM ram);

	// (M) * (F) -> (M)
	void Multiply(); 
    // "���������"
    unsigned long long peek(unsigned long long const& address, RAM const& Ram = 0);
    // "������"
	void poke(unsigned long long const& address, unsigned long long const& num, RAM const& Ram = 0);

	void FlagW(unsigned long long const& ll);

	bool execute();
};

// (X) - 1 -> (X)
void decrementAddress(unsigned long long& Cll);
// (X) + 1 -> (X)
void incrementAddress(unsigned long long& Cll);
void SumAddress(unsigned long long& a1, unsigned long long const& a2);
#endif 