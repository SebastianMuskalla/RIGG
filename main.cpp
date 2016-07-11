#include <iostream>
#include <c++/4.8.3/chrono>
#include <c++/4.8.3/atomic>
#include "common/Alphabet.h"
#include "common/NFA.h"
#include "dfa/Solver.h"
#include "cachat/Determinizer.h"
#include "cachat/GrammarDFAtoPDSAFA.h"
#include "cachat/Cachat.h"

using namespace std;

/**
 * Hacky hackfix to avoid optimization breaking time measurement
 */
#define milestone lock.store(!lock.load(), memory_order::memory_order_seq_cst)


int main ()
{
    atomic_bool lock;

    Alphabet* Sigma = new Alphabet();
    Letter* a = Sigma->addLetter("a");
    Letter* b = Sigma->addLetter("b");

    Alphabet* Nprover = new Alphabet();
    Letter* Y = Nprover->addLetter("Y");

    Alphabet* Nrefuter = new Alphabet();
    Letter* X = Nrefuter->addLetter("X");

    GameGrammar* G = new GameGrammar(Sigma, Nprover, Nrefuter);
    G->addRule(X, {a, Y});
    G->addRule(X, {});
    G->addRule(Y, {b, X});

    Alphabet* Q = new Alphabet();
    Letter* q0 = Q->addLetter("q0");
    Letter* q1 = Q->addLetter("q1");
    set<Letter*> finals = {q0};

    NFA* A = new NFA(Sigma, Q, q0, finals);
    A->addTransition(q0, a, q1);
    A->addTransition(q1, b, q0);


//    for (auto pair : G->rules)
//    {
//        cout << "key: " << *pair.first << " value: ";
//        for (Letter* l : pair.second)
//        {
//            cout << *l;
//        }
//        cout << endl;
//
//    }

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



    auto start = chrono::high_resolution_clock::now();

    Solver* s = new Solver(A, G);
    s->solve();

    auto end = chrono::high_resolution_clock::now();


    cout << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << endl;

//
    cout << "from X: (expected result: prover wins, not rejecting)" << endl;
    Formula* solX = s->formulaFor({X});
//    cout << "formula: " << *solX << endl;
    cout << "rejecting: " << solX->isRejecting() << endl;

    cout << "from Y: (expected result: refuter wins, is rejecting)" << endl;
    Formula* solY = s->formulaFor({Y});
//    cout << "formula: " << *solY << endl;
    cout << "rejecting: " << solY->isRejecting() << endl;

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


    Determinizer* det = new Determinizer(A);
    NFA* D = det->determinize();

    cout << *A << endl;

    cout << endl;

    cout << *D << endl;

    cout << endl;

    cout << *G << endl;


    GrammarDFAtoPDSAFA* cachatifier = new GrammarDFAtoPDSAFA(D, G);
    tuple<GamePDS*, PAFA*, Letter*, Letter*> restuple = cachatifier->cachatify();
    GamePDS* P = get<0>(restuple);
    PAFA* AFA = get<1>(restuple);
    Letter* init_refuter = get<2>(restuple);
    Letter* init_prover = get<3>(restuple);

    cout << endl;

    cout << *P << endl;

    cout << endl;

    cout << *AFA << endl;

    cout << "init refuter: " << *init_refuter << endl;

    cout << "init prover: " << *init_prover << endl;

    cout << endl;

    Cachat* cachat = new Cachat(P, AFA);
    cachat->saturate();

    cout << endl;

    cout << *AFA << endl;

    cout << endl;


    cout << "checking X (expect: player0 does not win)" << endl;

    vector<Letter*> stack_X = cachatifier->wordToStackWord({X});

    cout << AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_refuter], stack_X) << endl;

    cout << "checking Y (expect: player0 wins)" << endl;

    vector<Letter*> stack_Y = cachatifier->wordToStackWord({Y});

    cout << AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_prover], stack_Y) << endl;

    return 0;
}
