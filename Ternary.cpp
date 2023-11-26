#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string.h>
#include <vector>

#include "Trit.h"
#include "CPU.h"
#include "RAM.h"
#include "Word.h"

//#pragma optimize
using namespace std;

ostream& operator << (ostream& out, Trit const& t) {
    if (t.b1 == 1) {
        out << '+';
    }
    else if (t.b2 == 1) {
        out << '-';
    }
    else {
        out << '0';
    }
    return out;
}

ostream& operator << (ostream& out, Word const& w) {
    for (int i = 62; i >= 0; i -= 2) {
        if (w.ll & (2ULL << i)) {
            out << '+';
        }
        else if (w.ll & (1ULL << i)) {
            out << '-';
        }
        else {
            out << '0';
        }
    }
    return out;
}
// adr size = 8, code size = 4
unsigned long long createCommand(char const* adr, char const* code) { 
    unsigned long long command = 0;
    for (int i = 0; i < 8; i++) {
        command <<= 2;
        if (adr[i] == '+') {
            command += 2;
        }
        else if (adr[i] == '-') {
            command += 1;
        }
    }
    for (int i = 0; i < 4; i++) {
        command <<= 2;
        if (code[i] == '+') {
            command += 2;
        }
        else if (code[i] == '-') {
            command += 1;
        }
    }
    return command << 40;
}
// adr size = 8, code size = 4
unsigned long long createCommand(unsigned long long adr, char const* code) {
    int x = 46;
    for (int i = 0; i < 4; i++) {
        if (code[i] == '+') {
            adr |= (2ULL << x);
            adr &= ~(1ULL << x);
        }
        else if (code[i] == '-') {
            adr &= ~(2ULL << x);
            adr |= (1ULL << x);
        }
        else {
            adr &= ~(2ULL << x);
            adr &= ~(1ULL << x);
        }
        x -= 2;
    }
    return adr;
}
// adr size = 8t
unsigned long long createAddress(char const* adr) {
    unsigned long long address = 0;
    for (int i = 0; i < 8; i++) {
        address <<= 2;
        if (adr[i] == '+') {
            address += 2;
        }
        else if (adr[i] == '-') {
            address += 1;
        }
    }
    return address << 48;
}

// - = 0   0 = 1   + = 2
// 0 = --------
// 1 = -------0
// 2 = -------+
// 3 = ------0-
unsigned long long createAddress(unsigned long long ll) {
    unsigned long long ans = 0;
    short ll3 = 0;
    for (int i = 48; i < 64; i += 2) { // 48 50 52 54 56 58 60 62
        ll3 = ll % 3;
        ll /= 3;
        if (ll3 == 2) {
            ans += 2ULL << i;
        }
        else if (ll3 == 0) {
            ans += 1ULL << i;
        }
    }
    return ans;
}

unsigned long long createNumber(char const* num) {
    unsigned long long ans = 0;
    size_t i = 0;
    while (num[i] != '\0') {
        ans <<= 2;
        if (num[i] == '+') {
            ans += 2;
        }
        else if (num[i] == '-') {
            ans += 1;
        }
        ++i;
    }
    return ans;
}

