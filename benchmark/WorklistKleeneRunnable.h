#ifndef RIGG_WORKLISTKLEENERUNNABLE_H
#define RIGG_WORKLISTKLEENERUNNABLE_H


#include "Runnable.h"
#include "../dfa/WorklistKleene.h"
#include "../common/NFA.h"
#include "../common/GameGrammar.h"
#include "../common/Letter.h"
#include "Event.h"

using namespace std;

class WorklistKleeneRunnable : public Runnable
{
    WorklistKleene* s;
    NFA* A;
    GameGrammar* G;
    vector<Letter*> word;


public:
    WorklistKleeneRunnable (NFA* A, GameGrammar* G, vector<Letter*> word) :
            A(A),
            G(G),
            word(word)
    {
        s = nullptr;
    }


    virtual ~WorklistKleeneRunnable ()
    {
        if (s)
        {
            delete s;
        }
    }

    virtual unsigned int execute (Event* e)
    {
        solveWithWorklistKleene(A, G, word);
        e->signal();
        return 0;
    }

private:

    bool solveWithWorklistKleene (NFA* A, GameGrammar* G, vector<Letter*> word)
    {
        s = new WorklistKleene(A, G);
        s->solve();
        Formula* sol = s->formulaFor(word);
        return sol->isRejecting();
    }
};


#endif //RIGG_WORKLISTKLEENERUNNABLE_H
