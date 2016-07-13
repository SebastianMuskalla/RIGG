#include <iostream>
#include <c++/4.8.3/chrono>
#include "common/Alphabet.h"
#include "common/NFA.h"
#include "dfa/Solver.h"
#include "cachat/Determinizer.h"
#include "cachat/GrammarDFAtoPDSAFA.h"
#include "cachat/Cachat.h"
#include "randomgen/TVGrammarGen.h"
#include "randomgen/TVAutomataGen.h"

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

bool dfa (NFA* A, GameGrammar* G, vector<Letter*> word)
{
    Solver* s = new Solver(A, G);
    s->solve();
    Formula* sol = s->formulaFor(word);
//    cout << *sol << endl;
    return sol->isRejecting();
}

bool cachat (NFA* A, GameGrammar* G, vector<Letter*> word)
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
    vector<Letter*> stack_word = cachatifier->wordToStackWord(word);
    return AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_refuter], stack_word);
}

tuple<NFA*, GameGrammar*, vector<Letter*>> example11 ()
{
    Alphabet* Sigma = new Alphabet();
    Letter* a = Sigma->addLetter("a");
    Letter* b = Sigma->addLetter("b");

    Alphabet* Nprover = new Alphabet();
    Letter* Y = Nprover->addLetter("Y");

    Alphabet* Nrefuter = new Alphabet();
    Letter* X = Nrefuter->addLetter("X");

    GameGrammar* G = new GameGrammar(Sigma, Nrefuter, Nprover);
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

    return tuple<NFA*, GameGrammar*, vector<Letter*>>(A, G, {X});
};

tuple<NFA*, GameGrammar*, vector<Letter*>> example2 ()
{
    Alphabet* Sigma = new Alphabet();
    Letter* a = Sigma->addLetter("a");
    Letter* b = Sigma->addLetter("b");

    Alphabet* Nprover = new Alphabet();
    Letter* Y = Nprover->addLetter("Y");

    Alphabet* Nrefuter = new Alphabet();
    Letter* X = Nrefuter->addLetter("X");

    GameGrammar* G = new GameGrammar(Sigma, Nrefuter, Nprover);
    G->addRule(X, {a, Y});
    G->addRule(X, {b, Y});
    G->addRule(Y, {a, X});
    G->addRule(Y, {b, X});

    Alphabet* Q = new Alphabet();
    Letter* q0 = Q->addLetter("q0");
    Letter* qf = Q->addLetter("qf");
    Letter* qa1 = Q->addLetter("qa1");
    Letter* qa2 = Q->addLetter("qa2");
    Letter* qb1 = Q->addLetter("qb1");
    Letter* qb2 = Q->addLetter("qb2");
    set<Letter*> finals = {qf};

    NFA* A = new NFA(Sigma, Q, q0, finals);
    A->addTransition(q0, a, qa1);
    A->addTransition(qa1, a, qa2);
    A->addTransition(qa2, a, qf);
    A->addTransition(q0, b, qb1);
    A->addTransition(qb1, b, qb2);
    A->addTransition(qb2, b, qf);
    return tuple<NFA*, GameGrammar*, vector<Letter*>>(A, G, {X});
};

tuple<NFA*, GameGrammar*, vector<Letter*>> example3 ()
{
    Alphabet* Sigma = new Alphabet();
    Letter* a = Sigma->addLetter("a");
    Letter* b = Sigma->addLetter("b");

    Alphabet* Nprover = new Alphabet();
    Letter* Y = Nprover->addLetter("Y");
    Letter* Z = Nprover->addLetter("Z");

    Alphabet* Nrefuter = new Alphabet();
    Letter* X = Nrefuter->addLetter("X");
    Letter* W = Nrefuter->addLetter("W");

    GameGrammar* G = new GameGrammar(Sigma, Nrefuter, Nprover);
    G->addRule(Y, {a, a});
    G->addRule(Y, {b, a});
    G->addRule(Z, {a});
    G->addRule(Z, {a, Z});
    G->addRule(X, {a, Y, b});
    G->addRule(X, {b});
    G->addRule(W, {b, a});
    G->addRule(W, {b});


    Alphabet* Q = new Alphabet();
    Letter* q0 = Q->addLetter("q0");
    Letter* q1 = Q->addLetter("q1");
    Letter* q2 = Q->addLetter("q2");
    set<Letter*> finals = {q1, q2};

    NFA* A = new NFA(Sigma, Q, q0, finals);
    A->addTransition(q2, a, q1);
    A->addTransition(q1, b, q2);
    return tuple<NFA*, GameGrammar*, vector<Letter*>>(A, G, {Y});
};

tuple<NFA*, GameGrammar*, vector<Letter*>> example4 ()
{
    Alphabet* Sigma = new Alphabet();
//    Letter* a = Sigma->addLetter("a");
//    Letter* b = Sigma->addLetter("b");

    Alphabet* Nprover = new Alphabet();
    Letter* Y = Nprover->addLetter("Y");
    Letter* Z = Nprover->addLetter("Z");

    Alphabet* Nrefuter = new Alphabet();
//    Letter* X = Nrefuter->addLetter("X");
//    Letter* W = Nrefuter->addLetter("W");

    GameGrammar* G = new GameGrammar(Sigma, Nrefuter, Nprover);

    G->addRule(Y, {});
    G->addRule(Y, {Z});

    G->addRule(Z, {});
    G->addRule(Z, {Y});

//
//    G->addRule(X, {Y});
//    G->addRule(X, {});
//
//    G->addRule(W, {});

    Alphabet* Q = new Alphabet();
    Letter* q0 = Q->addLetter("q0");
    set<Letter*> finals = {q0};

    NFA* A = new NFA(Sigma, Q, q0, finals);
//    A->addTransition(q2, a, q1);
//    A->addTransition(q1, b, q2);
    return tuple<NFA*, GameGrammar*, vector<Letter*>>(A, G, {Y});
};

