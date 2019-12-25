#ifndef SKILL_H
#define SKILL_H

#include<string>
#include<QString>
class Skill
{
public:
    Skill();
    Skill(int ,std::pair<bool,int> ,std::pair<bool,int> ,QString, QString, bool ,bool ,bool ,bool , bool, bool,int ,int);
    ~Skill();
    Skill(const Skill& copied);
    void set_describe(std::string str);

    int get_cost();
    int get_damage();
    int get_heal();
    bool get_revive();
    bool get_group();
    bool get_penetrate();
    bool get_addMP();
    bool get_crit();

    std::pair<bool,int> get_evade();
    std::pair<bool,int> get_rebound();
    QString get_name();
    QString get_describe();
private:
    //std::string describe;
    int costEnergy;//消耗能量
    bool group;//群体
    bool kill;//能否对玩家造成伤害
    bool penetrate;//穿透
    bool crit;//暴击
    bool addMP;//增加能量
    bool revive;//复活
    int damage;//伤害
    int heal;//治愈
    QString name;//技能名称
    QString description;//技能描述
    std::pair<bool,int> evade;//闪避
    std::pair<bool,int> rebound;//反弹
};

#endif // SKILL_H
