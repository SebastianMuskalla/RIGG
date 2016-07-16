//
// Created by solai on 16.07.2016.
//

#ifndef RIGG_NAIVEKLEENERUNNABLE_H
#define RIGG_NAIVEKLEENERUNNABLE_H


#include <zconf.h>
#include "../dfa/NaiveKleene.h"
#include "Runnable.h"
#include "Event.h"

class NaiveKleeneRunnable : public Runnable
{
    NaiveKleene* s;
    NFA* A;
    GameGrammar* G;
    vector<Letter*> word;

public:
    NaiveKleeneRunnable (NFA* A, GameGrammar* G, vector<Letter*> word) :
            A(A),
            G(G),
            word(word)
    {
        s = nullptr;
    }


    virtual ~NaiveKleeneRunnable ()
    {
        if (s)
        {
            delete s;
        }
        cout << "destructor called" << endl;
    }

    virtual unsigned int execute (Event* e)
    {
        solveWithNaiveKleene(A, G, word);
        e->signal();
        return 0;
    }

private:

    bool solveWithNaiveKleene (NFA* A, GameGrammar* G, vector<Letter*> word)
    {
        s = new NaiveKleene(A, G);
        s->solve();
        Formula* sol = s->formulaFor(word);
        bool res = sol->isRejecting();
        cout << sol->toString() << endl;
//        sleep(3000);
        return res;
    }

};


#endif //RIGG_NAIVEKLEENERUNNABLE_H
