﻿#include "player.h"
#include <QDebug>

Player::Player(std::string myName,int lv)
{
    monster.resize(3, nullptr);
    HP=1;
    MP=10;
    name=myName;
    level=lv;
    crazy=false;
}
void Player::attack(json &sendinfo){
    sendinfo["define"]=OPTION;
    sendinfo["choice"]=ATTACK;

}
void Player::defend(json &sendinfo){
    sendinfo["define"]=OPTION;
     sendinfo["choice"]=DEFEND;
}

void Player::summon(json &sendinfo){
    sendinfo["define"]=OPTION;
    sendinfo["choice"]=SUMMON;
}

void Player::accumulate(json &sendinfo){
    sendinfo["define"]=OPTION;
    sendinfo["choice"]=ACCUMULATE;
}

void Player::skill(json &sendinfo){
    sendinfo["define"]=OPTION;
    sendinfo["choice"]=SKILL;
}

void Player::surrender(json &sendinfo){
    sendinfo["define"]=OPTION;
    sendinfo["choice"]=SURRENDER;
}

void Player::win(json &sendinfo){
    sendinfo["define"]=OPTION;
    sendinfo["choice"]=WIN;
}

void Player::lose(json &sendinfo){
    sendinfo["define"]=OPTION;
    sendinfo["choice"]=LOSE;
}

void Player::inc_MP(){
    MP++;
}

void Player::inc_MP(Skill s){
    if(s.get_addMP()){
        MP++;
    }
}

void Player::dec_HP(){
    HP--;
}



void Player::dec_MP(int mp){
    MP-=mp;
}

int Player::monster_num(){
    int num = 0;
    for(int i = 0; i < 3; i++)
    {
        if(monster[i] != nullptr)
            ++num;
    }
    return num;
}

int Player::get_HP(){
    return HP;
}

void Player::get_HP_vec(std::vector<int>& vec)
{
    vec[0] = HP;
    for(int i = 0; i < 3; i++)
    {
        if(monster[i] != nullptr)
        {
            vec[i + 1] = monster[i]->get_HP();
        }
        else{
            vec[i + 1] = -1;
        }
    }
}

int Player::get_MP(){
    return MP;
}

std::string Player::get_name(){
    return name;
}

int Player::get_lv(){
    return level;
}

int Player::dec_monster_HP(int pos)
{
    if(monster[pos]->dec_HP(1)){
//        delete monster[pos];
//        monster[pos] = nullptr;
//        empty_slot++;
        return 0;
    }
    else{
        return 1;
    }

}

void Player::surrender()
{
    HP = -1;
}

int Player::get_empty_slot()
{
    return empty_slot;
}

int Player::get_slot_num()
{
    int i = -1;
    for(i = 0; i < 3; i++)
    {
        if(monster[i] == nullptr)
            break;
    }
    return i;
}

int Player::add_new_monster(Monster* mp)
{
    int empty = this->get_slot_num();
    monster[empty] = mp;
    return mp->get_cost();
}

Skill Player::pos_i_skill_j(int i, int j){
    switch (j) {
    case 0:{
        return monster[i - 1]->skill1();
    }
    case 1:{
        return monster[i - 1]->skill2();
    }
    case 2:{
        return monster[i - 1]->skill3();
    }
    case 3:{
        return monster[i - 1]->skill4();
    }
    }
    return monster[i - 1]->skill1();
}

void Player::add_buff(int addBuff, int round){
    buff[addBuff]=round;
    return;
}

int Player::get_buff(int key){
    if(buff.count(key)!=0){
        return buff[key];
    }
    else{
        return 0;
    }
}

void Player::dec_round(){
    if(buff.count(EVADE)){
        buff[EVADE]--;
        if(buff[EVADE]==0){
            buff.erase(EVADE);
        }
    }
    if(buff.count(REBOUND)){
        buff[REBOUND]--;
        if(buff[REBOUND]==0){
            buff.erase(REBOUND);
        }
    }
    return ;
}

void Player::add_HP(Skill s, int pos){
    int addHP=s.get_heal();
    if(addHP==0){
        return ;
    }
    if(s.get_group()){
        for(unsigned int i=0;i < monster.size();i++){
            monster[i]->add_HP(addHP);
        }
    }
    else{
        monster[pos - 1]->add_HP(addHP);
    }
    return;
}
void Player::ski_attack_monster_HP(Skill &s, int pos){//pos = -1是反弹
    int flag = -1;
    for(unsigned int i=0;i < monster.size();i++){
        if(monster[i]!=nullptr){
            flag = i;
            break;
        }
    }
    if(flag == -1){
        return;
    }

    int damage = s.get_damage();
    if(crazy){
        damage = damage*2;
    }
    crazy = false;
    if(s.get_crit()){
        crazy=true;
    }
    if(s.get_group()){
        for(unsigned int i=0;i < monster.size();i++){
            if(monster[i]!=nullptr){
                monster[i]->dec_HP(damage);
            }

        }
        return;
    }
    else{
        if(pos == -1){
            pos = flag;
        }
        if(monster[pos]!=nullptr){
            monster[pos]->dec_HP(damage);
        }
        return;
    }
}
int Player::rebound_aim(){
    for(unsigned int i=0;i<monster.size();i++){
        if(monster[i]!=nullptr){
            return i+1;
        }
    }
    return 0;
}
void Player::delete_dead_monster(){
    for(unsigned int i=0;i<monster.size();i++){
        if(monster[i] != nullptr && monster[i]->get_HP() <= 0){

            delete monster[i];
            monster[i] = nullptr;
        }
    }
}

int Player::has_obj(int monsterPos, int skiPos)
{
    return monster[monsterPos]->has_obj(skiPos);
}

void Player::get_mon_No(std::vector<int>& monNo)
{
    for(int i = 0; i < 3; i++)
    {
        if(monster[i] != nullptr)
        {
            monNo[i + 1] = monster[i]->get_No();
        }
        else{
            monNo[i + 1] = -1;
        }
    }
}

void Player::get_dead_monster(std::vector<int>& vecMon)
{
    for(int i = 0; i < 3; i++)
    {
        if(monster[i] != nullptr)
        {
            if(monster[i]->get_HP() <= 0)
                vecMon.emplace_back(i + 1);
        }
    }
}
void Player::monster_revive(Skill s){
    if(s.get_revive()){
        for(unsigned int i=0; i<3; i++){
            if(monster[i] != nullptr)
            {
                if(monster[i]->get_HP() <= 0 && monster[i]->get_name() == "mage"){
                    monster[i]->add_HP(1000);
                }

            }
        }
    }
    return;
}
