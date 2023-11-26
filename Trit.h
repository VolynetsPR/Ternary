#ifndef TRIT_H
#define TRIT_H

struct Trit {
    bool b1;
    bool b2;
    Trit(bool b1 = 0, bool b2 = 0) : b1(b1), b2(b2) {}
    int Asymmetrical() const;
    Trit(char const& c);

    Trit& operator= (char const& c);
};

#endif

