#include <iostream>
#include <chrono>
#include <unistd.h>
#include "common/Types.h"
#include "common/Alphabet.h"
#include "common/NFA.h"
#include "dfa/WorklistKleene.h"
#include "cachat/Determinizer.h"
#include "cachat/GrammarDFAtoPDSAFA.h"
#include "cachat/Cachat.h"
#include "randomgen/TVGrammarGen.h"
#include "randomgen/TVAutomataGen.h"
#include "dfa/NaiveKleene.h"
#include "cachat/Minimizer.h"
#include "cachat/CachatWorklist.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
using namespace std;


/**
 * Prints the (ab)^* example from the paper in detail
 */
void printEverything ()
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

    WorklistKleene* s = new WorklistKleene(A, G);
    s->solve();

    cout << "BASIC EXAMPLE FROM THE PAPER:" << endl;

    cout << "############################" << endl;

    cout << "GRAMMAR:" << endl;
    cout << G->toString() << endl;

    cout << endl;

    cout << "AUTOMATON:" << endl;
    cout << A->toString() << endl;

    cout << endl << endl;

    cout << "OUR SOLUTION:" << endl;

    cout << "For X: (expected result: prover wins ~ not rejecting)" << endl;
    Formula* solX = s->formulaFor({X});
    cout << "Formula: " << solX->toString() << endl;
    cout << "Rejecting: " << solX->isRejecting() << endl;

    cout << "For Y: (expected result: refuter wins ~ is rejecting)" << endl;
    Formula* solY = s->formulaFor({Y});
    cout << "Formula: " << solY->toString() << endl;
    cout << "Rejecting: " << solY->isRejecting() << endl;

    cout << endl << endl;

//    Cachat
//    1. Upfront determinization of the automaton
//    2. Create pushdown system & AFA
//    3. Saturate AFA
//    4. Check whether given word is accepted by saturated AFA

    Determinizer* det = new Determinizer(A);
    NFA* D = det->determinize();

    Minimizer* min = new Minimizer(D);
    NFA* M = min->minimize();

    cout << "CACHAT:" << endl;

    cout << "DFA:" << endl;

    cout << D->toString() << endl;

    cout << "minDFA:" << endl;

    cout << M->toString() << endl;

    cout << endl;

    GrammarDFAtoPDSAFA* cachatifier = new GrammarDFAtoPDSAFA(M, G);
    tuple<GamePDS*, PAFA*, Letter*, Letter*> restuple = cachatifier->cachatify();
    GamePDS* P = get<0>(restuple);
    PAFA* AFA = get<1>(restuple);
    Letter* init_refuter = get<2>(restuple);
    Letter* init_prover = get<3>(restuple);

    cout << "PUSHDOWN SYSTEM:" << endl;

    cout << P->toString() << endl;

    cout << endl;

    cout << "INITIAL AFA:" << endl;

    cout << AFA->toString() << endl;

    cout << endl;

    Cachat* cachat = new Cachat(P, AFA);
    cachat->saturate();

    cout << "SATURATED AFA:" << endl;

    cout << AFA->toString() << endl;

    cout << endl;

    cout << "For X (expected result: player0 (= refuter) does not win)" << endl;

    vector<Letter*> stack_X = cachatifier->wordToStackWord({X});

    cout << AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_refuter], stack_X) << endl;

    cout << "For Y (expected result: player1 (= refuter) does win)" << endl;

    vector<Letter*> stack_Y = cachatifier->wordToStackWord({Y});

    cout << AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_prover], stack_Y) << endl;

    cout << "############################" << endl;

    cout << endl;
}

///**
// * Test for the formula composition
// */
//void testFormulaComposition ()
//{
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
//    Formula* F = new Formula(nullptr); // fix this
//    F->clauses.push_back(c1);
//    F->clauses.push_back(c2);
//
//    Formula* G = new Formula(nullptr); // fix this
//    G->clauses.push_back(c3);
//    G->clauses.push_back(c4);
//
//    cout << c1->toString() << endl;
//    cout << c2->toString() << endl;
//    cout << c3->toString() << endl;
//    cout << c4->toString() << endl;
//
//    cout << F->toString() << endl;
//    cout << G->toString() << endl;
//
//    Formula* FG = (F->composeWith(G));
//    cout << FG->toString() << endl;
//
//    cout << F->implies(G) << endl;
//    cout << FG->implies(F) << endl;
//    cout << F->implies(FG) << endl;
//
//    Formula* TRUEFORM = new Formula(nullptr); // fix this
//    Formula* FALSEFORM = new Formula(nullptr); // fix this
//    FALSEFORM->clauses.push_back(new Clause());
//
//    cout << (TRUEFORM->implies(F)) << endl;
//    cout << (F->implies(TRUEFORM)) << endl;
//    cout << (FALSEFORM->implies(F)) << endl;
//    cout << (F->implies(FALSEFORM)) << endl;
//};

