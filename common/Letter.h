#ifndef RIGG_LETTER_H
#define RIGG_LETTER_H


#include <c++/4.8.3/string>


#include "Printable.h"

class Alphabet;

using namespace std;


class Letter : public Printable<Letter>
{
    friend class Alphabet;

public:
    Alphabet* alphabet;
    unsigned long long int index;
    string name;

    virtual string toString () const;


private:
    Letter (Alphabet* alphabet, unsigned long long int index, string &name) :
            alphabet(alphabet),
            index(index),
            name(name)
    { }


};


#endif //RIGG_LETTER_H
