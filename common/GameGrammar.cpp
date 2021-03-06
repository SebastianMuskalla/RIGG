#include "GameGrammar.h"

using namespace std;

string GameGrammar::toString () const
{
    string res;
    res.append("Terminals: ");
    res.append(Sigma->toString());
    res.append("\nNonterminals of Prover: ");
    res.append(Nprover->toString());
    res.append("\nNonterminals of Refuter ");
    res.append(Nrefuter->toString());
    res.append("\nProduction rules:");
    for (pair<Letter*, vector<Letter*>> rule : rules)
    {
        res.append("\n");
        res.append(rule.first->toString());
        res.append(" -> ");
        for (Letter* l : rule.second)
        {
            res.append(l->toString());
        }
    }
    return res;
}

GameGrammar::~GameGrammar ()
{
    delete Nrefuter;
    delete Nprover;
}
