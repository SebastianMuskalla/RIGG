#ifndef RIGG_FORMULASTORAGE_H
#define RIGG_FORMULASTORAGE_H

class Formula;

#include "Formula.h"

class FormulaStorage
{
    virtual void registerFormula (Formula* f) = 0;
};


#endif //RIGG_FORMULASTORAGE_H
