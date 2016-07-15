#include "Alphabet.h"

using namespace std;

string Alphabet::toString () const
{
    bool first = true;
    string res;
    for (Letter* l : letters)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            res.append(",");
        }
        res.append(l->toString());
    }
    return res;
}

Letter* Alphabet::addLetter (string name)
{
    Letter* l = new Letter(this, size(), name);
    letters.push_back(l);
    return l;
}

Alphabet::~Alphabet ()
{
    for (Letter* q : letters)
    {
        delete q;
    }
}
