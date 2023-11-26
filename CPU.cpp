#include "CPU.h" // #include "RAM.h"
#include "Trit.h"

char Summ3Enter(Trit const& si, Trit const& ai, Trit& p) {
    switch (si.Asymmetrical() + ai.Asymmetrical() + p.Asymmetrical()) {
    case 6:
        p = '+';
        return '0';
    case 5:
        p = '+';
        return '-';
    case 4:
        p = '0';
        return '+';
    case 3:
        p = '0';
        return '0';
    case 2:
        p = '0';
        return '-';
    case 1:
        p = '-';
        return '+';
    case 0:
        p = '-';
        return '0';
    }
}
// (S) + (A) -> S
//void CPU::Sum() {
//    Trit p, si, ai; // si, ai - триты (S) и (A), p - перенос из предыдущего разряда 
//    char summ; // запись в текущий разяд
//    for (int i = 0; i < 64; i += 2) {
//        si.b1 = (S.ll) & (2ULL << i);
//        si.b2 = (S.ll) & (1ULL << i);
//        ai.b1 = A.ll & (2ULL << i);
//        ai.b2 = A.ll & (1ULL << i);
//        summ = Summ3Enter(si, ai, p);
//
//        if (summ == '+') {
//            S.ll |= (2ULL << i);
//            S.ll &= ~(1ULL << i);
//        }
//        else if (summ == '-') {
//            S.ll |= (1ULL << i);
//            S.ll &= ~(2ULL << i);
//        }
//        else {
//            S.ll &= ~((1ULL << i) | (2ULL << i));
//        }
//    }
//}

void Sum(unsigned long long& Sll, unsigned long long const& All) {
    Trit p, si, ai; // si, ai - триты (S) и (A), p - перенос из предыдущего разряда 
    char summ; // запись в текущий разяд
    for (int i = 0; i < 64; i += 2) {
        si.b1 = Sll & (2ULL << i);
        si.b2 = Sll & (1ULL << i);
        ai.b1 = All & (2ULL << i);
        ai.b2 = All & (1ULL << i);
        summ = Summ3Enter(si, ai, p);

        if (summ == '+') {
            Sll |= (2ULL << i);
            Sll &= ~(1ULL << i);
        }
        else if (summ == '-') {
            Sll |= (1ULL << i);
            Sll &= ~(2ULL << i);
        }
        else {
            Sll &= ~((1ULL << i) | (2ULL << i));
        }
    }
}

void SumAddress(unsigned long long& a1, unsigned long long const& a2) {
    Trit p, si, ai; // si, ai - триты (S) и (A), p - перенос из предыдущего разряда 
    char summ; // запись в текущий разяд
    for (int i = 48; i < 64; i += 2) { // 48 50 52 54 56 58 60 62
        si.b1 = a1 & (2ULL << i);
        si.b2 = a1 & (1ULL << i);
        ai.b1 = a2 & (2ULL << i);
        ai.b2 = a2 & (1ULL << i);
        summ = Summ3Enter(si, ai, p);

        if (summ == '+') {
            a1 |= (2ULL << i);
            a1 &= ~(1ULL << i);
        }
        else if (summ == '-') {
            a1 |= (1ULL << i);
            a1 &= ~(2ULL << i);
        }
        else {
            a1 &= ~((1ULL << i) | (2ULL << i));
        }
    }
}
//// (Adr) -> (S)
//void CPU::pS(unsigned long long const& address) {
//    S = peek(address);
//}
//// (Adr) -> (A)
//void CPU::pA(unsigned long long const& address) {
//    A = peek(address);
//}
//
//// (S) -> (Adr)
//void CPU::pAdrS(unsigned long long const& address) {
//    poke(address, S.ll);
//}
//// (A) -> (Adr)
//void CPU::pAdrA(unsigned long long const& address) {
//    poke(address, A.ll);
//}

