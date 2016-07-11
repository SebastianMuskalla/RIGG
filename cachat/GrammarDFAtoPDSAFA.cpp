#include "GrammarDFAtoPDSAFA.h"

using namespace std;

tuple<GamePDS*, PAFA*, Letter*, Letter*> GrammarDFAtoPDSAFA::cachatify ()
{

    GamePDS* res_pds = new GamePDS();

    Gamma = new Alphabet();
    player0_states = new Alphabet();
    player1_states = new Alphabet();

    res_pds->player1_states = player1_states;
    res_pds->player0_states = player0_states;
    res_pds->stack_alphabet = Gamma;

    for (Letter* a : Sigma->letters)
    {
        Letter* x = Gamma->addLetter(a->toString());
        stack_terminals.emplace(a, x);
    }
    for (Letter* X : Nrefuter->letters)
    {
        Letter* x = Gamma->addLetter(X->toString());
        stack_refuter_nonterminals.emplace(X, x);
    }
    for (Letter* Y : Nprover->letters)
    {
        Letter* x = Gamma->addLetter(Y->toString());
        stack_prover_nonterminals.emplace(Y, x);
    }

    for (Letter* q : DFA->states->letters)
    {
        Letter* q_refuter = player0_states->addLetter(q->toString().append("_r"));
        Letter* q_prover = player1_states->addLetter(q->toString().append("_p"));
        refuter_states.emplace(q, q_refuter);
        prover_states.emplace(q, q_prover);

    }

    PAFA* res_afa = new PAFA(Gamma, res_pds);
    res_afa->control_states = new Alphabet();

    for (Letter* q : player0_states->letters)
    {
        Letter* q_AFA = res_afa->control_states->addLetter(q->toString());
        res_afa->pds_state_to_afa_state.emplace(q, q_AFA);
    }
    for (Letter* q : player1_states->letters)
    {
        Letter* q_AFA = res_afa->control_states->addLetter(q->toString());
        res_afa->pds_state_to_afa_state.emplace(q, q_AFA);
    }

    for (Letter* q : DFA->states->letters)
    {
        if (DFA->final_states.find(q) == DFA->final_states.end())
        {
            Letter* q_prover = prover_states[q];
            Letter* q_refuter = refuter_states[q];
            Letter* q_prover_afa = res_afa->pds_state_to_afa_state.find(q_prover)->second;
            Letter* q_refuter_afa = res_afa->pds_state_to_afa_state.find(q_refuter)->second;
            res_afa->final_states.insert(q_prover_afa);
            res_afa->final_states.insert(q_refuter_afa);
        }
    }

    // transitions that change between players
    for (Letter* q : DFA->states->letters)
    {
        Letter* q_refuter = refuter_states.find(q)->second;
        Letter* q_prover = prover_states.find(q)->second;


        for (Letter* x : Gamma->letters)
        {
            PDSTransition* p1 = new PDSTransition(q_refuter, x, {x}, q_prover);
            PDSTransition* p2 = new PDSTransition(q_prover, x, {x}, q_refuter);

            res_pds->transitions.insert(p1);
            res_pds->transitions.insert(p2);
        }
    }


    // transitions that process terminals
    for (Transition* t : DFA->transitions)
    {
        Letter* source_refuter = refuter_states[t->origin];
        Letter* source_prover = prover_states[t->origin];
        Letter* target_refuter = refuter_states[t->target];
        Letter* target_prover = prover_states[t->target];
        Letter* x = stack_terminals[t->label];

        PDSTransition* p1 = new PDSTransition(source_refuter, x, {}, target_refuter);
        PDSTransition* p2 = new PDSTransition(source_prover, x, {}, target_prover);
        res_pds->transitions.insert(p1);
        res_pds->transitions.insert(p2);
    }

    // transitions that process nonterminals owned by refuter
    for (Letter* X : Nrefuter->letters)
    {

        Letter* stack_X = stack_refuter_nonterminals.find(X)->second;

        auto itrpair = G->rules.equal_range(X);


        for (auto itr = itrpair.first; itr != itrpair.second; ++itr)
        {

            vector<Letter*> stack_word;
            for (Letter* l : itr->second)
            {
                if (l->alphabet == Sigma)
                {
                    Letter* x = stack_terminals[l];
                    stack_word.push_back(x);
                }
                if (l->alphabet == Nprover)
                {
                    Letter* x = stack_prover_nonterminals[l];
                    stack_word.push_back(x);
                }
                if (l->alphabet == Nrefuter)
                {
                    Letter* x = stack_refuter_nonterminals[l];
                    stack_word.push_back(x);
                }
            }

            for (Letter* q : player0_states->letters)
            {
                PDSTransition* p = new PDSTransition(q, stack_X, stack_word, q);
                res_pds->transitions.insert(p);
            }
        }
    }

    // transitions that process nonterminals owned by prover
    for (Letter* Y : Nprover->letters)
    {

        Letter* stack_Y = stack_prover_nonterminals[Y];

        auto itrpair = G->rules.equal_range(Y);


        for (auto itr = itrpair.first; itr != itrpair.second; ++itr)
        {
            vector<Letter*> stack_word;
            for (Letter* l : itr->second)
            {
                if (l->alphabet == Sigma)
                {
                    Letter* x = stack_terminals[l];
                    stack_word.push_back(x);
                }
                if (l->alphabet == Nprover)
                {
                    Letter* x = stack_prover_nonterminals[l];
                    stack_word.push_back(x);
                }
                if (l->alphabet == Nrefuter)
                {
                    Letter* x = stack_refuter_nonterminals[l];
                    stack_word.push_back(x);
                }
            }

            for (Letter* q : player1_states->letters)
            {
                PDSTransition* p = new PDSTransition(q, stack_Y, stack_word, q);
                res_pds->transitions.insert(p);
            }
        }
    }

    Letter* init_refuter = refuter_states[DFA->initial_state];
    Letter* init_prover = prover_states[DFA->initial_state];

    return make_tuple(res_pds, res_afa, init_refuter, init_prover);


}

GrammarDFAtoPDSAFA::GrammarDFAtoPDSAFA (NFA* DFA, GameGrammar* G) :
        DFA(DFA),
        G(G),
        Sigma(G->Sigma),
        Nrefuter(G->Nrefuter),
        Nprover(G->Nprover),
        refuter_states(),
        prover_states(),
        stack_terminals(),
        stack_refuter_nonterminals(),
        stack_prover_nonterminals()
{

}

vector<Letter*> GrammarDFAtoPDSAFA::wordToStackWord (vector<Letter*> word)
{
    vector<Letter*> res;
    for (Letter* l : word)
    {
        if (l->alphabet == Sigma)
        {
            res.push_back(stack_terminals[l]);
        }
        if (l->alphabet == Nprover)
        {
            res.push_back(stack_prover_nonterminals[l]);
        }
        if (l->alphabet == Nrefuter)
        {
            res.push_back(stack_refuter_nonterminals[l]);
        }
    }
    return res;
}


