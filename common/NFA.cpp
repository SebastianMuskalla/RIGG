#include "NFA.h"
#include "PointeeComparator.h"

using namespace std;

NFA::NFA (Alphabet* Sigma, Alphabet* Q, Letter* initial_state, set<Letter*> final_states) :
        Sigma(Sigma),
        Q(Q),
        initial_state(initial_state),
        final_states(final_states)
{ }

Transition* NFA::addTransition (Letter* source, Letter* label, Letter* target)
{
    Transition* t = new Transition(source, label, target);
    transitions.emplace(t);
    return t;
}

Box* NFA::boxFor (Letter* a)
{
    auto itr = box_for_letter.find(a);
    if (itr != box_for_letter.end())
    {
        return itr->second;
    }
    // Box does not exist yet, create it

    Box* res = new Box(this, Q, a->name);
    for (Transition* t : transitions)
    {
        if (t->label == a)
        {
            res->content.emplace(t->source, t->target);
        }
    }
    box_for_letter.emplace(a, res);
    return res;
}

void NFA::addTransition (Transition* t)
{
    transitions.insert(t);
}

string NFA::toString () const
{
    string res;
    res.append("Input alphabet: ");
    res.append(Sigma->toString());
    res.append("\nStates: ");
    res.append(Q->toString());
    res.append("\nInitial state: ");
    res.append(initial_state->toString());
    res.append("\nFinal states: ");

    bool first = true;
    for (Letter* f : final_states)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            res.append(",");
        }
        res.append(f->toString());
    }

    res.append("\nTransition relation:");
    for (Transition* t : transitions)
    {
        res.append("\n").append(t->toString());
    }
    return res;
}

bool NFA::tryAddTransition (Letter* source, Letter* label, Letter* target)
{
    Transition* t = new Transition(source, label, target);

    PointeeComparator<Transition> eq = {t};

    auto itr = find_if(transitions.begin(), transitions.end(), eq);

    if (itr == transitions.end())
    {
        transitions.insert(t);
        return true;
    }
    else
    {
        delete t;
        return false;
    }
}

NFA::~NFA ()
{
    delete Q;
    for (Transition* t : transitions)
    {
        delete t;
    }
    for (Box* b : all_boxes)
    {
        delete b;
    }
}