//void AssemblerCompiler(char const* program, RAM ram, CPU cpu) {
//    char command[4] = "+++";
//    char address[9] = "++++++++";
//    char num[33] = "00000000000000000000000000000000";
//    size_t i = 0;
//    size_t ramAdr = 0;
//    while (program[i] != '\0') {
//        command[0] = program[i];
//        command[1] = program[i + 1];
//        command[2] = program[i + 2];
//        //command[4] = '\0';
//        std::cout << command << '\n';
//        i += 4;
//        if (strcmp(command, "set") == 0) {
//            int j;
//            for (j = 0; j < 8; ++j) {
//                address[j] = program[i];
//                ++i;
//            }
//            //std::cout << address << '\n';
//            j = 31;
//            for (int x = 0; x < 32; x++) {
//                num[x] = '0';
//            }
//            while (program[i] != '\n') { ++i; }
//            int end = i;
//            while (program[--i] != ' ') {
//                num[j--] = program[i];
//            }
//            i = end + 1;
//            //if (i == 0) std::cout << i << '\n';
//            //std::cout << num << '\n';
//            std::cout << "num - " << num << '\n';
//            cpu.poke(createAddress(address), createNumber(num));
//        }
//        else if (strcmp(command, "snd") == 0) {
//            for (int j = 3; j < 11; ++j) {
//                address[j - 3] = program[i + j];
//            }
//            std::cout << "adr - " << address << '\n';
//            switch (program[i]) {
//            case 'S':
//                ram.memory[ramAdr++] = createCommand(address, "++++");                                    // ++++     (Adr) -> (S)
//                break;
//            case 'A':
//                ram.memory[ramAdr++] = createCommand(address, "+++0");                                    // +++0     (Adr) -> (A)
//                break;
//            case 'C':
//                ram.memory[ramAdr++] = createCommand(address, "+00+");                                    // +00+     (Adr) -> (C), безусловный переход
//                ramAdr = 0;
//                for (int i = 0; i < 8; i++) {
//                    std::cout << ramAdr << '\n';
//                    ramAdr *= 3;
//                    if (address[i] == '+') {
//                        ramAdr += 2;
//                        continue;
//                    }
//                    if (address[i] == '0') {
//                        ramAdr += 1;
//                    }
//                }
//                std::cout << 'r' << ramAdr << '\n';
//                break;
//            }
//            i += 12;
//        }
//        else if (strcmp(command, "mov") == 0) {
//            for (int j = 3; j < 11; ++j) {
//                address[j - 3] = program[i + j];
//            }
//            switch (program[i]) {
//            case 'S':
//                ram.memory[ramAdr++] = createCommand(address, "++-0");                                    // ++-0     (S) -> (Adr)
//                break;
//            case 'A':
//                ram.memory[ramAdr++] = createCommand(address, "++--");                                    // ++--     (A) -> (Adr)
//                break;
//            }
//            i += 12;
//        }
//        else if (strcmp(command, "sum") == 0) {
//            ram.memory[ramAdr++] = 0b10'10'10'01'0000000000'0000000000'0000000000'0000000000ULL;           // +++-     (S) + (A) -> (S)
//        }
//        else if (strcmp(command, "sub") == 0) {
//            if (program[i] == 'S') {
//                ram.memory[ramAdr++] = 0b10'10'00'10'0000000000'0000000000'0000000000'0000000000ULL;       // ++0+     (S) - (A) -> (S)
//                i += 2;
//            }
//            else {
//                ram.memory[ramAdr++] = 0b10'10'00'00'0000000000'0000000000'0000000000'0000000000ULL;       // ++00     -(S) + (A) -> (S)
//                i += 2;
//            }
//        }
//        else if (strcmp(command, "inv") == 0) {
//            if (program[i] == 'S') {
//                ram.memory[ramAdr++] = 0b10'10'00'01'0000000000'0000000000'0000000000'0000000000ULL;       // ++0-     -(S) -> (S)
//            }
//            else {
//                ram.memory[ramAdr++] = 0b10'10'01'10'0000000000'0000000000'0000000000'0000000000ULL;       // ++-+     -(A) -> (A)
//            }
//            i += 2;
//        }
//    }
//}

// 1 3 9 27 81   1 4 13 40 121  0 1 2 3 4          0 + 1 + 2 + 3 = (0 + 1) + 2
// 5 9
//unsigned long long tenToTernary(long long ten) {
//    int i = 1;
//
//}

#include <map>