/**
 * Tests the reachability algorithm for AFAs
 *
 * Does not work anymore
 */
void testAFAReachability ()
{
    Alphabet* Gamma = new Alphabet();
    Letter* a = Gamma->addLetter("a");
    Letter* b = Gamma->addLetter("a");
    PAFA* TEST = new PAFA(nullptr);
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

    cout << TEST->toString() << endl;

    cout << endl << endl;

    for (set<Letter*> S : TEST->reachableFromControlState(q0, {a, b}))
    {
        cout << "one S: ";
        for (Letter* s : S)
        {
            cout << s->toString() << ",";
        }
        cout << endl;
    }
}

void testMinimization ()
{
    Alphabet* Q = new Alphabet();
    Letter* a = Q->addLetter("a");
    Letter* b = Q->addLetter("b");
    Letter* c = Q->addLetter("c");
    Letter* d = Q->addLetter("d");
    Letter* e = Q->addLetter("e");
    Letter* f = Q->addLetter("f");

    Alphabet* Sigma = new Alphabet();
    Letter* n = Sigma->addLetter("0");
    Letter* o = Sigma->addLetter("1");

    NFA* DFA = new NFA(Sigma, Q, a, {c, e, d});
    DFA->addTransition(a, n, b);
    DFA->addTransition(a, o, c);
    DFA->addTransition(b, n, a);
    DFA->addTransition(b, o, d);
    DFA->addTransition(c, n, e);
    DFA->addTransition(c, o, f);
    DFA->addTransition(d, n, e);
    DFA->addTransition(d, o, f);
    DFA->addTransition(e, n, e);
    DFA->addTransition(e, o, f);
    DFA->addTransition(f, n, f);
    DFA->addTransition(f, o, f);

    cout << DFA->toString() << endl;
    cout << endl << endl;

    Minimizer* min = new Minimizer(DFA);
    NFA* M = min->minimize();

    cout << M->toString() << endl;
}

/**
 * Grammar generating (ab)^*, Automaton accepting (ab)^*
 *
 * (Example from the Paper)
 */
tuple<NFA*, GameGrammar*, vector<Letter*>> example1 ()
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

/**
 * Grammar generating (ab)^*, automaton accepting words where three consecutive a's or b's appear
 */
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
    // termination rule is missing

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

/**
 * Random example
 */
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

/**
 * Given an NFA representing the regular goal language, a game grammar and an initial sentential form,
 * return true iff refuter can win the non-inclusion game from the given sentential form
 *
 * Internally, fixed point iteration over the domain of box formulas is used
 */
bool solveWithWorklistDFA (NFA* A, GameGrammar* G, vector<Letter*> word)
{
    WorklistKleene* s = new WorklistKleene(A, G);
    s->solve();
    Formula* sol = s->formulaFor(word);

    bool res = sol->isRejecting();

    delete s;

    return res;
}

/**
 * Given an NFA representing the regular goal language, a game grammar and an initial sentential form,
 * return true iff refuter can win the non-inclusion game from the given sentential form
 *
 * Internally, fixed point iteration over the domain of box formulas is used
 */
bool solveWithNaiveDFA (NFA* A, GameGrammar* G, vector<Letter*> word)
{
    NaiveKleene* s = new NaiveKleene(A, G);
    s->solve();
    Formula* sol = s->formulaFor(word);

    bool res = sol->isRejecting();

    delete s;

    return res;
}

/**
 * Given an NFA representing the regular goal language, a game grammar and an initial sentential form,
 * return true iff refuter can win the non-inclusion game from the given sentential form
 *
 * Internally, the game is converted to a pushdown game a la Cachat and solved using his saturation procedure
 */
