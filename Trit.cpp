#include "Trit.h"

int Trit::Asymmetrical() const {
    if (b1 == 1) {
        return 2;
    }
    else if (b2 == 1) {
        return 0;
    }
    else {
        return 1;
    }
}

Trit::Trit(char const& c) {
    if (c == '+') {
        b1 = 1;
        b2 = 0;
    }
    else if (c == '-') {
        b1 = 0;
        b2 = 1;
    }
    else {
        b1 = b2 = 0;
    }
}

Trit& Trit::operator= (char const& c) {
    if (c == '+') {
        b1 = 1;
        b2 = 0;
    }
    else if (c == '-') {
        b1 = 0;
        b2 = 1;
    }
    else {
        b1 = b2 = 0;
    }
    return *this;
}