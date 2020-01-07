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

int Monster::get_MaxHP()
{
    return max_HP;
}

int Monster::get_times()
{
    return callTimes;
}

int Monster::get_MP_cost(int skiNo)
{
    return skills[skiNo].get_cost();
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


Flareon::Flareon()
{
    max_HP = 1;
    cur_HP=max_HP;
    energyCost = 1;
    callTimes = 5;
    name = "Mage";
    No = FLAREON;
    skills.emplace_back(Skill(2,std::make_pair(false,0),std::make_pair(false,0),"thunder","kill a monster of opponent",false,false,false,false,false,false,100,0));
    skills.emplace_back(Skill(6,std::make_pair(false,0),std::make_pair(false,0),"fire","penetrate dec two damage for every monster",true,false,true,false,false,false,2,0));
    skills.emplace_back(Skill(4,std::make_pair(false,0),std::make_pair(false,0),"heal","治疗自己4点血",false,false,false,false,false,false,0,3));
    skills.emplace_back(Skill(2,std::make_pair(false,0),std::make_pair(false,0),"group heal","治疗所有怪兽1点血量",true,false,false,false,false,false,0,1));
}
Flareon::~Flareon()
{
    delete this;
}
Skill Flareon::skill1()
{
    return skills[0];
}
Skill Flareon::skill2()
{
    return skills[1];
}
Skill Flareon::skill3()
{
    return skills[2];
}
Skill Flareon::skill4()
{
    return skills[3];
}

Pikachu::Pikachu()
{
    max_HP = 10;
    cur_HP=max_HP;
    energyCost = 1;
    callTimes = 2;
    name = "Pegasus";
    No = PIKACHU;
    skills.emplace_back(Skill(2,std::make_pair(true,1),std::make_pair(false,0),"疾风步","躲避当前攻击，并且下次攻击翻倍",false,false,false,true,false,false,0,0));
    skills.emplace_back(Skill(1,std::make_pair(false,0),std::make_pair(false,0),"钢闪刃","造成2点伤害",false,true,false,false,false,false,2,0));
    skills.emplace_back(Skill(4,std::make_pair(false,0),std::make_pair(false,0),"剑刃风暴","对所有怪兽造成3点伤害",true,false,false,false,false,false,3,0));
    skills.emplace_back(Skill(2,std::make_pair(false,0),std::make_pair(false,0),"再生","遭遇死亡，恢复至满血",false,false,false,false,false,true,0,0));
}
Pikachu::~Pikachu()
{
    delete this;
}
Skill Pikachu::skill1()
{
    return skills[0];
}
Skill Pikachu::skill2()
{
    return skills[1];
}
Skill Pikachu::skill3()
{
    return skills[2];
}
Skill Pikachu::skill4()
{
    return skills[3];
}

Greninja::Greninja()
{
    max_HP = 8;
    cur_HP=max_HP;
    energyCost = 1;
    callTimes = 3;
    name = "Scorpicore";
    No = GRENINJA;
    skills.emplace_back(Skill(3,std::make_pair(false,0),std::make_pair(true,2),"反弹","可在接下来两回合内反弹对手的攻击",false,false,false,false,false,false,0,0));
    skills.emplace_back(Skill(2,std::make_pair(false,0),std::make_pair(false,0),"撕咬","造成3点伤害，恢复自身1点血量",false,false,false,false,false,false,3,1));
    skills.emplace_back(Skill(3,std::make_pair(false,0),std::make_pair(false,0),"淬毒","对敌人所有怪物造成1点伤害",true,false,false,false,false,false,1,0));
    skills.emplace_back(Skill(3,std::make_pair(false,0),std::make_pair(false,0),"枯萎","穿透 造成8点伤害",false,false,true,false,false,false,1,0));
}
Greninja::~Greninja()
{
    delete this;
}
Skill Greninja::skill1()
{
    return skills[0];
}
Skill Greninja::skill2()
{
    return skills[1];
}
Skill Greninja::skill3()
{
    return skills[2];
}
Skill Greninja::skill4()
{
    return skills[3];
}


Dragonite::Dragonite()
{
    max_HP = 10;
    cur_HP=max_HP;
    energyCost = 1;
    callTimes = 3;
    name = "Dragonite";
    No = DRAGONITE;
    skills.emplace_back(Skill(7,std::make_pair(false,0),std::make_pair(false,0),"血暮","击杀对手所有怪兽",true,false,false,false,false,false,100,0));
    skills.emplace_back(Skill(2,std::make_pair(false,0),std::make_pair(false,0),"怖刃","造成1点伤害，加一点能量",false,false,false,false,true,false,1,0));//加能量
    skills.emplace_back(Skill(8,std::make_pair(false,0),std::make_pair(false,0),"暗瞳","穿透伤害",false,true,true,false,false,false,3,0));
   skills.emplace_back(Skill(2,std::make_pair(true,1),std::make_pair(false,0),"敏捷","闪避当前伤害，回敬一点伤害",false,false,false,false,false,false,1,0));
}
Dragonite::~Dragonite()
{
    delete this;
}
Skill Dragonite::skill1()
{
    return skills[0];
}
Skill Dragonite::skill2()
{
    return skills[1];
}
Skill Dragonite::skill3()
{
    return skills[2];
}
Skill Dragonite::skill4()
{
    return skills[3];
}

