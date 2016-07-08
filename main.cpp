#include <iostream>
#include "common/Alphabet.h"
#include "common/NFA.h"
#include "dfa/Solver.h"

using namespace std;


int main ()
{

    Alphabet* Sigma = new Alphabet();
    Letter* a = Sigma->addLetter("a");
    Letter* b = Sigma->addLetter("b");

    Alphabet* Nprover = new Alphabet();
    Letter* X = Nprover->addLetter("X");

    Alphabet* Nrefuter = new Alphabet();
    Letter* Y = Nrefuter->addLetter("Y");

    GameGrammar* G = new GameGrammar(Sigma, Nprover, Nrefuter);
    G->addRule(X, {b, Y});
    G->addRule(X, {});
    G->addRule(Y, {a, X});

    Alphabet* Q = new Alphabet();
    Letter* q0 = Q->addLetter("q0");
    Letter* q1 = Q->addLetter("q1");
    set<Letter*> finals = {q0};

    NFA* A = new NFA(Sigma, Q, q0, finals);
    A->addTransition(q0, a, q1);
    A->addTransition(q1, b, q0);

//
//    Box* rho_a = A->boxFor(a);
//    Box* rho_b = A->boxFor(b);
//    Box* rho_ab = rho_a->composeWith(rho_b);
//
//
//    cout << *rho_a << endl;
//    cout << *rho_b << endl;
//    cout << *rho_ab << endl;
//
//    Box* rho_abab = rho_ab->composeWith(rho_ab);
//
//    cout << (*rho_abab == *rho_ab) << endl;
//
//
//    Box* ba = Box::test("a");
//    Box* bb = Box::test("b");
//    Box* bc = Box::test("c");
//    Box* bd = Box::test("d");
//    Box* be = Box::test("e");
//    Box* bf = Box::test("f");
//    Box* bg = Box::test("g");
//    Box* bh = Box::test("h");
//
//    Clause* c1 = new Clause();
//    c1->boxes.push_back(ba);
//    c1->boxes.push_back(bb);
//
//    Clause* c2 = new Clause();
//    c2->boxes.push_back(bc);
//    c2->boxes.push_back(bd);
//
//    Clause* c3 = new Clause();
//    c3->boxes.push_back(be);
//    c3->boxes.push_back(bf);
//
//    Clause* c4 = new Clause();
//    c4->boxes.push_back(bg);
//    c4->boxes.push_back(bh);
//
//    Formula* F = new Formula();
//    F->clauses.push_back(c1);
//    F->clauses.push_back(c2);
//
//    Formula* G = new Formula();
//    G->clauses.push_back(c3);
//    G->clauses.push_back(c4);
//
//    cout << *c1 << endl;
//    cout << *c2 << endl;
//    cout << *c3 << endl;
//    cout << *c4 << endl;
//
//    cout << *F << endl;
//    cout << *G << endl;
//
//    Formula* FG = (F->composeWith(G));
//    cout << *FG << endl;
//
//    cout << F->implies(G) << endl;
//    cout << FG->implies(F) << endl;
//    cout << F->implies(FG) << endl;
//
//    Formula* TRUEFORM = new Formula();
//    Formula* FALSEFORM = new Formula();
//    FALSEFORM->clauses.push_back(new Clause());
//
//    cout << (TRUEFORM->implies(F)) << endl;
//    cout << (F->implies(TRUEFORM)) << endl;
//    cout << (FALSEFORM->implies(F)) << endl;
//    cout << (F->implies(FALSEFORM)) << endl;

    Solver* s = new Solver(A, G);
    s->solve();
    Formula* F = s->formulaFor({a, X, Y, b});
    cout << *F << endl;
    cout << F->isRejecting() << endl;


//    Our Stuff
//    1. Create system of equations from Grammar
//    2. Solve system of equations over the CNF-formula domain using Kleene-Worklist-Procedure
//    3. Compute formula for given word
//    4. Check whether it is rejecting

//    Cachat
//    1. Upfront determinization of the automaton
//    2. Create pushdown system
//    3. Create AFA
//    4. Saturate AFA
//    5. Check whether given word is accepted by saturated AFA



    return 0;
}
