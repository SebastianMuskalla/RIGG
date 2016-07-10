#ifndef RIGG_ALPHABET_H
#define RIGG_ALPHABET_H


#include <c++/4.8.3/vector>
#include <c++/4.8.3/string>
#include "Letter.h"


using namespace std;

class Alphabet : public Printable<Alphabet>
{
public:

    vector<Letter*> letters;


    unsigned long long int size () const
    {
        return letters.size();
    }

    Letter* get (unsigned int index) const
    {
        return letters.at(index);
    }

    Letter* addLetter (string name)
    {
        Letter* l = new Letter(this, size(), name);
        letters.push_back(l);
        return l;
    }

    virtual string toString () const;

};


#endif //RIGG_ALPHABET_H
