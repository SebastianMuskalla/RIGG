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

#ifndef RIGG_PDSTRANSITION_H
#define RIGG_PDSTRANSITION_H

#include <vector>
#include "../common/Letter.h"

using namespace std;

/**
 * transition of a (game) pushdown system
 */
class PDSTransition : public Printable<PDSTransition>
{
public:

    Letter* source;

    /**
     * stack symbol that is popped when executing the transition
     */
    Letter* read;

    /**
     * word that is pushed when executing the transition
     */
    vector<Letter*> write;

    Letter* target;

    PDSTransition (Letter* source, Letter* read, const vector<Letter*>& write, Letter* target) :
            source(source),
            read(read),
            write(write),
            target(target)
    {}

    string toString () const override;

    PDSTransition (PDSTransition const&) = delete;

    PDSTransition& operator= (PDSTransition const&) = delete;
};

#endif //RIGG_PDSTRANSITION_H
