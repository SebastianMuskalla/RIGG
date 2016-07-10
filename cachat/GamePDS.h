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


    Alphabet* player1_states;
    Alphabet* stack_alphabet;
    Alphabet* player2_states;
    set<PDSTransition*> transitions;
};


#endif //RIGG_GAMEPDS_H
