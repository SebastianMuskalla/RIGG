/*
 * Copyright 2016-2022 Sebastian Muskalla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SaturationSolver.h"

void SaturationSolver::saturate ()
{
    unsigned int iteration = 0;
    bool done = false;

    logger.debug("Initialized saturation solver");

    while (!done)
    {
        done = true;
        iteration++;

        if (logger.accepts(DEBUG))
        {
            logger.debug("Iteration number " + to_string(iteration), 1);
        }

        for (Letter* p : P->statesExistential->letters)
        {
            for (Letter* a : P->Gamma->letters)
            {
                for (PDSTransition* t : P->transitions)
                {
                    if (t->read == a && t->source == p)
                    {
                        Letter* q = t->target;
                        Letter* qAFA = convertToAFAState(q);

                        vector<Letter*> v = t->write;
                        set<set<Letter*>> setsA = AFA->reachableFromControlState(qAFA, v);

                        for (const set<Letter*>& S : setsA)
                        {
                            // try to add transition (do not add it if already present)
                            if (AFA->addTransition(convertToAFAState(p), a, S))
                            {
                                done = false;
                            }
                        }
                    }
                }
            }
        }

        if (logger.accepts(DEBUG))
        {
            logger.debug("AFA after processing states of the Existential Player:", 2);
            logger.debug(AFA->toString(), 3);
        }

        for (Letter* p : P->statesUniversal->letters)
        {
            for (Letter* a : P->Gamma->letters)
            {
                // collect (q_i, v_i) states such that (p,a) -> (q_i, v_i) in the PDS
                set<pair<Letter*, vector<Letter*>>> allQvi;

                for (PDSTransition* t : P->transitions)
                {
                    if (t->read == a && t->source == p)
                    {
                        Letter* q = t->target;
                        Letter* qAFA = convertToAFAState(q);

                        vector<Letter*> v = t->write;
                        allQvi.insert(make_pair(qAFA, v));
                    }
                }

                // now we need to compute for each (qi vi) all possible S_i_j such that q_i - v_i -> S_i_j in the AFA
                // then we need to compute all unions S_1_j(1) cup ... cup S_imax_j(imax)
                set<set<Letter*>> unions;
                set<set<Letter*>> unionsNew;

                // add empty set as initial element
                unions.insert(set<Letter*>());


                for (const auto& qvi : allQvi)
                {
                    unionsNew.clear();
                    // compute for each i all possible S such that q_i - v_i -> S in the AFA
                    set<set<Letter*>> possibleSi = AFA->reachableFromControlState(qvi.first, qvi.second);

                    for (const set<Letter*>& Si : possibleSi)
                    {
                        for (const set<Letter*>& oldS : unions)
                        {
                            set<Letter*> newS;
                            newS.insert(oldS.begin(), oldS.end());
                            newS.insert(Si.begin(), Si.end());

                            if (!newS.empty())
                            {
                                unionsNew.insert(newS);
                            }
                        }
                    }
                    unions = unionsNew;
                }

                for (const set<Letter*>& S : unions)
                {
                    // try to add transition (do not add it if already present)
                    if (AFA->addTransition(convertToAFAState(p), a, S))
                    {
                        done = false;
                    }
                }
            }
        }

        if (logger.accepts(DEBUG))
        {
            logger.debug("AFA after processing states of the Universal Player:", 2);
            logger.debug(AFA->toString(), 3);
        }

    } // while !done

    logger.info("Saturation complete after " + to_string(iteration) + " iterations");

}


