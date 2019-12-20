#ifndef MONSTERFACTORY_H
#define MONSTERFACTORY_H
#include"monster.h"
#include"protocol.h"

class MonsterFactory
{
public:
    MonsterFactory();

    Monster* create_monster(int);
};

#endif // MONSTERFACTORY_H
