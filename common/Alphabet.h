#ifndef RIGG_ALPHABET_H
#define RIGG_ALPHABET_H

#include <vector>
#include <string>
#include "Letter.h"

using namespace std;

/**
 * alphabet, i.e. list of letters
 */
class Alphabet : public Printable<Alphabet>
{
public:
    /**
     * the letters
     */
    vector<Letter*> letters;


    Alphabet ()
    {}

    virtual ~Alphabet ();

/**
     * number of letters currently in the alphabet
     */
    unsigned long long int size () const
    {
        return letters.size();
    }

    /**
     * get the letter with the specified index
     */
    Letter* get (unsigned int index) const
    {
        return letters[index];
    }

    Letter* addLetter (string name);

    virtual string toString () const;

    Alphabet (Alphabet const &) = delete;

    Alphabet &operator= (Alphabet const &) = delete;
};

#endif //RIGG_ALPHABET_H
