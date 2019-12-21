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
    int get_cost();
    int get_HP();
    bool dec_HP(int num);
    void add_HP(int num);
    int has_obj(int);
    virtual Skill skill1() = 0;
    virtual Skill skill2() = 0;
    virtual Skill skill3() = 0;
protected:
    int max_HP;
    int cur_HP;
	int energyCost;
	int callTimes;
	std::string name;
    std::vector<Skill> skills;
};

class Mage: public Monster
{
public:
    Mage();
    ~Mage();
    Skill skill1() override;
    Skill skill2() override;
    Skill skill3() override;

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
private:
    //std::vector<Skill> skills;
};

class Pegasus: public Monster
{
public:
    Pegasus();
    ~Pegasus();
    Skill skill1() override;
    Skill skill2() override;
    Skill skill3() override;
private:
    //std::vector<Skill> skills;
};

class Scorpicore: public Monster
{
public:
    Scorpicore();
    ~Scorpicore();
    Skill skill1() override;
    Skill skill2() override;
    Skill skill3() override;
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
private:
    //std::vector<Skill> skills;
};

class Knight: public Monster
{
public:
    Knight();
    ~Knight();
    Skill skill1() override;
    Skill skill2() override;
    Skill skill3() override;
private:
    //std::vector<Skill> skills;
};

#endif //MONSTER_H

