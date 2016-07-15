#ifndef RIGG_GAMEGRAMMAR_H
#define RIGG_GAMEGRAMMAR_H

#include <c++/4.8.3/unordered_map>
#include <c++/4.8.3/map>
#include "Alphabet.h"

using namespace std;

/**
 * grammar with a ownership partitioning on the non-termianls
 */
class GameGrammar : public Printable<GameGrammar>
{
public:
    /**
     * terminals
     */
    Alphabet* Sigma;

    /**
     * non-terminals owned by refuter / player 0
     */
    Alphabet* Nrefuter;

    /**
     * non-terminals owned by prover / player 1
     */
    Alphabet* Nprover;

    multimap<Letter*, vector<Letter*>> rules;

    GameGrammar (Alphabet* Sigma, Alphabet* Nrefuter, Alphabet* Nprover) :
            Sigma(Sigma),
            Nrefuter(Nrefuter),
            Nprover(Nprover)
    { }

    /**
     * We do not delete the terminal alphabet
     */
    virtual ~GameGrammar ();

/**
     * add rule with specified non-terminals als left-hand side and specified sentential form as right-hand-side to the grammar
     */
    void addRule (Letter* lhs, vector<Letter*> rhs)
    {
        rules.insert(make_pair(lhs, rhs));
    }

    /**
     * check whether a letter is a non-terminal owned by either player
     */
    bool isNonterminal (Letter* l) const
    {
        return (l->alphabet == Nprover || l->alphabet == Nrefuter);
    }

    virtual string toString () const;
};

#endif //RIGG_GAMEGRAMMAR_H
