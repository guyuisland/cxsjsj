#ifndef PROTOCOL_H
#define PROTOCOL_H
#include<vector>

/*--------------------请求类型------------------------*/
#define LOG_IN              2       // 登陆请求
#define SIGN_UP             3       // 注册请求
#define GET_WAITING_LIST     4       // 获取在线列表请求
#define GET_RANKING       5       // 获取排行榜请求
#define READY       7       //进入等待室
#define INVITE      8       // 邀请玩家请求
#define REPLY      9       // 回复邀请

/*--------------------游戏中请求类型------------------------*/
#define OPTION      20       // 发送我的选择
#define ATTACK     21      // 攻击
#define SUMMON      22      // 召唤
#define ACCUMULATE      23       // 蓄气
#define DEFEND      24       // 防御
#define SURRENDER      25      // 投降
#define WIN         26      //胜利
#define LOSE        27      //失败
#define FAIR        28      //平局
#define SKILL       29      //使用技能

#define ATTACK_PLAYER      30       // 攻击玩家本体
#define ATTACK_1      31       // 攻击1号位置的怪兽
#define ATTACK_2      32       // 攻击2号位置的怪兽
#define ATTACK_3      33      // 攻击3号位置的怪兽
#define ATTACK_N   34      // 没有攻击对象
#define ATTACK_A    35      // 群体攻击



/*--------------------玩家状态------------------------*/
#define EVADE         44      //闪避
#define REBOUND         45      //反弹





/*--------------------返回状态------------------------*/
#define LOG_IN_SUCCESS      65      // 登陆成功
#define LOG_IN_FAIL_WP      66      // 登陆失败，密码错误
#define LOG_IN_FAIL_AO      67      // 登陆失败，用户已经在线

#define SIGN_UP_SUCCESS     68      // 注册成功
#define SIGN_UP_FAIL        69      // 注册失败

#define SERVER_ERROR        50       // 服务器错误
#define QUERY_SUCCESS       51       // 查询成功
#define ACCEPT              52       // 请求成功处理
#define REFUSE       53       // 拒绝（邀请）请求
#define OFFLINE      54       //对方已经下线
#define BUSY                55       //玩家已被邀请

#define OK                  66      //ok

/*--------------------游戏中回复类型------------------------*/
#define RESULT      70       // 返回结果

/*--------------------怪物类型编号------------------------*/
#define FLAREON        100
#define PIKACHU        101
#define GRENINJA     102
#define DRAGONITE         103

const std::vector<int> MONSTER = {FLAREON, PIKACHU, GRENINJA, DRAGONITE};



#endif // PROTOCOL_H
