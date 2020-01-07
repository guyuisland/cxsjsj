#include "monsterfactory.h"

MonsterFactory::MonsterFactory()
{
    for(int i = 0; i < MONSTER.size(); i++)
    {
        monsBook[MONSTER[i]] = this->create_monster(MONSTER[i]);
    }

}

MonsterFactory::~MonsterFactory()
{
    for(int i = 0; i < MONSTER.size(); i++)
    {
        delete monsBook[MONSTER[i]];
    }
}


Monster* MonsterFactory::create_monster(int type)
{
    if(type == FLAREON){
        Monster* flareon = new Flareon();
        return flareon;
    }
    if(type == PIKACHU){
        Monster* pikachu = new Pikachu();
        return pikachu;
    }
    if(type == GRENINJA){
        Monster* greninja = new Greninja();
        return greninja;
    }
    if(type == DRAGONITE){
        Monster* dragonite = new Dragonite();
        return dragonite;
    }
}

int MonsterFactory::call_times_by_No(int monNo)
{
    return monsBook[monNo]->get_times();
}

int MonsterFactory::mons_called_MP(int monNo)
{
    return monsBook[monNo]->get_cost();
}

int MonsterFactory::mons_skill_MP(int monNo, int skiNo)
{
    return monsBook[monNo]->get_MP_cost(skiNo);
}

int MonsterFactory::mon_maxHP(int monNo)
{
    return monsBook[monNo]->get_MaxHP();
}
