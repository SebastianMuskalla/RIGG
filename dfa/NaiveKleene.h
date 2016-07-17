//
// Created by Sebastian on 15.07.2016.
//

#ifndef RIGG_NAIVEKLEENE_H
#define RIGG_NAIVEKLEENE_H

#include "../common/Alphabet.h"
#include "../common/GameGrammar.h"
#include "Formula.h"
#include "FormulaStorage.h"

/**
 * Solver for grammar games based on Kleene-iteration over the domain of box-formulas
 *
 * Kleene iteration is implemented in a naive way, i.e. all components of the solution vector are updated in each step
 */
class NaiveKleene : public FormulaStorage
{
public:
    NFA* A;
    Alphabet* Q;
    GameGrammar* G;
    set<Letter*> N;
    Alphabet* Nprover;
    Alphabet* Nrefuter;
    Alphabet* Sigma;

    bool timeout_flag = false;

/**
     * Generate solver for given game instance
     */
    NaiveKleene (NFA* A, GameGrammar* G, uint timeout = 0);

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


    NaiveKleene (NaiveKleene const &) = delete;

    NaiveKleene &operator= (NaiveKleene const &) = delete;

    virtual void registerFormula (Formula* f)
    {
        all_formulas.insert(f);
    }


    virtual ~NaiveKleene ()
    {
        for (Formula* f : all_formulas)
        {
            delete f;
        }
    }

private:
    Box* identity_box;
    Formula* identity_formula;

    map<Letter*, Formula*> solution;
    map<Letter*, Formula*> new_solution;

    set<Formula*> all_formulas;

    uint timeout;

    /**
     * Print debug information on the console during solution process
     */
    bool cout_debug = false;

    /**
     * Initially populate solution, worklist and dependencies
     */
    void populate ();

    /**
     * Recompute the solution for a given non-terminal
     */
    Formula* recomputeValue (Letter* l);

};

#endif //RIGG_NAIVEKLEENE_H