tuple<NFA*, GameGrammar*, vector<Letter*>> example5 ()
{
    Alphabet* Sigma = new Alphabet();
    Letter* a = Sigma->addLetter("a");
//    Letter* b = Sigma->addLetter("b");

    Alphabet* Nprover = new Alphabet();
    Letter* Y = Nprover->addLetter("Y");
//    Letter* Z = Nprover->addLetter("Z");

    Alphabet* Nrefuter = new Alphabet();
//    Letter* X = Nrefuter->addLetter("X");
//    Letter* W = Nrefuter->addLetter("W");

    GameGrammar* G = new GameGrammar(Sigma, Nrefuter, Nprover);

    G->addRule(Y, {a, a});
    G->addRule(Y, {a});

//    G->addRule(Y, {Z});
//
//    G->addRule(Z, {});
//    G->addRule(Z, {Y});

//
//    G->addRule(X, {Y});
//    G->addRule(X, {});
//
//    G->addRule(W, {});

    Alphabet* Q = new Alphabet();
    Letter* q0 = Q->addLetter("q0");
    set<Letter*> finals = {q0};

    NFA* A = new NFA(Sigma, Q, q0, finals);
//    A->addTransition(q2, a, q1);
//    A->addTransition(q1, b, q2);
    return tuple<NFA*, GameGrammar*, vector<Letter*>>(A, G, {Y});
};

tuple<bool, uint, uint> time_measuring (tuple<NFA*, GameGrammar*, vector<Letter*>> t)
{
    NFA* A = get<0>(t);
    GameGrammar* G = get<1>(t);
    vector<Letter*> word = get<2>(t);

    auto start = chrono::steady_clock::now();

    auto x = dfa(A, G, word);

    auto middle = chrono::steady_clock::now();

    auto y = cachat(A, G, word);

    auto end = chrono::steady_clock::now();

    if (x != y)
    {
        string error = "results differ: ours: ";
        error.append(to_string(x));
        error.append(" cachat: ");
        error.append(to_string(y));
        throw error;
    }
    auto our = chrono::duration_cast<chrono::milliseconds>(middle - start).count();
    auto cach = chrono::duration_cast<chrono::milliseconds>(end - middle).count();
    return tuple<bool, uint, uint>(x, our, cach);
//
//    cout << "Duration of our proc.: " << our << "ns : " << x.first << ", " << x.second << endl;
//    cout << "Duration of our proc.: " << cach << "ns : " << y.first << ", " << y.second << endl;

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

    GameGrammar* G = new GameGrammar(Sigma, Nrefuter, Nprover);
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

void AFAreachabilityTest ()
{
    Alphabet* Gamma = new Alphabet();
    Letter* a = Gamma->addLetter("a");
    Letter* b = Gamma->addLetter("a");
    PAFA* TEST = new PAFA(Gamma, nullptr);
    TEST->control_states = new Alphabet();
    Letter* q0 = TEST->control_states->addLetter("q0");
    Letter* q1 = TEST->control_states->addLetter("q1");
    Letter* q2 = TEST->control_states->addLetter("q2");
    Letter* q11 = TEST->control_states->addLetter("q11");
    Letter* q12 = TEST->control_states->addLetter("q12");
    Letter* q21 = TEST->control_states->addLetter("q21");
    Letter* q22_a = TEST->control_states->addLetter("q22_a");
    Letter* q22_b = TEST->control_states->addLetter("q22_b");
    TEST->addTransition(q0, a, {q1});
    TEST->addTransition(q0, a, {q2});
    TEST->addTransition(q1, b, {q11});
    TEST->addTransition(q1, b, {q12});
    TEST->addTransition(q2, b, {q21});
    TEST->addTransition(q2, b, {q22_a, q22_b});

    cout << *TEST << endl;

    cout << endl << endl;

    for (set<Letter*> S : TEST->reachableFromControlState(q0, {a, b}))
    {
        cout << "one S: ";
        for (Letter* s : S)
        {
            cout << *s << ",";
        }
        cout << endl;
    }
}

int main ()
{
//    print_everything();

//    // Redirect cout.
//    streambuf* oldCoutStreamBuf = cout.rdbuf();
//    ostringstream strCout;
//    cout.rdbuf(strCout.rdbuf());

//    auto t = example2();
//    auto pair = time_measuring(t);
//
//    cout << "dfa time:    " << pair.first << endl;
//    cout << "cachat time: " << pair.second << endl;
//    return 0;

    while (true)
    {
        NFA* A = TVAutomataGen(10, 5, 0.8, 0.8).generate();

        GameGrammar* G = TVGrammarGen(A->Sigma, 10, 10, 0.75, 0.85, 0.85, 0.85).generate();

//        cout << *A << endl;
//
//        cout << endl << endl << endl;
//
//        cout << *G << endl;

        try
        {
            auto t = time_measuring(tuple<NFA*, GameGrammar*, vector<Letter*>>(A, G, {G->Nrefuter->get(0)}));

            if (get<0>(t))
            {
                cout << "dfa time:    " << get<1>(t) << endl;
                cout << "cachat time: " << get<2>(t) << endl;
            }
        }
        catch (string s)
        {

//            cout.rdbuf(oldCoutStreamBuf);
            cout << s << endl;
//            cout << strCout.str();

            return 0;
        }
//        strCout.str("");
//        strCout.clear();
    }
//

}
