#include <iostream>
#include <c++/4.8.3/chrono>
#include "common/Alphabet.h"
#include "common/NFA.h"
#include "dfa/Solver.h"
#include "cachat/Determinizer.h"
#include "cachat/GrammarDFAtoPDSAFA.h"
#include "cachat/Cachat.h"

using namespace std;

void box_composition_test ()
{
    Box* ba = Box::test("a");
    Box* bb = Box::test("b");
    Box* bc = Box::test("c");
    Box* bd = Box::test("d");
    Box* be = Box::test("e");
    Box* bf = Box::test("f");
    Box* bg = Box::test("g");
    Box* bh = Box::test("h");

    Clause* c1 = new Clause();
    c1->boxes.push_back(ba);
    c1->boxes.push_back(bb);

    Clause* c2 = new Clause();
    c2->boxes.push_back(bc);
    c2->boxes.push_back(bd);

    Clause* c3 = new Clause();
    c3->boxes.push_back(be);
    c3->boxes.push_back(bf);

    Clause* c4 = new Clause();
    c4->boxes.push_back(bg);
    c4->boxes.push_back(bh);

    Formula* F = new Formula();
    F->clauses.push_back(c1);
    F->clauses.push_back(c2);

    Formula* G = new Formula();
    G->clauses.push_back(c3);
    G->clauses.push_back(c4);

    cout << *c1 << endl;
    cout << *c2 << endl;
    cout << *c3 << endl;
    cout << *c4 << endl;

    cout << *F << endl;
    cout << *G << endl;

    Formula* FG = (F->composeWith(G));
    cout << *FG << endl;

    cout << F->implies(G) << endl;
    cout << FG->implies(F) << endl;
    cout << F->implies(FG) << endl;

    Formula* TRUEFORM = new Formula();
    Formula* FALSEFORM = new Formula();
    FALSEFORM->clauses.push_back(new Clause());

    cout << (TRUEFORM->implies(F)) << endl;
    cout << (F->implies(TRUEFORM)) << endl;
    cout << (FALSEFORM->implies(F)) << endl;
    cout << (F->implies(FALSEFORM)) << endl;
};

pair<bool, bool> ourSolution (NFA* A, GameGrammar* G, Letter* X, Letter* Y)
{
    Solver* s = new Solver(A, G);
    s->solve();
    Formula* solX = s->formulaFor({X});
    Formula* solY = s->formulaFor({Y});
    bool resX = solX->isRejecting();
    bool resY = solY->isRejecting();
    return make_pair(resX, resY);
}

pair<bool, bool> cachat (NFA* A, GameGrammar* G, Letter* X, Letter* Y)
{
    Determinizer* det = new Determinizer(A);
    NFA* D = det->determinize();

    GrammarDFAtoPDSAFA* cachatifier = new GrammarDFAtoPDSAFA(D, G);
    tuple<GamePDS*, PAFA*, Letter*, Letter*> restuple = cachatifier->cachatify();
    GamePDS* P = get<0>(restuple);
    PAFA* AFA = get<1>(restuple);
    Letter* init_refuter = get<2>(restuple);
    Letter* init_prover = get<3>(restuple);
    Cachat* cachat = new Cachat(P, AFA);
    cachat->saturate();

    vector<Letter*> stack_X = cachatifier->wordToStackWord({X});
    vector<Letter*> stack_Y = cachatifier->wordToStackWord({Y});
    bool resX = AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_refuter], stack_X);
    bool resY = AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_prover], stack_Y);
    return make_pair(resX, resY);
}

void time_measuring ()
{
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


    auto start = chrono::steady_clock::now();
    auto x = ourSolution(A, G, X, Y);

    auto middle = chrono::steady_clock::now();
    auto y = cachat(A, G, X, Y);

    auto end = chrono::steady_clock::now();

    auto our = chrono::duration_cast<chrono::microseconds>(middle - start).count();
    auto cach = chrono::duration_cast<chrono::microseconds>(end - middle).count();

    cout << "Duration of our proc.: " << our << "ns : " << x.first << ", " << x.second << endl;
    cout << "Duration of our proc.: " << cach << "ns : " << y.first << ", " << y.second << endl;

}


void print_everything ()
{

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

//    Our Stuff
//    1. Create system of equations from Grammar
//    2. Solve system of equations over the CNF-formula domain using Kleene-Worklist-Procedure
//    3. Compute formula for given word
//    4. Check whether it is rejecting


    Solver* s = new Solver(A, G);
    s->solve();

    cout << "GRAMMAR:" << endl;
    cout << *G << endl;

    cout << endl;

    cout << "AUTOMATON:" << endl;
    cout << *A << endl;

    cout << endl << endl;

    cout << "OUR SOLUTION:" << endl;

    cout << "For X: (expected result: prover wins ~ not rejecting)" << endl;
    Formula* solX = s->formulaFor({X});
    cout << "Formula: " << *solX << endl;
    cout << "Rejecting: " << solX->isRejecting() << endl;

    cout << "For Y: (expected result: refuter wins ~ is rejecting)" << endl;
    Formula* solY = s->formulaFor({Y});
    cout << "Formula: " << *solY << endl;
    cout << "Rejecting: " << solY->isRejecting() << endl;


    cout << endl << endl;

//    Cachat
//    1. Upfront determinization of the automaton
//    2. Create pushdown system
//    3. Create AFA
//    4. Saturate AFA
//    5. Check whether given word is accepted by saturated AFA

    Determinizer* det = new Determinizer(A);
    NFA* D = det->determinize();


    cout << "CACHAT:" << endl;

    cout << "DFA:" << endl;

    cout << *D << endl;

    cout << endl;


    GrammarDFAtoPDSAFA* cachatifier = new GrammarDFAtoPDSAFA(D, G);
    tuple<GamePDS*, PAFA*, Letter*, Letter*> restuple = cachatifier->cachatify();
    GamePDS* P = get<0>(restuple);
    PAFA* AFA = get<1>(restuple);
    Letter* init_refuter = get<2>(restuple);
    Letter* init_prover = get<3>(restuple);

    cout << "PUSHDOWN SYSTEM:" << endl;

    cout << *P << endl;

    cout << endl;

    cout << "INITIAL AFA:" << endl;

    cout << *AFA << endl;

    cout << endl;

    Cachat* cachat = new Cachat(P, AFA);
    cachat->saturate();

    cout << "SATURATED AFA:" << endl;

    cout << *AFA << endl;

    cout << endl;

    cout << "For X (expected result: player0 (= refuter) does not win)" << endl;

    vector<Letter*> stack_X = cachatifier->wordToStackWord({X});

    cout << AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_refuter], stack_X) << endl;

    cout << "For Y (expected result: player1 (= refuter) does win)" << endl;

    vector<Letter*> stack_Y = cachatifier->wordToStackWord({Y});

    cout << AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_prover], stack_Y) << endl;
}

int main ()
{
    //print_everything();

    time_measuring();

    return 0;
}
