#ifndef RIGG_GAMEPDS_H
#define RIGG_GAMEPDS_H


#include <c++/4.8.3/set>
#include "../common/Alphabet.h"
#include "PDSTransition.h"

using namespace std;

class GamePDS : public Printable<GamePDS>
{
public:

    virtual string toString () const;

    Alphabet* player0_states;
    Alphabet* player1_states;
    Alphabet* stack_alphabet;
    set<PDSTransition*> transitions;

    GamePDS () :
            player0_states(),
            player1_states(),
            stack_alphabet(),
            transitions()
    { }
};


#endif //RIGG_GAMEPDS_H
