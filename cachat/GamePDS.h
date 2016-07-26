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

    virtual string toString () const;

    Alphabet* player0_states;
    Alphabet* player1_states;

    /**
     * stack alphabet
     */
    Alphabet* Gamma;
    set<PDSTransition*> transitions;

    GamePDS () :
            player0_states(),
            player1_states(),
            Gamma(),
            transitions()
    {}

    virtual ~GamePDS ()
    {
        delete player0_states;
        delete player1_states;
        delete Gamma;
        for (PDSTransition* t :transitions)
        {
            delete t;
        }

    }

    GamePDS (GamePDS const &) = delete;

    GamePDS &operator= (GamePDS const &) = delete;
};

#endif //RIGG_GAMEPDS_H
