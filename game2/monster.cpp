#include"monster.h"

bool Monster::dec_HP(int num){
    if(num >= cur_HP){
       cur_HP=0;
       return true;
    }
    else{
       cur_HP -= num;
       return false;
    }
}

void Monster::add_HP(int num){
    cur_HP+=num;
    if(cur_HP>max_HP){
        cur_HP=max_HP;
    }
}

std::string Monster::get_name(){
    return name;
}

int Monster::get_No()
{
    return No;
}

int Monster::get_cost(){
    return energyCost;
}

int Monster::get_HP(){
    return cur_HP;
}
int Monster::has_obj(int skiPos){
    if(skills[skiPos].get_damage() > 0)
    {
        if(skills[skiPos].get_group())
            return ATTACK_A;
        else {
            return 1;
        }
    }
    return ATTACK_N;
}


Mage::Mage()
{
    max_HP = 6;
    cur_HP=max_HP;
    energyCost = 5;
    callTimes = 2;
    name = "Mage";

    skills.emplace_back(Skill(2,std::make_pair(false,0),std::make_pair(false,0),"thunder","kill a monster of opponent",false,false,false,false,false,false,100,0));
    skills.emplace_back(Skill(6,std::make_pair(false,0),std::make_pair(false,0),"fire","penetrate dec two damage for every monster",true,false,true,false,false,false,2,0));
    skills.emplace_back(Skill(4,std::make_pair(false,0),std::make_pair(false,0),"heal","治疗自己4点血",false,false,false,false,false,false,0,3));
}
Mage::~Mage()
{
}
Skill Mage::skill1()
{
    return skills[0];
}
Skill Mage::skill2()
{
    return skills[1];
}
Skill Mage::skill3()
{
    return skills[2];
}

/*Sprite::Sprite()
{
    max_HP = 5;
    cur_HP=max_HP;
    energyCost = 1;
    callTimes = 3;
    name = "Sprite";

    skills.emplace_back(Skill(2,std::make_pair(false,0),std::make_pair(false,0),true,false,false,false,false,false,0,1));
    skills.emplace_back(Skill(3,std::make_pair(false,0),std::make_pair(false,0),false,true,false,false,false,false,1,2));
    skills.emplace_back(Skill(2,std::make_pair(true,1),std::make_pair(false,0),false,false,false,false,false,false,1,0));
}
Sprite::~Sprite()
{
}
Skill Sprite::skill1()
{
    return skills[0];
}
Skill Sprite::skill2()
{
    return skills[1];
}
Skill Sprite::skill3()
{
    return skills[2];
}*/

Pegasus::Pegasus()
{
    max_HP = 10;
    cur_HP=max_HP;
    energyCost = 4;
    callTimes = 2;
    name = "Pegasus";

    skills.emplace_back(Skill(2,std::make_pair(true,1),std::make_pair(false,0),"疾风步","躲避当前攻击，并且下次攻击翻倍",false,false,false,true,false,false,0,0));
    skills.emplace_back(Skill(1,std::make_pair(false,0),std::make_pair(false,0),"钢闪刃","造成2点伤害",false,true,false,false,false,false,2,0));
    skills.emplace_back(Skill(4,std::make_pair(false,0),std::make_pair(false,0),"剑刃风暴","对所有怪兽造成3点伤害",true,false,false,false,false,false,3,0));
}
Pegasus::~Pegasus()
{
}
Skill Pegasus::skill1()
{
    return skills[0];
}
Skill Pegasus::skill2()
{
    return skills[1];
}
Skill Pegasus::skill3()
{
    return skills[2];
}

Scorpicore::Scorpicore()
{
    max_HP = 8;
    cur_HP=max_HP;
    energyCost = 2;
    callTimes = 3;
    name = "Scorpicore";

    skills.emplace_back(Skill(3,std::make_pair(false,0),std::make_pair(true,2),"反弹","可在接下来两回合内反弹对手的攻击",false,false,false,false,false,false,0,0));
    skills.emplace_back(Skill(2,std::make_pair(false,0),std::make_pair(false,0),"撕咬","造成3点伤害，恢复自身1点血量",false,false,false,false,false,false,3,1));
    skills.emplace_back(Skill(3,std::make_pair(false,0),std::make_pair(false,0),"淬毒","对敌人所有怪物造成1点伤害",true,false,false,false,false,false,1,0));
}
Scorpicore::~Scorpicore()
{
}
Skill Scorpicore::skill1()
{
    return skills[0];
}
Skill Scorpicore::skill2()
{
    return skills[1];
}
Skill Scorpicore::skill3()
{
    return skills[2];
}

/*Phoenix::Phoenix()
{
    max_HP = 12;
    cur_HP=max_HP;
    energyCost = 6;
    callTimes = 1;
    name = "Phoenix";

    skills.emplace_back(Skill(2,std::make_pair(false,0),std::make_pair(false,0),false,false,false,false,false,false,5,0));
    skills.emplace_back(Skill(2,std::make_pair(false,0),std::make_pair(false,0),false,false,false,false,false,true,0,0));//涅槃
    skills.emplace_back(Skill(2,std::make_pair(false,0),std::make_pair(false,0),false,true,false,false,false,false,2,2));
}
Phoenix::~Phoenix()
{
}
Skill Phoenix::skill1()
{
    return skills[0];
}
Skill Phoenix::skill2()
{
    return skills[1];
}
Skill Phoenix::skill3()
{
    return skills[2];
}*/

Knight::Knight()
{
    max_HP = 10;
    cur_HP=max_HP;
    energyCost = 1;
    callTimes = 3;
    name = "Knight";

    skills.emplace_back(Skill(7,std::make_pair(false,0),std::make_pair(false,0),"血暮","击杀对手所有怪兽",true,false,false,false,false,false,100,0));
    skills.emplace_back(Skill(2,std::make_pair(false,0),std::make_pair(false,0),"怖刃","造成1点伤害，加一点能量",false,false,false,false,true,false,1,0));//加能量
    skills.emplace_back(Skill(8,std::make_pair(false,0),std::make_pair(false,0),"暗瞳","穿透伤害",false,true,true,false,false,false,3,0));
}
Knight::~Knight()
{
}
Skill Knight::skill1()
{
    return skills[0];
}
Skill Knight::skill2()
{
    return skills[1];
}
Skill Knight::skill3()
{
    return skills[2];
}


