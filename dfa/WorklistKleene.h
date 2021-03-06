#ifndef RIGG_SOLVER_H
#define RIGG_SOLVER_H

#include "../common/NFA.h"
#include "../common/GameGrammar.h"
#include "../common/Alphabet.h"
#include "FormulaStorage.h"

using namespace std;

/**
 * Solver for grammar games based on Kleene-iteration over the domain of box-formulas
 *
 * Uses a worklist to only updates single values that could have been become unstable due to other updates
 */
class WorklistKleene : public FormulaStorage
{
public:
    NFA* A;
    Alphabet* Q;
    GameGrammar* G;
    Alphabet* Nprover;
    Alphabet* Nrefuter;
    Alphabet* Sigma;

    /**
     * Generate solver for given game instance
     */
    WorklistKleene (NFA* A, GameGrammar* G);

    /**
     * Computes the formula for a given sentential form (by composing the formulas for the single letters) according to the current solution
     */
    Formula* formulaFor (vector<Letter*> word);

    /**
     * Computes the formula for a given letter (terminal or non-terminal of any of the two players)  according to the current solution
     */
    Formula* formulaFor (Letter* l);

    /**
     * Start the iteration to the solution
     */
    void solve ();

    virtual void registerFormula (Formula* f)
    {
        all_formulas.insert(f);
    }


    virtual ~WorklistKleene ()
    {
        for (Formula* f : all_formulas)
        {
            delete f;
        }
        A->resetBoxes();
    }

private:
    Box* identity_box;
    Formula* identity_formula;

    multimap<Letter*, Letter*> dependencies;
    map<Letter*, Formula*> solution;

    set<Letter*> worklist;

    set<Formula*> all_formulas;

    /**
     * Print debug information on the console during solution process
     */
    bool cout_debug = false;

    /**
     * Initially populate solution, worklist and dependencies
     */
    void populate ();

    /**
     * Initially populate solution and worklist
     */
    void populateSolutionAndWorklist ();

    /**
     * Initially populate dependencies
     */
    void populateDependencies ();

    /**
     * Recompute the solution for a given non-terminal
     */
    Formula* recomputeValue (Letter* l);

    WorklistKleene (WorklistKleene const &) = delete;

    WorklistKleene &operator= (WorklistKleene const &) = delete;
};

#endif //RIGG_SOLVER_H
