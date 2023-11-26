#ifndef WORD_H
#define WORD_H

struct Word
{
    unsigned long long ll;

    Word(unsigned long long num = 0);
    Word(Word const& w);
};

#endif