#include "NFA.h"

using namespace std;

NFA::NFA (Alphabet* Sigma, Alphabet* states, Letter* initial_state, set<Letter*> &final_states) :
        Sigma(Sigma),
        states(states),
        initial_state(initial_state),
        final_states(final_states)
{ }

Transition* NFA::addTransition (Letter* origin, Letter* label, Letter* target)
{
    Transition* t = new Transition(origin, label, target);
    transitions.emplace(t);
    return t;
}

Box* NFA::boxFor (Letter* a)
{
    auto itr = boxes.find(a);
    if (itr != boxes.end())
    {
        return itr->second;
    }
    // Box does not exist yet, create it

    Box* res = new Box(this, states, a->name);
    for (Transition* t : transitions)
    {
        if (t->label == a)
        {
            res->content.emplace(t->origin, t->target);
        }
    }
    boxes.emplace(a, res);
    return res;
}
