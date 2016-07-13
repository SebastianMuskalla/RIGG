//
// Created by Sebastian on 13.07.2016.
//

#ifndef RIGG_POINTEECOMPARATOR_H
#define RIGG_POINTEECOMPARATOR_H


template<typename T>
struct PointeeComparator
{
    const T* to_find;

    PointeeComparator (const T* to_find) :
            to_find(to_find)
    { }

    bool operator() (const T* other) const
    {
        return *to_find == *other;
    }
};


#endif //RIGG_POINTEECOMPARATOR_H
