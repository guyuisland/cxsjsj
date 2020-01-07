#ifndef MONSTER_H
#define MONSTER_H


#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include"skill.h"
#include"protocol.h"
class Monster
{
public:

    std::string get_name();
    int get_No();
    int get_cost();
    int get_HP();
    int get_MaxHP();
    int get_times();
    int get_MP_cost(int );
    bool dec_HP(int num);
    void add_HP(int num);
    int has_obj(int);
    virtual Skill skill1() = 0;
    virtual Skill skill2() = 0;
    virtual Skill skill3() = 0;
    virtual Skill skill4() = 0;
protected:
    int max_HP;
    int cur_HP;
	int energyCost;
	int callTimes;
	std::string name;
    std::vector<Skill> skills;
    int No;
};

class Flareon: public Monster
{
public:
    Flareon();
    ~Flareon();
    Skill skill1() override;
    Skill skill2() override;
    Skill skill3() override;
    Skill skill4() override;

private:
    //std::vector<Skill> skills;
};

class Sprite: public Monster
{
public:
    Sprite();
    ~Sprite();
    Skill skill1() override;
    Skill skill2() override;
    Skill skill3() override;
    Skill skill4() override;
private:
    //std::vector<Skill> skills;
};

class Pikachu: public Monster
{
public:
    Pikachu();
    ~Pikachu();
    Skill skill1() override;
    Skill skill2() override;
    Skill skill3() override;
    Skill skill4() override;
private:
    //std::vector<Skill> skills;
};

class Greninja: public Monster
{
public:
    Greninja();
    ~Greninja();
    Skill skill1() override;
    Skill skill2() override;
    Skill skill3() override;
    Skill skill4() override;
private:
    //std::vector<Skill> skills;
};

class Phoenix: public Monster
{
public:
    Phoenix();
    ~Phoenix();
    Skill skill1() override;
    Skill skill2() override;
    Skill skill3() override;
    Skill skill4() override;
private:
    //std::vector<Skill> skills;
};

class Dragonite: public Monster
{
public:
    Dragonite();
    ~Dragonite();
    Skill skill1() override;
    Skill skill2() override;
    Skill skill3() override;
    Skill skill4() override;
private:
    //std::vector<Skill> skills;
};

#endif //MONSTER_H

