#include "Determinizer.h"

using namespace std;

string Determinizer::setToString (set<Letter*> set)
{
    string res = "{";
    bool first = true;
    for (Letter* l : set)
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
    res.append("}");
    return res;
}

