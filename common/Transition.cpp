#include "Transition.h"

using namespace std;

string Transition::toString () const
{
    return origin->toString().append(" - ").append(label->toString()).append(" -> ").append(target->toString());
}

