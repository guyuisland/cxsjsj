#ifndef PLAYER_H
#define PLAYER_H

#include<vector>
#include<unordered_map>
#include"json.hpp"
#include"protocol.h"
#include"monster.h"
#include<vector>

using json=nlohmann::json;

class Player
{
public:
    Player(){}
    Player(std::string myName,int lv);
    void attack(json&);
    void summon(json&);
    void accumulate(json&);
    void defend(json&);
    void skill(json&);
    void surrender(json&);
    void win(json&);
    void lose(json&);

    int get_HP();
    void get_HP_vec(std::vector<int>& );
    int get_MP();
    std::string get_name();
    int get_lv();

    void inc_MP();
    void inc_MP(Skill s);
    void dec_MP(int);
    void dec_HP();
    void add_HP(Skill s,int pos);

    void surrender();
    int monster_num();
    int dec_monster_HP(int );
    void ski_attack_monster_HP(Skill &s, int pos);
    void monster_revive(Skill s);
    void get_dead_monster(std::vector<int>&);
    //反弹攻击目标
    int rebound_aim();
    //怪兽空位数
    int get_empty_slot();
    //返回最低的空位号
    int get_slot_num();
    //添加新的怪物 返回消耗的能量
    int add_new_monster(Monster* );
    //返回i号位置的怪物j号技能
    Skill pos_i_skill_j(int ,int );
    //赋予状态
    void add_buff(int , int );
    //获得状态
    int  get_buff(int );
    //持续回合数下降
    void dec_round();
    //删除死亡的怪兽
    void delete_dead_monster();
    //i位置的怪兽的j号技能是否需要选择目标对象
    int has_obj(int , int);
    //返回所有怪物的编号
    void get_mon_No(std::vector<int>&);
private:
    std::vector<Monster*> monster;
    int empty_slot = 3;
    int HP = 1;
    int MP = 1;
    bool crazy;
    std::unordered_map<int,int> buff;//玩家状态(状态：持续回合数)
    std::string name;
    int level;
};

#endif // PLAYER_H
