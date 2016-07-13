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

    bool cout_debug = true;

    void populate ();

    void populateSolutionAndWorklist ();

    void populateDependencies ();

    Formula* recomputeValue (Letter* pLetter);
};


#endif //RIGG_SOLVER_H