bool solveWithCachat (NFA* A, GameGrammar* G, vector<Letter*> word)
{
    // determinize the given automaton
    Determinizer* det = new Determinizer(A);
    NFA* D = det->determinize();

    // generate pushdown system and alternating automaton that define the equivalent game
    GrammarDFAtoPDSAFA* cachatifier = new GrammarDFAtoPDSAFA(D, G);
    tuple<GamePDS*, PAFA*, Letter*, Letter*> restuple = cachatifier->cachatify();
    GamePDS* P = get<0>(restuple);
    PAFA* AFA = get<1>(restuple);
    Letter* init_refuter = get<2>(restuple);
    Letter* init_prover = get<3>(restuple);

    // solve using cachats saturation procedure
    Cachat* cachat = new Cachat(P, AFA);
    cachat->saturate();
    vector<Letter*> stack_word = cachatifier->wordToStackWord(word);

    bool res = AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_refuter], stack_word);

    delete det;
    delete cachatifier;
    delete cachat;
    delete D;
    delete AFA;
    delete P;

    return res;
}

/**
 * Given an NFA representing the regular goal language, a game grammar and an initial sentential form,
 * return true iff refuter can win the non-inclusion game from the given sentential form
 *
 * Internally, the game is converted to a pushdown game a la Cachat and solved using his saturation procedure
 *
 * Provides time measuring for the 3 phases of the procedure
 */
tuple<bool, uint64, uint64, uint64, uint64> measureCachat (NFA* A, GameGrammar* G, vector<Letter*> word,
                                                           bool use_worklist = false)
{
    auto start = chrono::steady_clock::now();

    // determinize the given automaton
    Determinizer* det = new Determinizer(A);
    NFA* D = det->determinize();

    auto post_det = chrono::steady_clock::now();

    // determinize the given automaton
    Minimizer* min = new Minimizer(D);
    NFA* M = min->minimize();

    auto post_min = chrono::steady_clock::now();

    // generate pushdown system and alternating automaton that define the equivalent game
    GrammarDFAtoPDSAFA* cachatifier = new GrammarDFAtoPDSAFA(M, G);
    tuple<GamePDS*, PAFA*, Letter*, Letter*> restuple = cachatifier->cachatify();
    GamePDS* P = get<0>(restuple);
    PAFA* AFA = get<1>(restuple);
    Letter* init_refuter = get<2>(restuple);
    Letter* init_prover = get<3>(restuple);

    auto post_gen = chrono::steady_clock::now();

    // solve using cachats saturation procedure
    if (use_worklist)
    {
        CachatWorklist* cachat = new CachatWorklist(P, AFA);
        cachat->saturate();
        vector<Letter*> stack_word = cachatifier->wordToStackWord(word);
        bool res = AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_refuter], stack_word);

        auto end = chrono::steady_clock::now();

        uint64 determinize_time = chrono::duration_cast<chrono::milliseconds>(post_det - start).count();
        uint64 minimize_time = chrono::duration_cast<chrono::milliseconds>(post_min - post_det).count();
        uint64 generate_time = chrono::duration_cast<chrono::milliseconds>(post_gen - post_min).count();
        uint64 saturate_time = chrono::duration_cast<chrono::milliseconds>(end - post_gen).count();


        delete det;
        delete min;
        delete cachatifier;
        delete cachat;
        delete D;
        delete M;
        delete AFA;
        delete P;

        return tuple<bool, uint64, uint64, uint64, uint64>(res, determinize_time, minimize_time, generate_time,
                                                           saturate_time);
    } else
    {
        Cachat* cachat = new Cachat(P, AFA);
        cachat->saturate();
        vector<Letter*> stack_word = cachatifier->wordToStackWord(word);
        bool res = AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_refuter], stack_word);

        auto end = chrono::steady_clock::now();

        uint64 determinize_time = chrono::duration_cast<chrono::milliseconds>(post_det - start).count();
        uint64 minimize_time = chrono::duration_cast<chrono::milliseconds>(post_min - post_det).count();
        uint64 generate_time = chrono::duration_cast<chrono::milliseconds>(post_gen - post_min).count();
        uint64 saturate_time = chrono::duration_cast<chrono::milliseconds>(end - post_gen).count();


        delete det;
        delete min;
        delete cachatifier;
        delete cachat;
        delete D;
        delete M;
        delete AFA;
        delete P;

        return tuple<bool, uint64, uint64, uint64, uint64>(res, determinize_time, minimize_time, generate_time,
                                                           saturate_time);
    }
}


