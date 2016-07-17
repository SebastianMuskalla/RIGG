#ifndef RIGG_CACHATRUNNABLE_H
#define RIGG_CACHATRUNNABLE_H


#include "../common/NFA.h"
#include "../common/GameGrammar.h"
#include "Event.h"
#include "Runnable.h"
#include "../cachat/GrammarDFAtoPDSAFA.h"
#include "../cachat/Determinizer.h"
#include "../cachat/Minimizer.h"
#include "../cachat/Cachat.h"

using namespace std;

class CachatRunnable : public Runnable
{
    Determinizer* det;
    Minimizer* min;
    GrammarDFAtoPDSAFA* cachatifier;
    Cachat* cachat;
    NFA* D;
    NFA* M;
    PAFA* AFA;
    GamePDS* P;

    NFA* A;
    GameGrammar* G;
    vector<Letter*> word;

public:
    CachatRunnable (NFA* A, GameGrammar* G, vector<Letter*> word) :
            A(A),
            G(G),
            word(word)
    {
        det = nullptr;
        min = nullptr;
        cachatifier = nullptr;
        cachat = nullptr;
        D = nullptr;
        M = nullptr;
        AFA = nullptr;
        P = nullptr;
    }


    virtual ~CachatRunnable ()
    {
        if (det)
        {
            delete det;
        }
        if (min)
        {
            delete min;
        }
        if (cachatifier)
        {
            delete cachatifier;
        }
        if (cachat)
        {
            delete cachat;
        }
        if (D)
        {
            delete D;
        }
        if (M)
        {
            delete M;
        }
        if (AFA)
        {
            delete AFA;
        }
        if (P)
        {
            delete P;
        }
    }

    virtual unsigned int execute (Event* e)
    {
        solveWithCachat(A, G, word);
        e->signal();
        return 0;
    }

private:

    bool solveWithCachat (NFA* A, GameGrammar* G, vector<Letter*> word)
    {
        // determinize the given automaton
        det = new Determinizer(A);
        D = det->determinize();

        // minimize the given automaton
        min = new Minimizer(D);
        M = min->minimize();

        // generate pushdown system and alternating automaton that define the equivalent game
        cachatifier = new GrammarDFAtoPDSAFA(M, G);
        tuple<GamePDS*, PAFA*, Letter*, Letter*> restuple = cachatifier->cachatify();
        P = get<0>(restuple);
        AFA = get<1>(restuple);
        Letter* init_refuter = get<2>(restuple);

        // solve using cachats saturation procedure
        cachat = new Cachat(P, AFA);
        cachat->saturate();

        vector<Letter*> stack_word = cachatifier->wordToStackWord(word);
        return AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_refuter], stack_word);
    }

};


#endif //RIGG_CACHATRUNNABLE_H