void AssemblerCompiler(vector<string> program, RAM& ram, CPU& cpu) {
    map<string, unsigned long long> glb;        // имя глобальной переменной - адрес
    map<string, unsigned long long> labels;     // имя функции - адрес
    map <unsigned long long, string> posFunc;   // место вызова функции(ramAdr) - имя функции
    map<string, unsigned long long> var;        // переменная - смещение
    //char lblSize = 0;
    char command[4] = "+++";        // название операции
    char address[9] = "++++++++";   // адрес для команд с троичным адресом
    char num[33] = "00000000000000000000000000000000";  // число для команды с троичным число(set, var, glb и т.д.)
    size_t i = 0;
    int ramAdr = 0;
    while (program[i][0] != '\0' && program[i][0] != ' ') {
        command[0] = program[i][0];
        command[1] = program[i][1];
        command[2] = program[i][2];
        //command[4] = '\0';
        std::cout << command << '\n';
        if (strcmp(command, "set") == 0) {  // установить число в оперативную память
            int j;
            for (j = 0; j < 8; ++j) {
                address[j] = program[i][j + 4];
            }
            //std::cout << address << '\n';
            for (int x = 0; x < 32; x++) {
                num[x] = '0'; 
            }
            j = 31;
            int end = program[i].size();
            while (program[i][--end] != ' ') {
                num[j--] = program[i][end];
            }

            std::cout << "adr - " << address << "  num - " << num << '\n';
            cpu.poke(createAddress(address), createNumber(num));
        }
        else if (strcmp(command, "glb") == 0) {
            int sp = 4;
            while (program[i][sp] != ',') { ++sp; }
            string name = program[i].substr(4, sp - 4);
            glb[name] = cpu.P.ll;
            std::cout << "name - " << name << ' ';

            for (int x = 0; x < 32; x++) {
                num[x] = '0';
            }

            int j = 31;
            int end = program[i].size();
            while (program[i][--end] != ' ') {
                num[j--] = program[i][end];
            }
            std::cout << "num - " << num << '\n';
            cpu.poke(cpu.P.ll, createNumber(num));
            std::cout << Word(cpu.peek(cpu.P.ll)) << '\n';
            decrementAddress(cpu.P.ll);
        }
        else if (strcmp(command, "var") == 0) { // определение переменной
            int sp = 4;
            while (program[i][sp] != ',') { ++sp; }
            string name = program[i].substr(4, sp - 4);
            var[name] = 0;

            std::cout << "name shift\n";
            for (auto& [n, sh] : var) {
                std::cout << n << ' ' << Word(sh) << " ; ";
                incrementAddress(sh);
                std::cout << n << ' ' << Word(var[n]) << '\n';
            }

            for (int x = 0; x < 32; x++) {
                num[x] = '0';
            }

            int j = 31;
            int end = program[i].size();
            while (program[i][--end] != ' ') {
                num[j--] = program[i][end];
            }
            std::cout << "num - " << num << '\n';
            cpu.poke(cpu.P.ll, createNumber(num));

            ram.memory[ramAdr++] = createCommand(cpu.P.ll, "+-0+"); // +-0+     (Adr) -> {P}, переменную по адресу положить на вершину стека(нужно для функций/рекурсии)
            decrementAddress(cpu.P.ll);
        }
        else if (strcmp(command, "del") == 0) { // удаляет переменную со стека(сдвигает указатель на вершину стека)
            ram.memory[ramAdr++] = 0b10'01'10'00'0000000000'0000000000'0000000000'0000000000ULL; // +-+0     (P) + 1 -> (P), инкримент P
        }
        else if (strcmp(command, "snd") == 0) { // из оперативной памяти в регистр    snd ?, 0-+t/var
            if (program[i][program[i].size() - 1] == 't') {
                for (int j = 0; j < 8; ++j) {
                    address[j] = program[i][j + 7];
                }
            }
            else {
                string nam = program[i].substr(7,string::npos);
                std::cout << "name - (" << nam << ")\n";
                if (var.contains(nam)) {
                    std::cout << "lcl ";
                    unsigned long long ull = var[nam];
                    std::cout << Word(ull) << '\n';
                    switch (program[i][4]) {
                    case 'S':
                        ram.memory[ramAdr++] = createCommand(ull, "+-00");                              // +-00     {(P) + Adr} -> (S)
                        break;
                    case 'A':
                        ram.memory[ramAdr++] = createCommand(ull, "+--+");                              // +--+     {(P) + Adr} -> (A)
                        break;
                    case 'M':
                        ram.memory[ramAdr++] = createCommand(ull, "+---");                              // +---     {(P) + Adr} -> (M)
                        break;
                    case 'F':
                        ram.memory[ramAdr++] = createCommand(ull, "0++0");                              // 0++0     {(P) + Adr} -> (F)
                        break;
                    }
                    i += 1;
                    continue;
                }
                else {
                    std::cout << "glb ";
                    unsigned long long ull = glb[nam];
                    std::cout << Word(ull) << '\n';
                    switch (program[i][4]) {
                    case 'S':
                        ram.memory[ramAdr++] = createCommand(ull, "++++");                              // ++++     (Adr) -> (S)
                        break;
                    case 'A':
                        ram.memory[ramAdr++] = createCommand(ull, "+++0");                              // +++0     (Adr) -> (A)
                        break;
                    case 'M':
                        ram.memory[ramAdr++] = createCommand(ull, "+00-");                              // +00-     (Adr) -> (M)
                        break;
                    case 'F':
                        ram.memory[ramAdr++] = createCommand(ull, "+0-+");                              // +0-+     (Adr) -> (F)
                        break;
                    }
                    i += 1;
                    continue;
                }

            }
            std::cout << program[i][4] << " adr - " << address << '\n';

            switch (program[i][4]) {
            case 'S':
                ram.memory[ramAdr++] = createCommand(address, "++++");                                    // ++++     (Adr) -> (S)
                break;
            case 'A':
                ram.memory[ramAdr++] = createCommand(address, "+++0");                                    // +++0     (Adr) -> (A)
                break;
            case 'C':
                ram.memory[ramAdr++] = createCommand(address, "+00+");                                    // +00+     (Adr) -> (C), безусловный переход
                break;
            case 'M':
                ram.memory[ramAdr++] = createCommand(address, "+00-");                                    // +00-     (Adr) -> (M)
                break;
            case 'F':
                ram.memory[ramAdr++] = createCommand(address, "+0-+");                                    // +0-+     (Adr) -> (F)
                break;
            }
        }
        else if (strcmp(command, "mov") == 0) { // из регистра в оперативную память
            if (program[i][program[i].size() - 1] == 't') {
                for (int j = 0; j < 8; ++j) {
                    address[j] = program[i][j + 7];
                }
                switch (program[i][4]) {
                case 'S':
                    ram.memory[ramAdr++] = createCommand(address, "++-0");  // ++-0     (S) -> (Adr)
                    break;
                case 'A':
                    ram.memory[ramAdr++] = createCommand(address, "++--");  // ++--     (A) -> (Adr)
                    break;
                case 'M':
                    ram.memory[ramAdr++] = createCommand(address, "0+0+");  // 0+0+     (M) -> (Adr)
                    break;
                case 'F':
                    ram.memory[ramAdr++] = createCommand(address, "0+00");  // 0+00     (F) -> (Adr)
                    break;
                }
            }
            else {
                string nam = program[i].substr(7, string::npos);
                std::cout << "name - " << nam << '\n';
                if (var.contains(nam)) {
                    std::cout << "lcl ";
                    unsigned long long ull = var[nam];
                    std::cout << Word(ull) << '\n';
                    switch (program[i][4]) {
                    case 'S':
                        ram.memory[ramAdr++] = createCommand(ull, "+-0-");                              // +-0-     (S) -> {(P) + Adr}
                        break;
                    case 'A':
                        ram.memory[ramAdr++] = createCommand(ull, "+--0");                              // +--0     (A) -> {(P) + Adr}
                        break;
                    case 'M':
                        ram.memory[ramAdr++] = createCommand(ull, "0+++");                              // 0+++     (M) -> {(P) + Adr}
                        break;
                    case 'F':
                        ram.memory[ramAdr++] = createCommand(ull, "0++-");                              // 0++-     (F) -> {(P) + Adr}
                        break;
                    }
                    i += 1;
                    continue;
                }
                else {
                    std::cout << "glb ";
                    unsigned long long ull = glb[nam];
                    std::cout << Word(ull) << '\n';
                    switch (program[i][4]) {
                    case 'S':
                        ram.memory[ramAdr++] = createCommand(ull, "++-0");                              // ++-0     (S) -> (Adr)
                        break;
                    case 'A':
                        ram.memory[ramAdr++] = createCommand(ull, "++--");                              // ++--     (A) -> (Adr)
                        break;
                    case 'M':
                        ram.memory[ramAdr++] = createCommand(ull, "0+0+");                              // 0+0+     (M) -> (Adr)
                        break;
                    case 'F':
                        ram.memory[ramAdr++] = createCommand(ull, "0+00 ");                              // 0+00     (F) -> (Adr)
                        break;
                    }
                    i += 1;
                    continue;
                }
            }
        }
        else if (strcmp(command, "swp") == 0) {
            ram.memory[ramAdr++] = 0b00'10'01'00'0000000000'0000000000'0000000000'0000000000ULL;   // 0+-0     (S) <-> (A)
        }
        else if (strcmp(command, "sum") == 0) { // сложить
            ram.memory[ramAdr++] = 0b10'10'10'01'0000000000'0000000000'0000000000'0000000000ULL;           // +++-     (S) + (A) -> (S)
        }
        else if (strcmp(command, "sub") == 0) { // вычесть
            std::cout << program[i][4] << '\n';
            if (program[i][4] == 'S') {
                ram.memory[ramAdr++] = 0b10'10'00'10'0000000000'0000000000'0000000000'0000000000ULL;       // ++0+     (S) - (A) -> (S)
            }
            else {
                ram.memory[ramAdr++] = 0b10'10'00'00'0000000000'0000000000'0000000000'0000000000ULL;       // ++00     -(S) + (A) -> (S)
            }
        }
        else if (strcmp(command, "inv") == 0) { // инвертировать
            std::cout << program[i][4] << '\n';
            if (program[i][4] == 'S') {
                ram.memory[ramAdr++] = 0b10'10'00'01'0000000000'0000000000'0000000000'0000000000ULL;       // ++0-     -(S) -> (S)
            }
            else {
                ram.memory[ramAdr++] = 0b10'10'01'10'0000000000'0000000000'0000000000'0000000000ULL;       // ++-+     -(A) -> (A)
            }
        }
        else if (strcmp(command, "mul") == 0) { // умножить
            ram.memory[ramAdr++] = 0b10'00'01'00'0000000000'0000000000'0000000000'0000000000ULL;           // +0-0     (M) * (F) -> (M)
        }
        else if (strcmp(command, "end") == 0) { // завершить выполнение программы
            ram.memory[ramAdr++] = 0ULL; 
        }
        else if (strcmp(command, "inp") == 0) {
            if (program[i][program[i].size() - 1] == 't') {
                int j;
                for (j = 0; j < 8; ++j) {
                    address[j] = program[i][j + 4];
                }
                std::cout << address << '\n';
                ram.memory[ramAdr++] = createCommand(address, "0+0-");    // 0+0-  "input.txt" -> (Adr)
            }
            else {
                string nam = program[i].substr(4, string::npos);
                std::cout << "name - (" << nam << ")\n";
                if (var.contains(nam)) {
                    std::cout << "lcl ";
                    unsigned long long ull = var[nam];
                    std::cout << Word(ull) << '\n';
                    ram.memory[ramAdr++] = createCommand(ull, "0+0-");    // 00++     "input.txt" -> {(P) + Adr}
                }
                else {
                    std::cout << "glb ";
                    unsigned long long ull = glb[nam];
                    std::cout << Word(ull) << '\n';
                    ram.memory[ramAdr++] = createCommand(ull, "0+0-");    // 0+0-  "input.txt" -> (Adr)
                }
            }

        }
        else if (strcmp(command, "jmp") == 0) { // условный переход
            std::cout << program[i][4] << '\n';
            string l = program[i].substr(7, program[i].size());
            if (program[i][4] == '1' || program[i][4] == '+') {
                ram.memory[ramAdr] = 0b10'00'10'10'0000000000'0000000000'0000000000'0000000000ULL;   // +0++  (W == +) ? Adr -> (C), условный переход 1
            }
            else if (program[i][4] == '0') {
                ram.memory[ramAdr] = 0b10'00'10'00'0000000000'0000000000'0000000000'0000000000ULL;   // +0+0  (W == 0) ? Adr -> (C), условный переход 0
            }
            else if(program[i][4] == '-'){
                ram.memory[ramAdr] = 0b10'00'10'01'0000000000'0000000000'0000000000'0000000000ULL;   // +0+-  (W == -) ? Adr -> (C), условный переход -1
            }
            else {
                ram.memory[ramAdr] = 0b10'00'00'10'0000000000'0000000000'0000000000'0000000000ULL;// +00+ Adr -> (C), безусловный переход
                l = program[i].substr(4, program[i].size());
            }
            std::cout << "lbl - (" << l << ')' << '\n';
            posFunc[ramAdr++] = l;
        } 
        else if (strcmp(command, "cll") == 0) { // вызов функции
            string s;
            int l = 4;
            while (program[i][++l] != '(') {}
            s = program[i].substr(4, l - 4); // имя функции
            std::cout << '(' << s << ')' << ' ';
            ram.memory[ramAdr] = 0b10'01'10'10'0000000000'0000000000'0000000000'0000000000ULL;        // +-++     (C) -> {P}, адрес возврата
            int Cadr = ramAdr++;
            incrementAddress(ram.memory[Cadr]);
            unsigned long long lclValue = 0;
            unsigned long long plus = 0; // 0
            if (program[i][++l] != ')') {
                int r = l, size = program[i].size() - 1;
                while (r < size) {
                    while (program[i][++r] != ')' && program[i][r] != ',') {}
                    string num = program[i].substr(l, r - l);
                    std::cout << "par - " << num << '\n';
                    if (program[i][r - 1] == 't') {
                        num = num.substr(0, num.size() - 1);
                        cpu.poke(cpu.P.ll, createNumber(num.c_str()));
                        ram.memory[ramAdr++] = createCommand(cpu.P.ll, "+-0+"); // +-0+     (Adr) -> {P}, переменную по адресу положить на вершину стека(нужно для функций/рекурсии)
                        decrementAddress(cpu.P.ll);
                    }
                    else {
                        if (var.contains(num)) {
                            std::cout << "lcl\n";
                            lclValue = var[num];
                            incrementAddress(plus);
                            SumAddress(lclValue, plus);
                            std::cout << "plus - " << Word(plus) << " sh - " << Word(lclValue) << '\n';
                            ram.memory[ramAdr++] = createCommand(lclValue, "0+--");  // 0+--     {(P) + Adr} -> {P}
                        }
                        else {
                            std::cout << "glb\n";
                            ram.memory[ramAdr++] = createCommand(glb[num], "+-0+");  // +-0+     (Adr) -> {P}, переменную по адресу положить на вершину стека(нужно для функций/рекурсии)
                        }
                    }
                    incrementAddress(ram.memory[Cadr]);
                    l = ++(++r);
                }
            }
            ram.memory[ramAdr] = 0b10'00'00'10'0000000000'0000000000'0000000000'0000000000ULL;          // +00+ Adr -> (C), безусловный переход
            if (labels.contains(s)) {
                ram.memory[ramAdr++] += labels[s]; // +00+ Adr -> (C), безусловный переход
                std::cout << "adr - " << Word(labels[s]) << '\n';
            }
            else {
                posFunc[ramAdr++] = s;
                std::cout << "Radr - " << ramAdr - 1 << '\n';
            }
        }
        else if (strcmp(command, "ret") == 0) { // возвращение из функции
            ram.memory[ramAdr++] = 0b10'00'01'01'0000000000'0000000000'0000000000'0000000000ULL;       // +0--     {P} -> (C)
        }
        else if (strcmp(command, "lbl") == 0) { // метка(функция)
            string s;
            int l = 4;
            while (program[i][++l] != '(') {}
            s = program[i].substr(4, l - 4); // имя функции
            std::cout << '(' << s << ')' << ' ';
            labels[s] = createAddress(ramAdr);
            //while (program[i][++end] != '\n') {
            //    labels[lblSize][++j] = program[i][end];
            //    labels[lblSize][j + 1] = '\0';
            //    std::cout << "lbl - " << end << ' ' << j << '(' << program[i][end]  << ')' << labels[lblSize] << '\n';
            //}
            std::cout << "adr - " << Word(labels[s]) << '\n';

            if (program[i][++l] == ')') {
                i++;
                continue;
            }
            int r = l, size = program[i].size() - 1;
            while (r < size) {
                while (program[i][++r] != ')' && program[i][r] != ',') {}
                string name = program[i].substr(l, r - l);
                std::cout << "par1 - " << name << '\n';
                var[name] = 0;
                for (auto& [n, sh] : var) {
                    incrementAddress(sh);
                }

                l = ++(++r);
            }
            for (auto& [n, sh] : var) {
                std::cout << n << " - " << Word(sh) << '\n';
            }
        }
        else if (strcmp(command, "out") == 0) {

        }
        else if (strcmp(command, "hel") == 0) { // help
            setlocale(LC_ALL, "");
            
            std::cout << "(adr) - адрес, должен содержать 8 тритов. Например: +-0-+0+0" ;
            std::cout << "set - записывает число в указанную ячейку оперативной памяти\n\t\
Синтаксис: set (adr), (value)\n";
            std::cout << "glb - создает глобальную переменную, с указанным именем, инициализируя ее указанным значением\n\t\
Синтаксис: glb name, (value)t\n";
            std::cout << "var - создаёт локальную переменную(кладет число на вершину стека)\
Синтаксис: var name, (value)t\n"; // определение переменной
            std::cout << "del - удаляет последнее число с вершины стека(уменьшает размер стека передвигая указатель на его стека). Все используемые переменные, в том \
числе параметры нужно удалять со стека и лучше это делать прямо перед завершением работы функции\n\t\
Синтаксис: del\n";
            std::cout << "snd - посылает значение из ячейки оперативной памяти в регистр\
Синтаксис: snd R, (adr)\n \
\tsnd R, var\n";
            std::cout << "mov - посылает значение из указанного регистра, в ячейку оперативной памяти или записывает его в переменную\n\t\
Синтаксис: mov R, (adr)\n \
\tmov R, var\n";
            std::cout << "swp - меняет значение регистров S и A\n\t\
Синтаксис: swp\n";
            std::cout << "sum - складывает регистры S и A, результат записывается в S\n\t\
Синтаксис: sum\n";
            std::cout << "sub - вычитает регистры S и A, результат записывается в S\n\t\
Синтаксис: sub S, A (вычитает из регистра S, регистр A)\n\
\tsub A, S(вычитает из регистра A, регистр S)";
            std::cout << "inv - инвертирует регистр указанный регистр(S или A)\n\t\
Синтаксис: inv R\n";
            std::cout << "mul - перемножает регистры M и F, результат записывается в M\n\t\
Синтаксис: mul\n";
            std::cout << "end - завершение выполниния программы(должно быть всегда)\n\t\
Синтаксис: end\n";
            std::cout << "inp - ввод из файла в указанный адрес или переменную\n\t\
Синтаксис: inp (adr)t\n";
            std::cout << "jmp - условный/безусловный переходы к меткам\n\t\
Синтаксис: jmp lbl\t\t(безусловный переход)\n\
\tjmp +/0/-, lbl\t(условный переход) \n";
            std::cout << "cll - переходит к метке, сохраняя адрес возврата(вызывает функцию)\n\t\
Синтаксис: cll func()   в скобках должны быть указаны параметры вызываемой функции\n";
            std::cout << "ret - возвращиние из функции(return)\n\t\
Синтаксис: ret\n";
            std::cout << "lbl - создает метку/функцию\n\t\
Синтаксис: lbl name()   в скобках могут быть указаны параметры\n";
            std::cout << "";
            std::cout << "";
            std::cout << "";
            std::cout << "";
        }

        i += 1;  
    }
    for (const auto& [adr, name] : posFunc) {
        ram.memory[adr] += labels[name]; 
        std::cout << "adr - " << adr << ' ' << "name - " << name << ' ' << Word(ram.memory[adr]) << '\n';
    }
}

