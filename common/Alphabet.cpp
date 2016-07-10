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