tuple<bool, bool, uint64, uint64, uint64, uint64> timeMeasuringWithWorklist (
        tuple<NFA*, GameGrammar*, vector<Letter*>, vector<Letter*>> t)
{
    NFA* A = get<0>(t);
    GameGrammar* G = get<1>(t);
    vector<Letter*> word1 = get<2>(t);
    vector<Letter*> word2 = get<3>(t);

    auto start = chrono::steady_clock::now();

    auto res_worklist_dfa_1 = solveWithWorklistDFA(A, G, word1);
    auto res_worklist_dfa_2 = solveWithWorklistDFA(A, G, word2);

    auto end = chrono::steady_clock::now();

    auto res_naive_dfa_1 = solveWithNaiveDFA(A, G, word1);
    auto res_naive_dfa_2 = solveWithNaiveDFA(A, G, word2);

    auto end2 = chrono::steady_clock::now();

    auto cachat_worklist_1 = measureCachat(A, G, word1, true);
    auto cachat_worklist_2 = measureCachat(A, G, word2, true);

    bool res_cachat_worklist_1 = get<0>(cachat_worklist_1);
    bool res_cachat_worklist_2 = get<0>(cachat_worklist_2);

    auto cachat_1 = measureCachat(A, G, word1);
    auto cachat_2 = measureCachat(A, G, word2);

    bool res_cachat_1 = get<0>(cachat_1);
    bool res_cachat_2 = get<0>(cachat_2);


    if (res_naive_dfa_1 != res_worklist_dfa_1
        || res_naive_dfa_2 != res_worklist_dfa_2
        || res_cachat_1 != res_worklist_dfa_1
        || res_cachat_2 != res_worklist_dfa_2
        || res_cachat_worklist_1 != res_worklist_dfa_1
        || res_cachat_worklist_2 != res_worklist_dfa_2
            )
    {
        string error = "RESULTS DIFFER:";
        error.append("\ndfa worklist:");
        error.append(to_string(res_worklist_dfa_1));
        error.append(", ");
        error.append(to_string(res_worklist_dfa_2));
        error.append("\ndfa naive:");
        error.append(to_string(res_naive_dfa_1));
        error.append(", ");
        error.append(to_string(res_naive_dfa_2));
        error.append("\ncachat: ");
        error.append(to_string(res_cachat_1));
        error.append(", ");
        error.append(to_string(res_cachat_2));
        error.append("\nworklist cachat: ");
        error.append(to_string(res_cachat_worklist_1));
        error.append(", ");
        error.append(to_string(res_cachat_worklist_2));
        throw error;
    }

    auto worklist_time = chrono::duration_cast<chrono::milliseconds>(end - start).count() / 2;

    auto naive_time = chrono::duration_cast<chrono::milliseconds>(end2 - end).count() / 2;

    uint64 cachat_time = (get<4>(cachat_1) + get<4>(cachat_2)) / 2;
    uint64 cachat_worklist_time = (get<4>(cachat_worklist_1) + get<4>(cachat_worklist_2)) / 2;


    return tuple<bool, bool, uint64, uint64, uint64, uint64>(res_worklist_dfa_1,
                                                             res_worklist_dfa_2,
                                                             naive_time,
                                                             worklist_time,
                                                             cachat_time,
                                                             cachat_worklist_time
    );
}

