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

#ifndef RIGG_GAMEPDS_H
#define RIGG_GAMEPDS_H

#include <set>
#include "../common/Alphabet.h"
#include "PDSTransition.h"

using namespace std;

/**
 * game pushdown system, i.e. pushdown system with ownership partitioning of the control states
 */
class GamePDS : public Printable<GamePDS>
{
public:

    string toString () const override;

    Alphabet* statesExistential;
    Alphabet* statesUniversal;

    /**
     * stack alphabet
     */
    Alphabet* Gamma;
    set<PDSTransition*> transitions;

    GamePDS () :
            statesExistential(),
            statesUniversal(),
            Gamma(),
            transitions()
    {}

    virtual ~GamePDS ()
    {
        delete statesExistential;
        delete statesUniversal;
        delete Gamma;
        for (PDSTransition* t : transitions)
        {
            delete t;
        }

    }

    GamePDS (GamePDS const&) = delete;

    GamePDS& operator= (GamePDS const&) = delete;
};

#endif //RIGG_GAMEPDS_H
