#include "PDSTransition.h"

using namespace std;

string PDSTransition::toString () const
{
    string res = source->toString().append(" - r: ").append(read->toString()).append(" / w: ");
    for (Letter* w : write)
    {
        res.append(w->toString());
    }
    res.append(" -> ").append(target->toString());
    return res;
}

