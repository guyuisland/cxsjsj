#include "monsterfactory.h"

MonsterFactory::MonsterFactory()
{

}


Monster* MonsterFactory::create_monster(int type)
{
    if(type == MAGE){
        Monster* mage = new Mage();
        return mage;
    }
    if(type == SPRITE){
        Monster* sprite = new Sprite();
        return sprite;
    }
    if(type== PEGASUS){
        Monster* pegasus = new Pegasus();
        return pegasus;
    }
    if(type == SCORPICORE){
        Monster* scorpicore = new Scorpicore();
        return scorpicore;
    }
    if(type == PHOENIX){
        Monster* phoenix = new Phoenix();
        return phoenix;
    }
    if(type == KNIGHT){
        Monster* knight = new Knight();
        return knight;
    }
}
