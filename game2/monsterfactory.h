#ifndef MONSTERFACTORY_H
#define MONSTERFACTORY_H
#include"monster.h"
#include"protocol.h"
#include<unordered_map>

class MonsterFactory
{
public:
    MonsterFactory();
    ~MonsterFactory();
    int call_times_by_No(int );
    int mons_called_MP(int );
    int mons_skill_MP(int , int);
    Monster* create_monster(int);

    std::unordered_map<int, Monster*> monsBook;
};

#endif // MONSTERFACTORY_H
