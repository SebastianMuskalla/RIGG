#include "GamePDS.h"

using namespace std;

string GamePDS::toString () const
{
    string res;
    res.append("Stack alphabet: ");
    res.append(stack_alphabet->toString());
    res.append("\nStates of Player I: ");
    res.append(player1_states->toString());
    res.append("\nStates of Player II: ");
    res.append(player2_states->toString());
    res.append("\nTransition relation:");
    for (PDSTransition* t : transitions)
    {
        res.append("\n").append(t->toString());
    }
    return res;
}

