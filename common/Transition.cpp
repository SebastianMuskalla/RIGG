#include "Transition.h"

using namespace std;

string Transition::toString () const
{
    return source->toString().append(" - ").append(label->toString()).append(" -> ").append(target->toString());
}

