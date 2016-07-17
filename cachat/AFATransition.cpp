#include "AFATransition.h"

using namespace std;

string AFATransition::toString () const
{
    string res = source->toString().append(" - ").append(label->toString()).append(" -> ");

    bool first = true;
    for (Letter* t : targets)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            res.append(" and ");
        }
        res.append(t->toString());
    }
    return res;
}

