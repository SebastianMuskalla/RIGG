#ifndef RIGG_GAMEGRAMMAR_H
#define RIGG_GAMEGRAMMAR_H


#include <c++/4.8.3/unordered_map>
#include <c++/4.8.3/map>
#include "Alphabet.h"

using namespace std;

class GameGrammar
{
public:
    Alphabet* Sigma;
    Alphabet* Nprover;
    Alphabet* Nrefuter;
    multimap<Letter*, vector<Letter*>> rules;


    GameGrammar (Alphabet* Sigma, Alphabet* Nprover, Alphabet* Nrefuter) :
            Sigma(Sigma),
            Nprover(Nprover),
            Nrefuter(Nrefuter)
    { }

    void addRule (Letter* lhs, vector<Letter*> rhs)
    {
        rules.insert(make_pair(lhs, rhs));
    }

    bool isNonterminal (Letter* l) const
    {
        return (l->alphabet == Nprover || l->alphabet == Nrefuter);
    }
};


#endif //RIGG_GAMEGRAMMAR_H
