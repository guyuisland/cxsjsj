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
    if(type == MAGE){
        Monster* mage = new Mage();
        return mage;
    }
    /*if(type == SPRITE){
        Monster* sprite = new Sprite();
        return sprite;
    }*/
    if(type== PEGASUS){
        Monster* pegasus = new Pegasus();
        return pegasus;
    }
    if(type == SCORPICORE){
        Monster* scorpicore = new Scorpicore();
        return scorpicore;
    }
    /*if(type == PHOENIX){
        Monster* phoenix = new Phoenix();
        return phoenix;
    }*/
    if(type == KNIGHT){
        Monster* knight = new Knight();
        return knight;
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