tuple<bool, bool, uint64, uint64, uint64, uint64> timeMeasuringWithoutWorklist (
        tuple<NFA*, GameGrammar*, vector<Letter*>, vector<Letter*>> t)
{
    NFA* A = get<0>(t);
    GameGrammar* G = get<1>(t);
    vector<Letter*> word1 = get<2>(t);
    vector<Letter*> word2 = get<3>(t);

    auto start = chrono::steady_clock::now();

    auto res_worklist_dfa_1 = solveWithWorklistDFA(A, G, word1);
    auto res_worklist_dfa_2 = solveWithWorklistDFA(A, G, word2);

    auto end = chrono::steady_clock::now();

    auto res_naive_dfa_1 = solveWithNaiveDFA(A, G, word1);
    auto res_naive_dfa_2 = solveWithNaiveDFA(A, G, word2);

    auto end2 = chrono::steady_clock::now();

    auto cachat_1 = measureCachat(A, G, word1);
    auto cachat_2 = measureCachat(A, G, word2);

    bool res_cachat_1 = get<0>(cachat_1);
    bool res_cachat_2 = get<0>(cachat_2);


    if (res_naive_dfa_1 != res_worklist_dfa_1
        || res_naive_dfa_2 != res_worklist_dfa_2
        || res_cachat_1 != res_worklist_dfa_1
        || res_cachat_2 != res_worklist_dfa_2
            )
    {
        string error = "RESULTS DIFFER:";
        error.append("\ndfa worklist:");
        error.append(to_string(res_worklist_dfa_1));
        error.append(", ");
        error.append(to_string(res_worklist_dfa_2));
        error.append("\ndfa naive:");
        error.append(to_string(res_naive_dfa_1));
        error.append(", ");
        error.append(to_string(res_naive_dfa_2));
        error.append("\ncachat: ");
        error.append(to_string(res_cachat_1));
        error.append(", ");
        error.append(to_string(res_cachat_2));
        throw error;
    }

    auto worklist_time = chrono::duration_cast<chrono::milliseconds>(end - start).count() / 2;

    auto naive_time = chrono::duration_cast<chrono::milliseconds>(end2 - end).count() / 2;

    uint64 cachat_time = (get<4>(cachat_1) + get<4>(cachat_2)) / 2;


    return tuple<bool, bool, uint64, uint64, uint64, uint64>(res_worklist_dfa_1,
                                                             res_worklist_dfa_2,
                                                             naive_time,
                                                             worklist_time,
                                                             cachat_time,
                                                             0
    );
}

/**
 * Takes a game instance (NFA, PDS, two initial sentential forms), solve it using both algorithms and measure the time it takes
 */
tuple<bool, bool, uint64, uint64, uint64, uint64> timeMeasuring (
        tuple<NFA*, GameGrammar*, vector<Letter*>, vector<Letter*>> t, bool with_cachat_worklist = false)
{
    if (with_cachat_worklist)
    {
        return timeMeasuringWithWorklist(t);
    } else
    {
        return timeMeasuringWithoutWorklist(t);
    }
};


/**
 * Generate random instances, solve them using both algorithms and print the needed time until the user terminates the program
 */
void measureAndPrint ()
{
    unsigned int nr_states = 3;
    unsigned int nr_terminals = 3;
    unsigned int nr_nonterminals_per_player = 3;

    // enabling this makes it very slow ...
    bool include_cachat_worklist = false;

    cout << "RANDOM GENERATED EXAMPLES:" << endl;
    cout << "############################" << endl;
    cout << "#states:       " << nr_states << endl;
    cout << "#terminals:    " << nr_terminals << endl;
    cout << "#nonterminals: " << 2 * nr_nonterminals_per_player << endl;
    cout << "############################" << endl;
    cout << endl;

    while (true)
    {
        NFA* A = TVAutomataGen(nr_terminals, nr_states, 0.8, 0.8).generate();
        GameGrammar* G = TVGrammarGen(A->Sigma, nr_nonterminals_per_player, nr_nonterminals_per_player, 0.75, 0.85,
                                      0.85, 0.85).generate();

        try
        {
            auto t = timeMeasuring(
                    tuple<NFA*, GameGrammar*, vector<Letter*>, vector<Letter*>>(A, G, {G->Nrefuter->get(0)},
                                                                                {G->Nprover->get(0)}),
                    include_cachat_worklist);

            cout << "naive dfa:       " << get<2>(t) << endl;
            cout << "worklist dfa:    " << get<3>(t) << endl;
            cout << "cachat:          " << get<4>(t) << endl;

            if (include_cachat_worklist)
            {
                cout << "worklist cachat: " << get<5>(t) << endl;
            }

            cout << endl;

            Alphabet* Sigma = A->Sigma;
            delete A;
            delete G;
            delete Sigma;
        }
        catch (string s)
        {
            cout << s << endl;
            return;
        }
    }
}

/**
 * Computes the every time Cachat's saturation algorithm needs for 10 random generated examples
 */
void averagify ()
{
    uint64 total = 0;
    uint64 nr_tries = 1;
    for (int i = 0; i < nr_tries; ++i)
    {
        NFA* A = TVAutomataGen(10, 5, 0.8, 0.8).generate();
        GameGrammar* G = TVGrammarGen(A->Sigma, 10, 10, 0.75, 0.85, 0.85, 0.85).generate();
        auto res1 = measureCachat(A, G, {G->Nrefuter->get(0)});
        auto res2 = measureCachat(A, G, {G->Nprover->get(0)});
        total += get<4>(res1);
        total += get<4>(res2);

        delete A->Sigma;
        delete A;
        delete G;
    }

    uint64 avg = total / (2 * nr_tries);
    cout << avg << endl;
}

