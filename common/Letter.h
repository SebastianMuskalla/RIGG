#ifndef RIGG_LETTER_H
#define RIGG_LETTER_H

#include <c++/4.8.3/string>

#include "Printable.h"

class Alphabet;

using namespace std;

/**
 * a single letter in a alphabet
 *
 * will be used for terminals, non-terminals, stack symbols and control states
 */
class Letter : public Printable<Letter>
{
    friend class Alphabet;

public:
    Alphabet* alphabet;
    unsigned long long int index;
    string name;

    /**
     * string representation
     */
    virtual string toString () const;

private:

    /**
     * constructor is private, will only be called by the method "addLetter" of the friend class Alphabet
     */
    Letter (Alphabet* alphabet, unsigned long long int index, string &name) :
            alphabet(alphabet),
            index(index),
            name(name)
    { }

};

#endif //RIGG_LETTER_H