// инвертирует триты слова
void inversion(unsigned long long& ll) {
    for (int i = 0; i < 64; i += 2) {
        if ((ll) & (2ULL << i)) {
            ll |= (1ULL << i);
            ll &= ~(2ULL << i);
        }
        else if ((ll) & (1ULL << i)) {
            ll |= (2ULL << i);
            ll &= ~(1ULL << i);
        }
    }
}
CPU::CPU(RAM ram) {
    unsigned long long address = 0b0101010101010101'111111111111111111111111111111111111111111111111; //  0
    peek(address, ram);
    poke(address, 0, ram);
    C.ll = address;
    P.ll = 0b10'10'10'10'10'10'10'10'111111111111111111111111111111111111111111111111; // ++++++++
    input.open("input.txt");
}

// (X) + 1 -> (X)
void incrementAddress(unsigned long long& Cll) {
    Trit p, si, ai(1, 0); // si - триты (C), p - перенос из предыдущего разряда 
    int i = 48; // 48 50 52 ... 58 60 62
    si.b1 = Cll & (2ULL << i);
    si.b2 = Cll & (1ULL << i);
    char summ = Summ3Enter(si, ai, p);
    ai.b1 = 0;
    ai.b2 = 0;
    while (i <= 62 && (p.b1 != 0 || p.b2 != 0)) {
        if (summ == '+') {
            Cll |= (2ULL << i);
            Cll &= ~(1ULL << i);
        }
        else if (summ == '-') {
            Cll |= (1ULL << i);
            Cll &= ~(2ULL << i);
        }
        else {
            Cll &= ~((1ULL << i) | (2ULL << i));
        }
        i += 2;
        si.b1 = (Cll) & (2ULL << i);
        si.b2 = (Cll) & (1ULL << i);
        summ = Summ3Enter(si, ai, p);
    }
    if (summ == '+') {
        Cll |= (2ULL << i);
        Cll &= ~(1ULL << i);
    }
    else if (summ == '-') {
        Cll |= (1ULL << i);
        Cll &= ~(2ULL << i);
    }
    else {
        Cll &= ~((1ULL << i) | (2ULL << i));
    }
}

// (X) - 1 -> (X)
void decrementAddress(unsigned long long& Cll) {
    Trit p, si, ai(0, 1); // si - триты (C), p - перенос из предыдущего разряда 
    int i = 48; // 48 50 52 ... 58 60 62
    si.b1 = Cll & (2ULL << i);
    si.b2 = Cll & (1ULL << i);
    char summ = Summ3Enter(si, ai, p);
    ai.b1 = 0;
    ai.b2 = 0;
    while (i <= 62 && (p.b1 != 0 || p.b2 != 0)) {
        if (summ == '+') {
            Cll |= (2ULL << i);
            Cll &= ~(1ULL << i);
        }
        else if (summ == '-') {
            Cll |= (1ULL << i);
            Cll &= ~(2ULL << i);
        }
        else {
            Cll &= ~((1ULL << i) | (2ULL << i));
        }
        i += 2;
        si.b1 = (Cll) & (2ULL << i);
        si.b2 = (Cll) & (1ULL << i);
        summ = Summ3Enter(si, ai, p);
    }
    if (summ == '+') {
        Cll |= (2ULL << i);
        Cll &= ~(1ULL << i);
    }
    else if (summ == '-') {
        Cll |= (1ULL << i);
        Cll &= ~(2ULL << i);
    }
    else {
        Cll &= ~((1ULL << i) | (2ULL << i));
    }
}
// (M) * (F) -> (M)
void CPU::Multiply() {
    unsigned long long ans = 0;
    unsigned long long invM = M.ll;
    inversion(invM);
    for (int i = 0; i < 64; i += 2) {
        if ((F.ll) & (2ULL << i)) {
            Sum(ans, (M.ll << i));
        }
        else if ((F.ll) & (1ULL << i)) {
            Sum(ans, invM << i);
        }
    }
    M.ll = ans;
}

void CPU::FlagW(unsigned long long const& ll) {
    if (ll == 0) {
        W.b1 = 0;
        W.b2 = 0;
    }
    else {
        for (int i = 62; i >= 0; i -= 2) {
            if (ll & (2ULL << i)) {
                W.b1 = 1;
                W.b2 = 0;
                return;
            }
            else if (ll & (1ULL << i)) {
                W.b1 = 0;
                W.b2 = 1;
                return;
            }
        }
    }
}

