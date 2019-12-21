#include "player.h"

Player::Player(std::string myName,int lv)
{
    monster.resize(3, nullptr);
    HP=1;
    MP=0;
    name=myName;
    level=lv;
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
    return monster.size();
}

int Player::get_HP(){
    return HP;
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

void Player::dec_monster_HP(int pos)
{
    if(monster[pos]->dec_HP(1)){
        delete monster[pos];
        monster[pos] = nullptr;
        empty_slot++;
        return;
    }
    else{
        return;
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
        return monster[i]->skill1();
    }
    case 1:{
        return monster[i]->skill1();
    }
    case 2:{
        return monster[i]->skill1();
    }
    }
    return monster[i]->skill1();
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
        monster[pos]->add_HP(addHP);
    }
    return;
}
void Player::ski_attack_monster_HP(Skill s, int pos, bool revive){//pos = -1是反弹
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
            if(monster[pos]->dec_HP(damage)){
                if(monster[pos]->get_name()=="Phoenix" && revive){
                    monster[pos]->add_HP(1000);
                }
            }

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
        if(monster[i]->get_HP() <= 0){
            delete monster[i];
        }
    }
}

int Player::has_obj(int monsterPos, int skiPos)
{
    return monster[monsterPos]->has_obj(skiPos);
}
