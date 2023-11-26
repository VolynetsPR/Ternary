#include "Word.h"

Word::Word(unsigned long long num) {
    ll = num;
}

Word::Word(Word const& w) {
    ll = w.ll;
}