long long ten(unsigned long long ll) {
    long long ans = 0;
    for (int i = 62; i >= 0; i -= 2) { // 62 60 58 ... 0
        ans *= 3;
        if (ll & (2ULL << i)) {
            ans += 1;
            continue;
        }
        if ((ll & (1ULL << i))) {
            ans -= 1;
        }
    }
    return ans;
}

int main()
{   
        //case 80:    // ++++     (Adr) -> (S)
        //case 79:    // +++0     (Adr) -> (A)
        //case 78:    // +++-     (S) + (A) -> (S)
        //case 77:    // ++0+     (S) - (A) -> (S)
        //case 76:    // ++00     -(S) + (A) -> (S)
        //case 75:    // ++0-     -(S) -> (S)
        //case 74:    // ++-+     -(A) -> (A)
        //case 73:    // ++-0     (S) -> (Adr)
        //case 72:    // ++--     (A) -> (Adr)  
        //case 71:    // +0++     (W == +) ? (Adr) -> (C), условный переход 1
        //case 70:    // +0+0     (W == 0) ? (Adr) -> (C), условный переход 0
        //case 69:    // +0+-     (W == -) ? (Adr) -> (C), условный переход -1
        //case 68:    // +00+     (Adr) -> (C),            безусловный переход
        //case 67:    // +000     (Adr) -> (P)
        //case 66:    // +00-     
        //case 65:    // +0-+     
        //default:
        //    throw "Unknown operation";
    // 1 2 4 8 16 32 64     127
    // 1 3 9 27 81          121
    // 1 3 9 
    // 1 3 9
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    std::cout.tie(nullptr);
    unsigned long long run = 1;
    size_t RAMsize = 6561;
    RAM ram(RAMsize);
    CPU Cpu(ram);

    //ifstream in;
    //string file;
    //cin >> file;
    //if (file == "1") {
    //    in.open("asmb.txt");
    //}
    //else {
    //    in.open(file);
    //}
    ifstream in("program.txt");
    char* str = new char[100];
    int Asize = 100;
    vector<string> asmb(Asize);
    int i = 0;
    while (!in.eof()) {
        in.getline(str, 30);
        asmb[i] = str;
        i++;
    }
    for (int j = 0; j < i; j++) {
        std::cout << asmb[j] << '\n';
    }
    try {
        AssemblerCompiler(asmb, ram, Cpu);
    }
    catch (...) {
        std::cout << "error\n";
    }

    cin >> run;
    while (run) {
        for (int i = 0; i < run; i++) {
            if(!Cpu.execute()) {
                std::cout << "S - " << Cpu.S << ' ' << ten(Cpu.S.ll) << " \nA - " << Cpu.A << ' ' << ten(Cpu.A.ll) <<
                " \nC - " << Cpu.C << "\nK - " << Cpu.K <<
                "\nM - " << Cpu.M << ' ' << ten(Cpu.M.ll) << " \nF - " << Cpu.F << ' ' << ten(Cpu.F.ll) <<
                " \nP - " << Cpu.P << "\nW - " << Cpu.W << "\n\n";
                //std::cout << "Error\n";
                return 0;
            }
            //std::cout << "S - " << Cpu.S << ' ' << ten(Cpu.S.ll) << " \nA - " << Cpu.A << ' ' << ten(Cpu.A.ll) <<
            //    " \nC - " << Cpu.C << "\nK - " << Cpu.K << 
            //    "\nM - " << Cpu.M << ' ' << ten(Cpu.M.ll) << " \nF - " << Cpu.F << ' ' << ten(Cpu.F.ll) <<
            //    " \nP - " << Cpu.P << "\nW - " << Cpu.W << "\n\n";
            //std::cout << "Stek:\n";
            //unsigned long long x;
            //for (x = 0b10'10'10'10'10'10'10'10'111111111111111111111111111111111111111111111111; ten(x) > ten(Cpu.P.ll); decrementAddress(x)) {
            //    std::cout << Word(x) << " - " << Word(Cpu.peek(x)) << '\n';
            //}
        }
        cin >> run;
    }
}