void cachatExample ()
{
//    Rules:
//
//    p # -> f #;
//    p a -> p _;
//    f a -> p a;
//    f a -> f a a;
//    f # -> f #;
//
//    Control States:
//
//    p Abelard 1;
//    f Abelard 2;
//
//    Interesting Configurations:
//
//    p a a a #;
//    p #;

    // states of the reachability player (player 0 / Eloise)
    Alphabet* reach_states = new Alphabet(); // empty

    // states of the safety player (player 1 / Abelard)
    Alphabet* safety_states = new Alphabet();
    Letter* p = safety_states->addLetter("p");
    Letter* f = safety_states->addLetter("f");

    // stack alphabet
    Alphabet* stack_alphabet = new Alphabet();
    Letter* a = stack_alphabet->addLetter("a");
    Letter* bot = stack_alphabet->addLetter("#");

    GamePDS* pds = new GamePDS();
    pds->Gamma = stack_alphabet;
    pds->player0_states = reach_states;
    pds->player1_states = safety_states;

    pds->transitions.insert(new PDSTransition(p, bot, {bot}, f)); // p # -> f #;
    pds->transitions.insert(new PDSTransition(p, a, {}, p));      // p a -> p _;
    pds->transitions.insert(new PDSTransition(f, a, {a}, p));     // f a -> p a;
    pds->transitions.insert(new PDSTransition(f, a, {a, a}, f));  // f a -> f a a;
    pds->transitions.insert(new PDSTransition(f, bot, {bot}, f)); // f # -> f #;

    PAFA* pafa = new PAFA(pds);
    pafa->control_states = new Alphabet();
    for (Letter* l : reach_states->letters)
    {
        Letter* l_pafa = pafa->control_states->addLetter(l->name);
        pafa->pds_state_to_afa_state.emplace(l, l_pafa);
    }
    for (Letter* l : safety_states->letters)
    {
        Letter* l_pafa = pafa->control_states->addLetter(l->name);
        pafa->pds_state_to_afa_state.emplace(l, l_pafa);
    }

    // set the final state
    Letter* final_state_pds = p;
    Letter* final_state_pafa = pafa->pds_state_to_afa_state[final_state_pds];

    pafa->final_states.insert(final_state_pafa);

    // add transitions accepting sigma*
    for (Letter* gamma : stack_alphabet->letters)
    {
        pafa->transitions.insert(new AFATransition(final_state_pafa, gamma, {final_state_pafa}));
    }

    cout << "PUSHDOWN SYSTEM:" << endl;
    cout << pds->toString() << endl;
    cout << endl;
    cout << "INITIAL AFA:" << endl;
    cout << pafa->toString() << endl;
    cout << endl;
    cout << "BAD STATE:" << endl;
    cout << final_state_pafa->name << endl;
    cout << endl;

    cout << "######################" << endl;
    cout << "    RUNNING CACHAT" << endl;
    cout << "######################" << endl;

    // solve using cachats saturation procedure
    Cachat* cachat = new Cachat(pds, pafa);
    cachat->saturate();

    cout << "SATURATED AFA:" << endl;
    cout << pafa->toString() << endl;
    cout << endl;

    Letter* initial_state_pds = p;
    Letter* initial_state_pafa = pafa->pds_state_to_afa_state[initial_state_pds];
    vector<Letter*> initial_stack_content = {a, bot};

    bool res = pafa->acceptsFromControlState(initial_state_pafa, initial_stack_content);

    cout << "######################" << endl;
    cout << "    RESULT " << endl;
    cout << "######################" << endl;

    cout << "INITIAL STATE: " << initial_state_pds->name << endl;
    cout << "INITIAL STACK CONTENT: ";
    for (Letter* l : initial_stack_content)
        cout << l->name;
    cout << endl;

    cout << "INITIAL CONFIG IN WR OF REACH PLAYER: " << res << endl;

    delete pafa;
    delete pds;
}

int main ()
{
//    printEverything();

//    srand(time(NULL) * getpid());

//    measureAndPrint();

    cachatExample();

    return 0;
}

#pragma clang diagnostic pop
