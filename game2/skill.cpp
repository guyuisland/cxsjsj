﻿#include "skill.h"

Skill::Skill(int cost,std::pair<bool,int> evd,std::pair<bool,int> reb, QString Name, QString Descp, bool Group=false, bool Kill = false, bool Penetrate = false, bool critical=false,bool AddMP=false, bool Revive=false,int Damage = false,int Heal = false) {
    //describe = desc;
    costEnergy = cost;
    group = Group;
    kill = Kill;
    penetrate = Penetrate;
    damage = Damage;
    heal = Heal;
    evade = evd;
    rebound= reb;
    crit=critical;
    addMP=AddMP;
    revive=Revive;
    name = Name;
    description = Descp;
}
Skill::Skill(const Skill& copied) {
    //describe = copied.describe;
    costEnergy = copied.costEnergy;
    group = copied.group;
    kill = copied.kill;
    penetrate = copied.penetrate;
    damage = copied.damage;
    heal = copied.heal;
    evade=copied.evade;
    rebound=copied.rebound;
    crit=copied.crit;
    addMP=copied.addMP;
    revive=copied.revive;
    name = copied.name;
    description = copied.description;
}
Skill::Skill()
{
}
Skill::~Skill()
{
}

std::pair<bool,int> Skill::get_evade(){
    return evade;
}

std::pair<bool,int> Skill::get_rebound(){
    return rebound;
}

void Skill::set_describe(std::string str) {
    //describe = str;
}

QString Skill::get_name() {
    return name;
}

QString Skill::get_describe() {
    return description;
}

int Skill::get_cost(){
    return costEnergy;
}

int Skill::get_damage(){
    return damage;
}
bool Skill::get_crit(){
    return crit;
}
int Skill::get_heal(){
    return heal;
}
bool Skill::get_revive(){
    return revive;
}
bool Skill::get_group(){
    return group;
}
bool Skill::get_penetrate(){
    return penetrate;
}
bool Skill::get_addMP(){
    return addMP;
}
