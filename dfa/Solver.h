#ifndef RIGG_SOLVER_H
#define RIGG_SOLVER_H


#include "../common/NFA.h"
#include "../common/GameGrammar.h"
#include "../common/Alphabet.h"

using namespace std;

class Solver
{
public:
    NFA* A;
    Alphabet* Q;
    GameGrammar* G;
    Alphabet* Nprover;
    Alphabet* Nrefuter;
    Alphabet* Sigma;

    Solver (NFA* A, GameGrammar* G);

    Formula* formulaFor (vector<Letter*> word);

    Formula* formulaFor (Letter* l);

    void solve ();

private:
    Box* id_box;
    Formula* id_formula;

    multimap<Letter*, Letter*> dependencies;
    map<Letter*, Formula*> solution;

    set<Letter*> todo;

    void populate ()
    {
        populateSolutionAndWorklist();
        populateDependencies();
    }

    void populateSolutionAndWorklist ()
    {
        for (Letter* l : Nprover->letters)
        {
            solution.insert(make_pair(l, Formula::falseFormula()));
            todo.insert(l);
        }
        for (Letter* l : Nprover->letters)
        {
            solution.insert(make_pair(l, Formula::falseFormula()));
            todo.insert(l);
        }

    }

    void populateDependencies ()
    {
        for (auto rule : G->rules)
        {
            Letter* lhs = rule.first;
            vector<Letter*> rhs = rule.second;
            for (Letter* l : rhs)
            {
                if (G->isNonterminal(l))
                {
                    dependencies.emplace(l, lhs);
                }
            }
        }
    }

    Formula* recomputeValue (Letter* pLetter);


};


#endif //RIGG_SOLVER_H