unsigned long long createNumber(char const* num);

bool CPU::execute() {
    K.ll = peek(C.ll);
    unsigned long long code = 0, p;
    char tr[33];
    bool b1, b2;
    for (int i = 46; i >= 40; i -= 2) { // 46 44 42 40
        code *= 3;
        b1 = K.ll & (2ULL << i);
        if (b1) {
            code += 2;
            continue;
        }
        b2 = K.ll & (1ULL << i);
        if (!b2) {
            code += 1;
        }
    }
    switch (code) {
    // () - содержимое ячейчи/регистра,  {} -  содержимое ячейки памяти по адресy в регистре
    // ->  -  записать правую часть выражения в левую
    // "" - следующая строчка из файла(строчка должна быть в троичном виде)
    case 80:    // ++++     (Adr) -> (S)
        S = peek(K.ll);
        FlagW(S.ll);
        break;
    case 79:    // +++0     (Adr) -> (A)
        A = peek(K.ll);
        FlagW(A.ll);
        break;
    case 78:    // +++-     (S) + (A) -> (S)
        Sum(S.ll, A.ll);
        FlagW(S.ll);
        break;
    case 77:    // ++0+     (S) - (A) -> (S)
        code = A.ll;
        inversion(A.ll);
        Sum(S.ll, A.ll);
        A.ll = code;
        FlagW(S.ll);
        break;
    case 76:    // ++00     -(S) + (A) -> (S)
        inversion(S.ll);
        Sum(S.ll, A.ll);
        FlagW(S.ll);
        break;
    case 75:    // ++0-     -(S) -> (S)
        inversion(S.ll);
        FlagW(S.ll);
        break;
    case 74:    // ++-+     -(A) -> (A)
        inversion(A.ll);
        FlagW(A.ll);
        break;
    case 73:    // ++-0     (S) -> (Adr)
        poke(K.ll, S.ll);
        FlagW(S.ll);
        break;
    case 72:    // ++--     (A) -> (Adr)
        poke(K.ll, A.ll);
        FlagW(A.ll);
        break;
    case 71:    // +0++     (W == +) ? Adr -> (C), условный переход 1
        if (W.b1) {
            C.ll = K.ll;
            return true;
        }
        break;
    case 70:    // +0+0     (W == 0) ? Adr -> (C), условный переход 0
        if (!(W.b1 || W.b2)) {
            C.ll = K.ll;
            return true;
        }
        break;
    case 69:    // +0+-     (W == -) ? Adr -> (C), условный переход -1
        if (W.b2) {
            C.ll = K.ll;
            return true;
        }
        break;
    case 68:    // +00+     Adr -> (C), безусловный переход
        C.ll = K.ll;
        return true;
    case 67:    // +000     Adr -> (P)
        P.ll = K.ll;
        break;
    case 66:    // +00-     (Adr) -> (M)
        M.ll = peek(K.ll);
        FlagW(M.ll);
        break;
    case 65:    // +0-+     (Adr) -> (F)
        F.ll = peek(K.ll);
        FlagW(F.ll);
        break;
    case 64:    // +0-0     (M) * (F) -> (M)
        Multiply();
        FlagW(M.ll);
        break;
    case 63:    // +0--     {P} -> (C), возвращение из под программы
        incrementAddress(P.ll);
        C.ll = peek(P.ll);
        //incrementAddress(C.ll);
        break;
    case 62:    // +-++     (C) -> {P}, адрес возврата
        code = C.ll;
        SumAddress(code, K.ll);
        poke(P.ll, code);
        decrementAddress(P.ll);
        break;
    case 61:    // +-+0     (P) + 1 -> (P), инкримент P
        incrementAddress(P.ll);
        break;
    case 60:    // +-+-     (P) - 1 -> (P), декремент P
        decrementAddress(P.ll);
        break;
    case 59:    // +-0+     (Adr) -> {P}, переменную по адресу положить на вершину стека(нужно для функций/рекурсии)
        poke(P.ll, peek(K.ll));
        decrementAddress(P.ll);
        break;
    case 58:    // +-00     {(P) + Adr} -> (S)
        code = K.ll;
        p = P.ll;
        SumAddress(p, code);
        S = peek(p);
        FlagW(S.ll);
        break;  
    case 57:    // +-0-     (S) -> {(P) + Adr}
        code = K.ll;
        p = P.ll;
        SumAddress(p, code);
        poke(p, S.ll);
        FlagW(S.ll);
        break;
    case 56:    // +--+     {(P) + Adr} -> (A)
        code = K.ll;
        p = P.ll;
        SumAddress(p, code);
        A = peek(p);
        FlagW(A.ll);
        break;
    case 55:    // +--0     (A) -> {(P) + Adr}
        code = K.ll;
        p = P.ll;
        SumAddress(p, code);
        poke(p, A.ll);
        FlagW(A.ll);
        break;
    case 54:    // +---     {(P) + Adr} -> (M)
        code = K.ll;
        p = P.ll;
        SumAddress(p, code);
        M = peek(p);
        FlagW(M.ll);
        break;
    case 53:    // 0+++     (M) -> {(P) + Adr}
        code = K.ll;
        p = P.ll;
        SumAddress(p, code);
        poke(p, M.ll);
        FlagW(M.ll);
        break;
    case 52:    // 0++0     {(P) + Adr} -> (F)
        code = K.ll;
        p = P.ll;
        SumAddress(p, code);
        F = peek(p);
        FlagW(F.ll);
        break;
    case 51:    // 0++-     (F) -> {(P) + Adr}
        p = P.ll;
        SumAddress(p, K.ll);
        poke(p, F.ll);
        FlagW(F.ll);
        break;
    case 50:    // 0+0+     (M) -> (Adr)
        poke(K.ll, M.ll);
        FlagW(M.ll);
        break;
    case 49:    // 0+00     (F) -> (Adr)
        poke(K.ll, M.ll);
        FlagW(M.ll);
        break;
    case 48:    // 0+0-     "input.txt" -> (Adr), считывает число из файлы "input.txt" и записывает его в ячейку по указанному адресу  
        for (int x = 0; x < 32; x++) {
            tr[x] = '0';
        }
        input >> tr;
        code = createNumber(tr);
        poke(K.ll, code);
        FlagW(code);
        break;
    case 47:    // 0+-+     (Adr) -> "output.txt"
        break;
    case 46:    // 0+-0     (S) <-> (A)
        code = A.ll;
        A.ll = S.ll;
        S.ll = code;
        FlagW(S.ll);
        break;
    case 45:    // 0+--     {(P) + Adr} -> {P}
        code = K.ll;
        p = P.ll;
        SumAddress(p, code);
        code = peek(p);
        poke(P.ll, code);
        decrementAddress(P.ll);
        FlagW(code);
        break;
    case 44:    // 00++     "input.txt" -> {(P) + Adr}
        for (int x = 0; x < 32; x++) {
            tr[x] = '0';
        }
        input >> tr;
        code = createNumber(tr);
        p = P.ll;
        SumAddress(p, K.ll);
        poke(p, code);
        FlagW(code);
    //case :    // 0+--     (M) <-> (F)
    default:
        return false;
        break;
    }
    incrementAddress(C.ll);
    return true;
}

// "заглянуть"
unsigned long long CPU::peek(unsigned long long const& address, RAM const& Ram) {
    static RAM const& ram = Ram;
    unsigned long long adr = 0;
    for (int i = 62; i >= 48; i -= 2) { // 62 60 58 ... 48
        adr *= 3;
        if (address & (2ULL << i)) {
            adr += 2;
            continue;
        }
        if (!(address & (1ULL << i))) {
            adr += 1;
        }
    }
    return ram.memory[adr];
}

// "ткнуть"
void CPU::poke(unsigned long long const& address, unsigned long long const& num, RAM const& Ram) {
    static RAM const& ram = Ram;
    unsigned long long adr = 0;
    for (int i = 62; i >= 48; i -= 2) { // 62 60 58 ... 36
        adr *= 3;
        if (address & (2ULL << i)) {
            adr += 2;
            continue;
        }
        if (!(address & (1ULL << i))) {
            adr += 1;
        }
    }
    ram.memory[adr] = num;
}