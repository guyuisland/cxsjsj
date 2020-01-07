#include "fightenv.h"
#include "ui_fightenv.h"


Fightenv::Fightenv(ClientSocket *client, QWidget *parent):
    _client(client),
    _parent(parent),
    ui(new Ui::Fightenv)
{
    monsterFactory = new MonsterFactory;
    for(int i = 0; i < MONSTER.size(); i++)
        sum_times[MONSTER[i]] = monsterFactory->call_times_by_No(MONSTER[i]);
}
Fightenv::Fightenv(Player& m, Player& o)
{
    me = m;
    opponent = o;
    monsterFactory = new MonsterFactory;
}
Fightenv::~Fightenv()
{
    delete monsterFactory;
}

void Fightenv::set_players(Player m, Player o)
{
    me = m;
    opponent = o;
}

void Fightenv::dispatch(int objSel, int skiSel, int arg)
{
    if(objSel == 0)
    {
        switch (skiSel) {
            case 0:
            attack_handle(arg);
            break;
            case 1:
            defend_handle();
            break;
            case 2:
            acc_handle();
            break;
            case 3:
            summon_handle(arg);
            break;
        }
    }
    else if(objSel > 0)
    {
        skill_handle(objSel, skiSel, arg);
    }
}

void Fightenv::attack_handle(int obj)
{
    json sendInfo;
    me.attack(sendInfo);
//    switch (obj) {
//        case 0:
//            obj =  ATTACK_PLAYER;
//        break;
//        case 1:
//            obj = ATTACK_1;
//        break;
//        case 2:
//            obj = ATTACK_2;
//        break;
//        case 3:
//            obj = ATTACK_3;
//        break;
//    }
    sendInfo["object"] = obj;
    sendInfo["myName"] = me.get_name();
    sendInfo["opponent"] = opponent.get_name();
    jsBuf = sendInfo;
    send_choice(sendInfo);
}
void Fightenv::defend_handle()
{
    json sendInfo;
    me.defend(sendInfo);
    sendInfo["myName"] = me.get_name();
    sendInfo["opponent"] = opponent.get_name();
    jsBuf = sendInfo;
    send_choice(sendInfo);
}
void Fightenv::acc_handle()
{
    json sendInfo;
    me.accumulate(sendInfo);
    sendInfo["myName"] = me.get_name();
    sendInfo["opponent"] = opponent.get_name();
    jsBuf = sendInfo;
    send_choice(sendInfo);
}
void Fightenv::summon_handle(int monsNo)
{
    json sendInfo;
    me.summon(sendInfo);
    sendInfo["object"] = MONSTER[monsNo];
    sendInfo["myName"] = me.get_name();
    sendInfo["opponent"] = opponent.get_name();
    jsBuf = sendInfo;
    send_choice(sendInfo);
    sum_times[MONSTER[monsNo]] = sum_times[MONSTER[monsNo]] - 1;
}
void Fightenv::skill_handle(int monsPos, int skiNo, int obj)
{
    json sendInfo;
    me.skill(sendInfo);
    sendInfo["myName"] = me.get_name();
    sendInfo["opponent"] = opponent.get_name();
    sendInfo["attacker"] = monsPos;
    sendInfo["skill_no"] = skiNo;
    sendInfo["object"] = obj;
    jsBuf = sendInfo;
    send_choice(sendInfo);
}
void Fightenv::surrender_handle()
{
    json sendInfo;
    me.surrender(sendInfo);
    sendInfo["myName"] = me.get_name();
    sendInfo["opponent"] = opponent.get_name();
    jsBuf = sendInfo;
    send_choice(sendInfo);
}

void Fightenv::send_choice(json& sendInfo)
{
    std::string (ClientSocket::*p)(std::string) = &ClientSocket::Send_Recv;
    strWatcherPtr = new QFutureWatcher<std::string>;
    QFuture<std::string> ret = QtConcurrent::run(_client, p, sendInfo.dump());
    connect(strWatcherPtr, SIGNAL(finished()), this, SLOT(round_handle()));
    strWatcherPtr->setFuture(ret);
}


void Fightenv::round_handle(){
    json recvInfo = json::parse(strWatcherPtr->result());
    disconnect(strWatcherPtr, SIGNAL(finished()), this, SLOT(round_handle()));
    delete strWatcherPtr;
    cmp_excute(jsBuf,recvInfo);
    int res=game_over();
    if(res != 0)
    {
        json nextInfo;
        if(res==1 || res == 4){
            //send(win);
            //recv(recvBuf);
            //Sleep();
            //exit;
            nextInfo["define"] = OPTION;
            nextInfo["choice"] = WIN;
            nextInfo["myName"] = me.get_name();
            nextInfo["opponent"] = opponent.get_name();
            emit on_my_win();
            //_client->Send_Recv(jsBuf.dump());
        }
        else if(res==2 || res == 3){
            //send(lose);
            //recv(recvBuf);
            //Sleep();
            //exit;

            nextInfo["define"] = OPTION;
            nextInfo["choice"] = LOSE;
            nextInfo["myName"] = me.get_name();
            nextInfo["opponent"] = opponent.get_name();
            emit on_my_lose();
            //_client->Send_Recv(jsBuf.dump());
        }
        else if(res == 5)
        {
            //send(fair);
            //recv(recvBuf);
            //Sleep();
            //exit;

            nextInfo["define"] = OPTION;
            nextInfo["choice"] = FAIR;
            nextInfo["myName"] = me.get_name();
            nextInfo["opponent"] = opponent.get_name();
            emit on_my_lose();
            //_client->Send_Recv(jsBuf.dump());
        }
        std::string (ClientSocket::*p)(std::string) = &ClientSocket::Send_Recv;
        strWatcherPtr = new QFutureWatcher<std::string>;
        QFuture<std::string> ret = QtConcurrent::run(_client, p, nextInfo.dump());
        connect(strWatcherPtr, SIGNAL(finished()), this, SLOT(round_result()));
        strWatcherPtr->setFuture(ret);
    }
    //进入下一回合


}

void Fightenv::round_result()
{
    json recvInfo = json::parse(strWatcherPtr->result());
    disconnect(strWatcherPtr, SIGNAL(finished()), this, SLOT(round_result()));
    delete strWatcherPtr;

    if(recvInfo["define"] == OK)
    {
        //游戏结束，回到大厅
        delay(3000);
        emit on_back();
    }

}

void Fightenv::cmp_excute(json& sendInfo, json& recvInfo){
    int myChoice=sendInfo["choice"].get<int>();
    int oppoChoice=recvInfo["choice"].get<int>();
    switch (myChoice) {
    case SUMMON:{
        switch (oppoChoice) {
        case SUMMON:{
            summon_summon(sendInfo,recvInfo);
            break;
        }
        case ATTACK:{
            summon_attack(sendInfo,recvInfo);
            break;
        }
        case ACCUMULATE:{
            summon_accumulate(sendInfo,recvInfo);
            break;
        }
        case DEFEND:{
            summon_defend(sendInfo,recvInfo);
            break;
        }
        case SKILL:{
            summon_skill(sendInfo,recvInfo);
            break;
        }
        case SURRENDER:{
            summon_surrender(sendInfo,recvInfo);
            break;
        }
        default:{
            //error
            break;
        }
        }
        break;
    }
    case ACCUMULATE:{
        switch (oppoChoice) {
        case SUMMON:{
            summon_accumulate(sendInfo,recvInfo);
            break;
        }
        case ATTACK:{
            attack_accumulate(sendInfo,recvInfo);
            break;
        }
        case ACCUMULATE:{
            accumulate_accumulate(sendInfo,recvInfo);
            break;
        }
        case DEFEND:{
            accumulate_defend(sendInfo,recvInfo);
            break;
        }
        case SKILL:{
            accumulate_skill(sendInfo,recvInfo);
            break;
        }
        case SURRENDER:{
            accumulate_surrender(sendInfo,recvInfo);
            break;
        }
        default:{
            //error
            break;
        }
        }
        break;
    }
    case DEFEND:{
        switch (oppoChoice) {
        case SUMMON:{
            summon_defend(sendInfo,recvInfo);
            break;
        }
        case ATTACK:{
            attack_defend(sendInfo,recvInfo);
            break;
        }
        case ACCUMULATE:{
            accumulate_defend(sendInfo,recvInfo);
            break;
        }
        case DEFEND:{
            defend_defend(sendInfo,recvInfo);
            break;
        }
        case SKILL:{
            defend_skill(sendInfo,recvInfo);
            break;
        }
        case SURRENDER:{
            defend_surrender(sendInfo,recvInfo);
            break;
        }
        default:{
            //error
            break;
        }
        }
        break;
    }
    case SKILL:{
        switch (oppoChoice) {
        case SUMMON:{
            summon_skill(sendInfo,recvInfo);
            break;
        }
        case ATTACK:{
            attack_skill(sendInfo,recvInfo);
            break;
        }
        case ACCUMULATE:{
            accumulate_skill(sendInfo,recvInfo);
            break;
        }
        case DEFEND:{
            defend_skill(sendInfo,recvInfo);
            break;
        }
        case SKILL:{
            skill_skill(sendInfo,recvInfo);
            break;
        }
        case SURRENDER:{
            skill_surrender(sendInfo,recvInfo);
            break;
        }
        default:{
            //error
            break;
        }
        }
        break;
    }
    case ATTACK:{
        switch (oppoChoice) {
        case SUMMON:{
            summon_attack(sendInfo,recvInfo);
            break;
        }
        case ACCUMULATE:{
            attack_accumulate(sendInfo,recvInfo);
            break;
        }
        case ATTACK:{
            attack_attack(sendInfo,recvInfo);
            break;
        }
        case DEFEND:{
            attack_defend(sendInfo,recvInfo);
            break;
        }
        case SKILL:{
            attack_skill(sendInfo,recvInfo);
            break;
        }
        case SURRENDER:{
            attack_surrender(sendInfo,recvInfo);
            break;
        }
        default:{
            //error
            break;
        }
        }
        break;
    }
    case SURRENDER:{
        switch (oppoChoice) {
        case SUMMON:{
            summon_surrender(sendInfo,recvInfo);
            break;
        }
        case ACCUMULATE:{
            accumulate_surrender(sendInfo,recvInfo);
            break;
        }
        case ATTACK:{
            attack_surrender(sendInfo,recvInfo);
            break;
        }
        case DEFEND:{
            defend_surrender(sendInfo,recvInfo);
            break;
        }
        case SKILL:{
            skill_surrender(sendInfo,recvInfo);
            break;
        }
        case SURRENDER:{
            surrender_surrender(sendInfo,recvInfo);
            break;
        }
        default:{
            //error
            break;
        }
        }
        break;
    }
    default:{
        //error
        break;
    }
    }
    me.dec_round();
    opponent.dec_round();
    emit on_button_visible();
}

int Fightenv::game_over(){
    if(me.get_HP()==0){//我方死亡
        return 2;
    }
    if(opponent.get_HP()==0){//对方死亡
        return 1;
    }
    if(me.get_HP() == -1)//我方投降
    {
        if(opponent.get_HP() == -1)//对方也投降，平局
        {
            return 5;
        }
        return 3;
    }
    else{
        if(opponent.get_HP() == -1)
            return 4;
    }

    return 0;
}

void Fightenv::summon_summon(json& sendInfo, json& recvInfo){
    int my_obj = sendInfo["object"].get<int>();
    int opp_obj = recvInfo["object"].get<int>();
    int mycost = me.add_new_monster(monsterFactory->create_monster(my_obj));
    int oppocost = opponent.add_new_monster(monsterFactory->create_monster(opp_obj));
    me.dec_MP(mycost);
    opponent.dec_MP(oppocost);
    //画动画
    int my_new_slot = me.get_slot_num();
    int opp_new_slot = opponent.get_slot_num();
    emit on_update_my_MP();
    delay(1000);
    emit on_my_summon(my_new_slot, my_obj);
    delay(1000);
    emit on_update_opp_MP();
    delay(1000);
    emit on_opp_summon(opp_new_slot, opp_obj);
    delay(1000);

//    emit on_update_my_HP();
//    emit on_update_opp_HP();
}
void Fightenv::summon_attack(json& sendInfo, json& recvInfo){
    Player* att_player, *sum_player;
    int sum_obj = -1, att_obj = -1;
    int side = -1;
    if(sendInfo["choice"].get<int>()==ATTACK){
        att_player = &me;
        sum_player = &opponent;
        sum_obj = recvInfo["object"].get<int>();
        att_obj = sendInfo["object"].get<int>();
        side = 1;
    }
    else{
        att_player = &opponent;
        sum_player = &me;
        sum_obj= sendInfo["object"].get<int>();
        att_obj = recvInfo["object"].get<int>();
        side = 0;
    }
    int new_slot = sum_player->get_slot_num();
     int cost = sum_player->add_new_monster(monsterFactory->create_monster(sum_obj));
     sum_player->dec_MP(cost);
     att_player->dec_MP(1);

     if(sum_player->get_buff(REBOUND)){
         switch(att_player->rebound_aim()+ATTACK_PLAYER){
         case ATTACK_1:{
             att_player->dec_monster_HP(0);
             break;
         }
         case ATTACK_2:{
             att_player->dec_monster_HP(1);
             break;
         }
         case ATTACK_3:{
             att_player->dec_monster_HP(2);
             break;
         }
         case ATTACK_PLAYER:{
             att_player->dec_HP();
             break;
         }
         }
         //攻击反弹
         if(side)
         {
             emit on_update_opp_MP();
             delay(1000);
             emit on_opp_summon(new_slot, sum_obj);
             delay(1000);
             emit on_update_my_MP();
             delay(1000);
             emit on_my_attack(att_obj);
             delay(1000);
             emit on_opp_rebound();
             delay(1000);
             //emit on_my_monster_dead();
         }
         else
         {
             emit on_update_my_MP();
             delay(1000);
             emit on_my_summon(new_slot, sum_obj);
             delay(1000);
             emit on_update_opp_MP();
             delay(1000);
             emit on_opp_attack(att_obj);
             delay(1000);
             emit on_my_rebound();
             delay(1000);
             //emit on_opp_monster_dead();
         }

         return;
     }

     if(sum_player->get_buff(EVADE)){
         //攻击抵挡动画
         if(side)
         {
             emit on_update_opp_MP();
             delay(1000);
             emit on_opp_summon(new_slot, sum_obj);
             delay(1000);
             emit on_update_my_MP();
             delay(1000);
             emit on_my_attack(att_obj);
             delay(1000);
             emit on_opp_evade();
             delay(1000);
             //emit on_my_monster_dead();
         }
         else
         {
             emit on_update_my_MP();
             delay(1000);
             emit on_my_summon(new_slot, sum_obj);
             delay(1000);
             emit on_update_opp_MP();
             delay(1000);
             emit on_opp_attack(att_obj);
             delay(1000);
             emit on_my_evade();
             delay(1000);
             //emit on_opp_monster_dead();
         }
         return;
     }


    if(att_obj == 0)
    {
        att_obj = new_slot;
        qDebug() << "new_slot:" << new_slot;
    }


    switch (att_obj) {
    case 1:{
        sum_player->dec_monster_HP(0);
        break;
    }
    case 2:{
        sum_player->dec_monster_HP(1);
        break;
    }
    case 3:{
        sum_player->dec_monster_HP(2);
        break;
    }
    default:{
        break;
    }
    }

    if(side)
    {
        emit on_update_opp_MP();
        delay(1000);
        emit on_opp_summon(new_slot, sum_obj);
        delay(1000);
        emit on_update_my_MP();
        delay(1000);
        emit on_my_attack(att_obj);
        delay(1000);
        vector<int> deadMon;
        sum_player->get_dead_monster(deadMon);
        for(int i = 0; i < deadMon.size(); i++)
            emit on_opp_monster_dead(deadMon[i]);
        sum_player->delete_dead_monster();
        emit on_update_opp_HP();
        delay(1000);
    }
    else
    {
        emit on_update_my_MP();
        delay(1000);
        emit on_my_summon(new_slot, sum_obj);
        delay(1000);
        emit on_update_opp_MP();
        delay(1000);
        emit on_opp_attack(att_obj);
        delay(1000);
        vector<int> deadMon;
        sum_player->get_dead_monster(deadMon);
        for(int i = 0; i < deadMon.size(); i++)
            emit on_my_monster_dead(deadMon[i]);
        sum_player->delete_dead_monster();
        emit on_update_my_HP();
        delay(1000);

    }


}
void Fightenv::summon_accumulate(json& sendInfo, json& recvInfo){
    Player* sum_player, *acc_player;
    int sum_obj = -1, side = -1;
    if(sendInfo["choice"].get<int>()==SUMMON){
        sum_player = &me;
        acc_player = &opponent;
        sum_obj = sendInfo["object"].get<int>();
        side = 1;
    }
    else{
        sum_player = &opponent;
        acc_player = &me;
        sum_obj= recvInfo["object"].get<int>();
        side = 0;
    }
    int new_slot = sum_player->get_slot_num();
    int cost = sum_player->add_new_monster(monsterFactory->create_monster(sum_obj));
    sum_player->dec_MP(cost);
    acc_player->inc_MP();

    if(side)
    {
        emit on_update_my_MP();
        delay(1000);
        emit on_my_summon(new_slot, sum_obj);
        delay(1000);
        emit on_opp_acc();
        delay(1000);
        emit on_update_opp_MP();
        delay(1000);
    }
    else
    {
        emit on_update_opp_MP();
        delay(1000);
        emit on_opp_summon(new_slot, sum_obj);
        delay(1000);
        emit on_my_acc();
        delay(1000);
        emit on_update_my_MP();
        delay(1000);
    }

}
void Fightenv::summon_defend(json& sendInfo, json& recvInfo){
    Player* sum_player, *def_player;
    int sum_obj = -1, side = -1;
    if(sendInfo["choice"].get<int>()==SUMMON){
        sum_player = &me;
        def_player = &opponent;
        sum_obj = sendInfo["object"].get<int>();
        side = 1;
    }
    else{
        sum_player = &opponent;
        def_player = &me;
        sum_obj= recvInfo["object"].get<int>();
        side = 0;
    }
    int new_slot = sum_player->get_slot_num();
    int cost = sum_player->add_new_monster(monsterFactory->create_monster(sum_obj));
    sum_player->dec_MP(cost);

    if(side)
    {
        emit on_update_my_MP();
        delay(1000);
        emit on_my_summon(new_slot, sum_obj);
        delay(1000);
        emit on_opp_defend();
        delay(1000);
    }
    else
    {
        emit on_update_opp_MP();
        delay(1000);
        emit on_opp_summon(new_slot, sum_obj);
        delay(1000);
        emit on_my_defend();
        delay(1000);
    }
}
void Fightenv::summon_skill(json& sendInfo, json& recvInfo){
    Player* sum_player, *ski_player;
     int sum_obj = -1, side = -1;
     int attacker, skillNo, skiPos;
    if(sendInfo["choice"].get<int>()==SUMMON){
        sum_player = &me;
        ski_player = &opponent;
        sum_obj = sendInfo["object"].get<int>();
        side = 1;
        attacker = recvInfo["attacker"].get<int>();
        skillNo = recvInfo["skill_no"].get<int>();
        skiPos = recvInfo["object"].get<int>();
    }
    else{
        sum_player = &opponent;
        ski_player = &me;
        sum_obj= recvInfo["object"].get<int>();
        attacker = sendInfo["attacker"].get<int>();
        skillNo = sendInfo["skill_no"].get<int>();
        skiPos = sendInfo["object"].get<int>();
        side = 0;
    }
    int new_slot = sum_player->get_slot_num();
    int cost = sum_player->add_new_monster(monsterFactory->create_monster(sum_obj));
    sum_player->dec_MP(cost);

    if(side)
    {
        emit on_update_my_MP();
        delay(1000);
        emit on_my_summon(new_slot, sum_obj);
        delay(1000);
    }
    else {
        emit on_update_opp_MP();
        delay(1000);
        emit on_opp_summon(new_slot, sum_obj);
        delay(1000);
    }

    Skill ski= ski_player->pos_i_skill_j(attacker,skillNo);
    if(ski.get_evade().first==true){
        ski_player->add_buff(EVADE,ski.get_evade().second);
    }
    if(ski.get_rebound().first==true){
        ski_player->add_buff(EVADE,ski.get_rebound().second);
    }

    ski_player->dec_MP(ski.get_cost());
    if(side)
    {
        emit on_update_opp_MP();
        delay(1000);
    }
    else {
        emit on_update_my_MP();
        delay(1000);
    }


    int aim = ski_player->rebound_aim();
    if(sum_player->get_buff(REBOUND)){
        ski_player->ski_attack_monster_HP(ski,-1);
    }
    else{
        if(sum_player->get_buff(EVADE)){
            if(ski.get_penetrate()){
                switch (skiPos) {
                case 1:{
                     sum_player->ski_attack_monster_HP(ski,0);
                    break;
                }
                case 2:{
                     sum_player->ski_attack_monster_HP(ski,1);
                    break;
                }
                case 3:{
                     sum_player->ski_attack_monster_HP(ski,2);
                    break;
                }
                case ATTACK_N:{
                    break;
                }
                case ATTACK_A:{
                    sum_player->ski_attack_monster_HP(ski,1);
                    break;
                }
                }//end switch
                if(side)
                {
                    emit on_opp_skill(attacker, skillNo);
                    delay(1000);
                    emit on_update_my_HP();
                    delay(1000);
                }
                else {
                    emit on_my_skill(attacker, skillNo);
                    delay(1000);
                    emit on_update_opp_HP();
                    delay(1000);
                }
            }
            else{
                if(side)
                {
                    emit on_opp_skill(attacker, skillNo);
                    delay(1000);
                    emit on_my_defend();
                    delay(1000);
                }
                else {
                    emit on_my_skill(attacker, skillNo);
                    delay(1000);
                    emit on_opp_defend();
                    delay(1000);
                }
            }
        }
        else{
             switch (skiPos) {
            case 1:{
                 sum_player->ski_attack_monster_HP(ski,0);
                break;
            }
            case 2:{
                 sum_player->ski_attack_monster_HP(ski,1);
                break;
            }
            case 3:{
                 sum_player->ski_attack_monster_HP(ski,2);
                break;
            }
            case ATTACK_N:{
                break;
            }
            case ATTACK_A:{
                sum_player->ski_attack_monster_HP(ski,1);
                break;
            }
            }//end switch
             if(side)
             {
                 emit on_opp_skill(attacker, skillNo);
                 delay(1000);
                 emit on_update_my_HP();
                 delay(1000);
             }
             else {
                 emit on_my_skill(attacker, skillNo);
                 delay(1000);
                 emit on_update_opp_HP();
                 delay(1000);
             }
        }
    }


    ski_player->inc_MP(ski);
    ski_player->add_HP(ski,attacker);
    if(side)
    {
        emit on_update_opp_MP();
        delay(1000);
        emit on_update_opp_HP();
        delay(1000);
    }
    else {
        emit on_update_my_MP();
        delay(1000);
        emit on_update_my_HP();
        delay(1000);
    }
    vector<int> myDeadMon;
    me.get_dead_monster(myDeadMon);
    vector<int> oppDeadMon;
    opponent.get_dead_monster(oppDeadMon);
    for(int i = 0; i < myDeadMon.size(); i++)
        emit on_my_monster_dead(myDeadMon[i]);
    for(int i = 0; i < oppDeadMon.size(); i++)
        emit on_opp_monster_dead(oppDeadMon[i]);

    sum_player->delete_dead_monster();
    ski_player->delete_dead_monster();



}
void Fightenv::summon_surrender(json& sendInfo, json& recvInfo){
    Player* sum_player, *sur_player;
    int side = -1;
    if(sendInfo["choice"].get<int>()==SUMMON){
        sum_player = &me;
        sur_player = &opponent;
        side = 1;
    }
    else{
        sum_player = &opponent;
        sur_player = &me;
        side = 0;
    }

    sur_player->surrender();
    if(side)
    {
        emit on_my_win();
        //delay(3000);
    }
    else{
        emit on_my_lose();
        //delay(3000);
    }


}

void Fightenv::attack_attack(json& sendInfo, json& recvInfo){
    me.dec_MP(1);
    opponent.dec_MP(1);
    //攻击抵消动画
    int my_attk_pos = sendInfo["object"].get<int>();
    int opp_attk_pos = recvInfo["object"].get<int>();
    emit on_my_attack(my_attk_pos);
    emit on_opp_attack(opp_attk_pos);
    delay(1000);
    emit on_update_my_MP();
    emit on_update_opp_MP();
    delay(1000);
}
void Fightenv::attack_accumulate(json& sendInfo, json& recvInfo){
    Player* att_player, *acc_player;
    int obj = -1, side = -1;
    if(sendInfo["choice"].get<int>()==ATTACK){
        att_player = &me;
        acc_player = &opponent;
        obj= sendInfo["object"].get<int>();
        side = 1;
    }
    else{
        att_player = &opponent;
        acc_player = &me;
        obj= recvInfo["object"].get<int>();
        side = 0;
    }

    acc_player->inc_MP();
    att_player->dec_MP(1);
    if(side)
    {
        emit on_update_opp_MP();
        emit on_my_attack(obj);
        delay(1000);
    }
    else{
        emit on_update_my_MP();
        emit on_opp_attack(obj);
        delay(1000);
    }

    if(acc_player->get_buff(EVADE)){
        //攻击抵挡动画
        if(side)
        {
            emit on_opp_defend();
            delay(1000);
        }
        else{
            emit on_my_defend();
            delay(1000);
        }
        return;
    }
    if(acc_player->get_buff(REBOUND)){
        switch(att_player->rebound_aim()+ATTACK_PLAYER){
        case ATTACK_1:{
            att_player->dec_monster_HP(0);
            break;
        }
        case ATTACK_2:{
            att_player->dec_monster_HP(1);
            break;
        }
        case ATTACK_3:{
            att_player->dec_monster_HP(2);
            break;
        }
        case ATTACK_PLAYER:{
            att_player->dec_HP();
            break;
        }
        }
        //攻击反弹
        if(side)
        {
            emit on_opp_rebound();
            delay(1000);
            emit on_update_my_HP();
            delay(1000);
        }
        else {
            emit on_my_rebound();
            delay(1000);
            emit on_update_opp_HP();
            delay(1000);
        }
        return;
    }
    int is_mon_dead = 1;
    switch (obj) {
    case 1:{
        is_mon_dead = acc_player->dec_monster_HP(0);
        break;
    }
    case 2:{
        is_mon_dead = acc_player->dec_monster_HP(1);
        break;
    }
    case 3:{
        is_mon_dead = acc_player->dec_monster_HP(2);
        break;
    }
    case 0:{
        acc_player->dec_HP();
        break;
    }
    default:{
        break;
    }
    }

    vector<int> myDeadMon;
    me.get_dead_monster(myDeadMon);
    vector<int> oppDeadMon;
    opponent.get_dead_monster(oppDeadMon);
    for(int i = 0; i < myDeadMon.size(); i++)
        emit on_my_monster_dead(myDeadMon[i]);
    for(int i = 0; i < oppDeadMon.size(); i++)
        emit on_opp_monster_dead(oppDeadMon[i]);

    att_player->delete_dead_monster();
    acc_player->delete_dead_monster();

}


void Fightenv::attack_defend(json& sendInfo, json& recvInfo){
    Player* att_player, *def_player;
    int side = -1, obj = -1;
    if(sendInfo["choice"].get<int>()==ATTACK){
        att_player = &me;
        def_player = &opponent;
        obj = sendInfo["object"].get<int>();
        side = 1;
    }
    else{
        att_player = &opponent;
        def_player = &me;
        obj = recvInfo["object"].get<int>();
        side = 0;
    }

    att_player->dec_MP(1);

    //画动画
    if(side)
    {
        emit on_update_my_MP();
        delay(1000);
        emit on_my_attack(obj);
        delay(1000);
        emit on_opp_defend();
        delay(1000);
    }
    else {
        emit on_update_opp_MP();
        delay(1000);
        emit on_opp_attack(obj);
        delay(1000);
        emit on_my_defend();
        delay(1000);
    }
}

void Fightenv::attack_skill(json& sendInfo, json& recvInfo){
    Player* att_player, *ski_player;
    int side = -1;
    int attPos, attacker, skillNo, skiPos;
    if(sendInfo["choice"].get<int>()==ATTACK){
        att_player = &me;
        ski_player = &opponent;
        side = 1;
        attPos= sendInfo["object"].get<int>();
        attacker = recvInfo["attacker"].get<int>();
        skillNo = recvInfo["skill_no"].get<int>();
        skiPos = recvInfo["object"].get<int>();
    }
    else{
        att_player = &opponent;
        ski_player = &me;
        side = 0;
        attPos= recvInfo["object"].get<int>();
        attacker = sendInfo["attacker"].get<int>();
        skillNo = sendInfo["skill_no"].get<int>();
        skiPos = sendInfo["object"].get<int>();
    }

    Skill ski= ski_player->pos_i_skill_j(attacker,skillNo);
    att_player->dec_MP(1);
    ski_player->dec_MP(ski.get_cost());
    if(side)
    {
        emit on_update_my_MP();
        emit on_update_opp_MP();
        delay(1000);
    }
    else{
        emit on_update_opp_MP();
        emit on_update_my_MP();
        delay(1000);
    }

    if(ski.get_evade().first==true){
        ski_player->add_buff(EVADE,ski.get_evade().second);
    }
    if(ski.get_rebound().first==true){
        ski_player->add_buff(EVADE,ski.get_rebound().second);
    }



    if(att_player->get_buff(REBOUND)){
        ski_player->ski_attack_monster_HP(ski,-1);
        if(side)
        {
            emit on_opp_skill(attacker, skillNo);
            delay(1000);
            emit on_my_rebound();
            delay(1000);
            emit on_update_opp_HP();
            delay(1000);
        }
        else{
            emit on_my_skill(attacker, skillNo);
            delay(1000);
            emit on_opp_rebound();
            delay(1000);
            emit on_update_my_HP();
            delay(1000);
        }
    }
    else{
        if(att_player->get_buff(EVADE)){
            if(ski.get_penetrate()){
                switch (skiPos) {
                case 1:{
                     att_player->ski_attack_monster_HP(ski,0);
                    break;
                }
                case 2:{
                     att_player->ski_attack_monster_HP(ski,1);
                    break;
                }
                case 3:{
                     att_player->ski_attack_monster_HP(ski,2);
                    break;
                }
                case ATTACK_N:{
                    break;
                }
                case ATTACK_A:{
                    att_player->ski_attack_monster_HP(ski,1);
                    break;
                }
                }//end switch
                if(side)
                {
                    emit on_opp_skill(attacker, skillNo);
                    delay(1000);
                    emit on_update_my_HP();
                    delay(1000);
                }
                else{
                    emit on_my_skill(attacker, skillNo);
                    delay(1000);
                    emit on_update_opp_HP();
                    delay(1000);
                }
            }
            else{
                if(side)
                {
                    emit on_opp_skill(attacker, skillNo);
                    delay(1000);
                    emit on_my_evade();
                    delay(1000);
                }
                else{
                    emit on_my_skill(attacker, skillNo);
                    delay(1000);
                    emit on_opp_evade();
                    delay(1000);
                }
            }
        }
        else{
             switch (skiPos) {
            case 1:{
                 att_player->ski_attack_monster_HP(ski,0);
                break;
            }
            case 2:{
                 att_player->ski_attack_monster_HP(ski,1);
                break;
            }
            case 3:{
                 att_player->ski_attack_monster_HP(ski,2);
                break;
            }
            case ATTACK_N:{
                break;
            }
            case ATTACK_A:{
                att_player->ski_attack_monster_HP(ski,1);
                break;
            }
            }//end switch
             if(side)
             {
                 emit on_opp_skill(attacker, skillNo);
                 delay(1000);
                 emit on_update_my_HP();
                 delay(1000);
             }
             else{
                 emit on_my_skill(attacker, skillNo);
                 delay(1000);
                 emit on_update_opp_HP();
                 delay(1000);
             }
        }
    }


    if(ski_player->get_buff(EVADE)){
        //攻击抵挡动画
        if(side)
        {
            emit on_my_attack(attPos);
            delay(1000);
            emit on_opp_evade();
            delay(1000);
        }
        else{
            emit on_opp_attack(attPos);
            delay(1000);
            emit on_my_evade();
            delay(1000);
        }
    }
    else if(ski_player->get_buff(REBOUND)){
        switch(att_player->rebound_aim()+ATTACK_PLAYER){
        case ATTACK_1:{
            att_player->dec_monster_HP(0);
            break;
        }
        case ATTACK_2:{
            att_player->dec_monster_HP(1);
            break;
        }
        case ATTACK_3:{
            att_player->dec_monster_HP(2);
            break;
        }
        case ATTACK_PLAYER:{
            att_player->dec_HP();
            break;
        }
        }
        //攻击反弹
        if(side)
        {
            emit on_my_attack(attPos);
            delay(1000);
            emit on_opp_rebound();
            delay(1000);
            emit on_update_my_HP();
            delay(1000);
        }
        else{
            emit on_opp_attack(attPos);
            delay(1000);
            emit on_my_rebound();
            delay(1000);
            emit on_update_opp_HP();
            delay(1000);
        }
    }
    else{
        switch (attPos) {
        case 1:{
            ski_player->dec_monster_HP(0);
            break;
        }
        case 2:{
            ski_player->dec_monster_HP(1);
            break;
        }
        case 3:{
            ski_player->dec_monster_HP(2);
            break;
        }
        case 0:{
            ski_player->dec_HP();
            break;
        }
        default:{
            break;
        }
        }
        if(side)
        {
            emit on_my_attack(attPos);
            delay(1000);
            emit on_update_opp_HP();
            delay(1000);
        }
        else{
            emit on_opp_attack(attPos);
            delay(1000);
            emit on_update_my_HP();
            delay(1000);
        }
    }
    ski_player->monster_revive(ski);
    ski_player->inc_MP(ski);
    ski_player->add_HP(ski,attacker);
    if(side)
    {
        emit on_update_opp_MP();
        emit on_update_opp_HP();
        delay(1000);
    }
    else{
        emit on_update_my_MP();
        emit on_update_my_HP();
        delay(1000);
    }

    vector<int> myDeadMon;
    me.get_dead_monster(myDeadMon);
    vector<int> oppDeadMon;
    opponent.get_dead_monster(oppDeadMon);
    for(int i = 0; i < myDeadMon.size(); i++)
        emit on_my_monster_dead(myDeadMon[i]);
    for(int i = 0; i < oppDeadMon.size(); i++)
        emit on_opp_monster_dead(oppDeadMon[i]);

    att_player->delete_dead_monster();
    ski_player->delete_dead_monster();

    //画动画
}
void Fightenv::attack_surrender(json& sendInfo, json& recvInfo){
    Player* att_player, *sur_player;
    int side = -1;
    if(sendInfo["choice"].get<int>()==ATTACK){
        att_player = &me;
        sur_player = &opponent;
        side = 1;
    }
    else{
        att_player = &opponent;
        sur_player = &me;
        side = 0;
    }

    sur_player->surrender();

    if(side)
    {
        emit on_my_win();
        //delay(3000);
    }
    else{
        emit on_my_lose();
        //delay(3000);
    }
}

void Fightenv::accumulate_accumulate(json& sendInfo, json& recvInfo){
    me.inc_MP();
    opponent.inc_MP();

    emit on_my_acc();
    emit on_update_my_MP();
    emit on_opp_acc();
    emit on_update_opp_MP();
    delay(1000);
}
void Fightenv::accumulate_defend(json& sendInfo, json& recvInfo){
    Player* acc_player, *def_player;
    int side = -1;
    if(sendInfo["choice"].get<int>()==ACCUMULATE){
        acc_player = &me;
        def_player = &opponent;
        side = 1;
    }
    else{
        acc_player = &opponent;
        def_player = &me;
        side = 0;
    }

    acc_player->inc_MP();
    if(side)
    {
        emit on_my_acc();
        emit on_opp_defend();
        delay(1000);
        emit on_update_my_MP();
        delay(1000);
    }
    else
    {
        emit on_opp_acc();
        emit on_my_defend();
        delay(1000);
        emit on_update_opp_MP();
        delay(1000);
    }
}
void Fightenv::accumulate_skill(json& sendInfo, json& recvInfo){
    Player* acc_player, *ski_player;
    int side = -1;
    int attacker, skillNo, skiPos;
    if(sendInfo["choice"].get<int>()==ACCUMULATE){
        acc_player = &me;
        ski_player = &opponent;
        side = 1;
        attacker = recvInfo["attacker"].get<int>();
        skillNo = recvInfo["skill_no"].get<int>();
        skiPos = recvInfo["object"].get<int>();
    }
    else{
        acc_player = &opponent;
        ski_player = &me;
        side = 0;
        attacker = sendInfo["attacker"].get<int>();
        skillNo = sendInfo["skill_no"].get<int>();
        skiPos = sendInfo["object"].get<int>();
    }

    Skill ski= ski_player->pos_i_skill_j(attacker,skillNo);
    acc_player->inc_MP();
    ski_player->dec_MP(ski.get_cost());
    if(side)
    {
        emit on_update_my_MP();
        emit on_update_opp_MP();
        delay(1000);
    }
    else{
        emit on_update_opp_MP();
        emit on_update_my_MP();
        delay(1000);
    }

    if(ski.get_evade().first==true){
        ski_player->add_buff(EVADE,ski.get_evade().second);
    }
    if(ski.get_rebound().first==true){
        ski_player->add_buff(EVADE,ski.get_rebound().second);
    }




    if(acc_player->get_buff(REBOUND)){
        ski_player->ski_attack_monster_HP(ski,-1);
        if(side)
        {
            emit on_opp_skill(attacker, skillNo);
            delay(1000);
            emit on_my_rebound();
            delay(1000);
            emit on_update_opp_HP();
            delay(1000);
        }
        else{
            emit on_my_skill(attacker, skillNo);
            delay(1000);
            emit on_opp_rebound();
            delay(1000);
            emit on_update_my_HP();
            delay(1000);
        }
    }
    else{
        if(acc_player->get_buff(EVADE)){
            if(ski.get_penetrate()){
                switch (skiPos) {
                case 1:{
                     acc_player->ski_attack_monster_HP(ski,0);
                    break;
                }
                case 2:{
                     acc_player->ski_attack_monster_HP(ski,1);
                    break;
                }
                case 3:{
                     acc_player->ski_attack_monster_HP(ski,2);
                    break;
                }
                case ATTACK_N:{
                    break;
                }
                case ATTACK_A:{
                    acc_player->ski_attack_monster_HP(ski,1);
                    break;
                }
                }//end switch
                if(side)
                {
                    emit on_opp_skill(attacker, skillNo);
                    delay(1000);
                    emit on_update_my_HP();
                    delay(1000);
                }
                else{
                    emit on_my_skill(attacker, skillNo);
                    delay(1000);
                    emit on_update_opp_HP();
                    delay(1000);
                }
            }
            else{
                if(side)
                {
                    emit on_opp_skill(attacker, skillNo);
                    delay(1000);
                    emit on_my_evade();
                    delay(1000);
                }
                else{
                    emit on_my_skill(attacker, skillNo);
                    delay(1000);
                    emit on_opp_evade();
                    delay(1000);
                }
            }

        }
        else{
             switch (skiPos) {
            case 1:{
                 acc_player->ski_attack_monster_HP(ski,0);
                break;
            }
            case 2:{
                 acc_player->ski_attack_monster_HP(ski,1);
                break;
            }
            case 3:{
                 acc_player->ski_attack_monster_HP(ski,2);
                break;
            }
            case ATTACK_N:{
                break;
            }
            case ATTACK_A:{
                acc_player->ski_attack_monster_HP(ski,1);
                break;
            }
            }//end switch
             if(side)
             {
                 emit on_opp_skill(attacker, skillNo);
                 delay(1000);
                 emit on_update_my_HP();
                 delay(1000);
             }
             else{
                 emit on_my_skill(attacker, skillNo);
                 delay(1000);
                 emit on_update_opp_HP();
                 delay(1000);
             }
        }
    }

    ski_player->inc_MP(ski);
    ski_player->add_HP(ski,attacker);

    if(side)
    {
        emit on_update_opp_MP();
        emit on_update_opp_HP();
        delay(1000);
    }
    else{
        emit on_update_my_MP();
        emit on_update_my_HP();
        delay(1000);
    }

    vector<int> myDeadMon;
    me.get_dead_monster(myDeadMon);
    vector<int> oppDeadMon;
    opponent.get_dead_monster(oppDeadMon);
    for(int i = 0; i < myDeadMon.size(); i++)
        emit on_my_monster_dead(myDeadMon[i]);
    for(int i = 0; i < oppDeadMon.size(); i++)
        emit on_opp_monster_dead(oppDeadMon[i]);


    acc_player->delete_dead_monster();
    ski_player->delete_dead_monster();

}
void Fightenv::accumulate_surrender(json& sendInfo, json& recvInfo){
    Player* acc_player, *sur_player;
    int side = -1;
    if(sendInfo["choice"].get<int>()==ACCUMULATE){
        acc_player = &me;
        sur_player = &opponent;
        side = 1;
    }
    else{
        acc_player = &opponent;
        sur_player = &me;
        side = 0;
    }

    sur_player->surrender();

    if(side)
    {
        emit on_my_win();
        //delay(3000);
    }
    else{
        emit on_my_lose();
        //delay(3000);
    }

}

void Fightenv::defend_defend(json& sendInfo, json& recvInfo){
    //动画

    emit on_my_defend();
    emit on_opp_defend();
    delay(1000);
}
void Fightenv::defend_skill(json& sendInfo, json& recvInfo){
    Player* def_player, *ski_player;
    int side = -1, attacker, skillNo, skiPos;
    if(sendInfo["choice"].get<int>()==DEFEND){
        def_player = &me;
        ski_player = &opponent;
        side = 1;
        attacker = recvInfo["attacker"].get<int>();
        skillNo = recvInfo["skill_no"].get<int>();
        skiPos = recvInfo["object"].get<int>();
    }
    else{
        def_player = &opponent;
        ski_player = &me;
        side = 0;
        attacker = sendInfo["attacker"].get<int>();
        skillNo = sendInfo["skill_no"].get<int>();
        skiPos = sendInfo["object"].get<int>();
    }

    Skill ski= ski_player->pos_i_skill_j(attacker,skillNo);
    if(ski.get_evade().first==true){
        ski_player->add_buff(EVADE,ski.get_evade().second);
    }
    if(ski.get_rebound().first==true){
        ski_player->add_buff(REBOUND,ski.get_rebound().second);
    }
    ski_player->dec_MP(ski.get_cost());

    if(side)
    {
        emit on_update_opp_MP();
        delay(1000);
    }
    else{
        emit on_update_my_MP();
        delay(1000);
    }

    if(def_player->get_buff(REBOUND)){
        ski_player->ski_attack_monster_HP(ski,-1);
        if(side)
        {
            emit on_opp_skill(attacker, skillNo);
            delay(1000);
            emit on_my_rebound();
            delay(1000);
            emit on_update_opp_HP();
            delay(1000);
        }
        else{
            emit on_my_skill(attacker, skillNo);
            delay(1000);
            emit on_opp_rebound();
            delay(1000);
            emit on_update_my_HP();
            delay(1000);
        }
    }
    else{
        if(ski.get_penetrate()){
            switch (skiPos) {
            case 1:{
                 def_player->ski_attack_monster_HP(ski,0);
                break;
            }
            case 2:{
                 def_player->ski_attack_monster_HP(ski,1);
                break;
            }
            case 3:{
                 def_player->ski_attack_monster_HP(ski,2);
                break;
            }
            case ATTACK_N:{
                break;
            }
            case ATTACK_A:{
                def_player->ski_attack_monster_HP(ski,1);
                break;
            }
            }//end switch
            if(side)
            {
                emit on_opp_skill(attacker, skillNo);
                delay(1000);
                emit on_update_my_HP();
                delay(1000);
            }
            else{
                emit on_my_skill(attacker, skillNo);
                delay(1000);
                emit on_update_opp_HP();
                delay(1000);
            }
        }
        else{
            if(side)
            {
                emit on_opp_skill(attacker, skillNo);
                delay(1000);
                emit on_my_defend();
                delay(1000);
            }
            else{
                emit on_my_skill(attacker, skillNo);
                delay(1000);
                emit on_opp_defend();
                delay(1000);
            }
        }

    }

    ski_player->inc_MP(ski);
    ski_player->add_HP(ski,attacker);

    if(side)
    {
        emit on_update_opp_MP();
        emit on_update_opp_HP();
        delay(1000);
    }
    else{
        emit on_update_my_MP();
        emit on_update_my_HP();
        delay(1000);
    }

    vector<int> myDeadMon;
    me.get_dead_monster(myDeadMon);
    vector<int> oppDeadMon;
    opponent.get_dead_monster(oppDeadMon);
    for(int i = 0; i < myDeadMon.size(); i++)
        emit on_my_monster_dead(myDeadMon[i]);
    for(int i = 0; i < oppDeadMon.size(); i++)
        emit on_opp_monster_dead(oppDeadMon[i]);


    def_player->delete_dead_monster();
    ski_player->delete_dead_monster();
}
void Fightenv::defend_surrender(json& sendInfo, json& recvInfo){
    Player* def_player, *sur_player;
    int side = -1;
    if(sendInfo["choice"].get<int>()==DEFEND){
        def_player = &me;
        sur_player = &opponent;
        side = 1;
    }
    else{
        def_player = &opponent;
        sur_player = &me;
        side = 0;
    }

    sur_player->surrender();

    if(side)
    {
        emit on_opp_surrender();
        delay(1000);
        emit on_my_win();
        //delay(3000);
    }
    else{
        emit on_my_surrender();
        delay(1000);
        emit on_my_lose();
        //delay(3000);
    }
}

void Fightenv::skill_skill(json& sendInfo, json& recvInfo){
    int myAttPos = sendInfo["attacker"].get<int>();
    int oppoAttPos = recvInfo["attacker"].get<int>();
    int mySkiNo = sendInfo["skill_no"].get<int>();
    int oppoSkiNo = recvInfo["skill_no"].get<int>();
    int myPos = sendInfo["object"].get<int>();
    int oppoPos = recvInfo["object"].get<int>();

    Skill mySkill = me.pos_i_skill_j(myAttPos,mySkiNo);
    Skill oppoSkill = opponent.pos_i_skill_j(oppoAttPos,oppoSkiNo);

    if(mySkill.get_evade().first){
        me.add_buff(EVADE,mySkill.get_evade().second);
    }
    if(mySkill.get_rebound().first){
        me.add_buff(REBOUND,mySkill.get_rebound().second);
    }
    if(oppoSkill.get_evade().first){
        opponent.add_buff(EVADE,oppoSkill.get_evade().second);
    }
    if(oppoSkill.get_rebound().first){
        opponent.add_buff(REBOUND,oppoSkill.get_rebound().second);
    }

    me.dec_MP(mySkill.get_cost());
    opponent.dec_MP(oppoSkill.get_cost());

    emit on_update_my_MP();
    emit on_update_opp_MP();
    delay(1000);


    if(opponent.get_buff(REBOUND)){
        me.ski_attack_monster_HP(mySkill,-1);

        emit on_my_skill(myAttPos, mySkiNo);
        delay(1000);
        emit on_opp_rebound();
        delay(1000);
        emit on_update_my_HP();
        delay(1000);
    }
    else{
        if(opponent.get_buff(EVADE)){
            if(mySkill.get_penetrate()){
                switch (myPos) {
                case 1:{
                     opponent.ski_attack_monster_HP(mySkill,0);
                    break;
                }
                case 2:{
                     opponent.ski_attack_monster_HP(mySkill,1);
                    break;
                }
                case 3:{
                     opponent.ski_attack_monster_HP(mySkill,2);
                    break;
                }
                case ATTACK_N:{
                    break;
                }
                case ATTACK_A:{
                    opponent.ski_attack_monster_HP(mySkill,1);
                    break;
                }
                }//end switch
                emit on_my_skill(myAttPos, mySkiNo);
                delay(1000);
                emit on_update_opp_HP();
                delay(1000);
            }
            else{
                emit on_my_skill(myAttPos, mySkiNo);
                delay(1000);
                emit on_opp_evade();
                delay(1000);
            }

        }
        else{
             switch (myPos) {
            case 1:{
                 opponent.ski_attack_monster_HP(mySkill,0);
                break;
            }
            case 2:{
                 opponent.ski_attack_monster_HP(mySkill,1);
                break;
            }
            case 3:{
                 opponent.ski_attack_monster_HP(mySkill,2);
                break;
            }
            case ATTACK_N:{
                break;
            }
            case ATTACK_A:{
                opponent.ski_attack_monster_HP(mySkill,1);
                break;
            }
            }//end switch
        }
        emit on_my_skill(myAttPos, mySkiNo);
        delay(1000);
        emit on_update_opp_HP();
        delay(1000);
    }

    if(me.get_buff(REBOUND)){
        opponent.ski_attack_monster_HP(oppoSkill,-1);
        emit on_opp_skill(myAttPos, oppoSkiNo);
        delay(1000);
        emit on_my_rebound();
        delay(1000);
        emit on_update_opp_HP();
        delay(1000);
    }
    else{
        if(me.get_buff(EVADE)){
            if(oppoSkill.get_penetrate()){
                switch (oppoPos) {
                case 1:{
                     me.ski_attack_monster_HP(oppoSkill,0);
                    break;
                }
                case 2:{
                     me.ski_attack_monster_HP(oppoSkill,1);
                    break;
                }
                case 3:{
                     me.ski_attack_monster_HP(oppoSkill,2);
                    break;
                }
                case ATTACK_N:{
                    break;
                }
                case ATTACK_A:{
                    me.ski_attack_monster_HP(oppoSkill,1);
                    break;
                }
                }//end switch
                emit on_opp_skill(myAttPos, oppoSkiNo);
                delay(1000);
                emit on_update_my_HP();
                delay(1000);
            }
            else{
                emit on_opp_skill(myAttPos, oppoSkiNo);
                delay(1000);
                emit on_my_evade();
                delay(1000);
            }

        }
        else{
             switch (oppoPos) {
            case 1:{
                 me.ski_attack_monster_HP(oppoSkill,0);
                break;
            }
            case 2:{
                 me.ski_attack_monster_HP(oppoSkill,1);
                break;
            }
            case 3:{
                 me.ski_attack_monster_HP(oppoSkill,2);
                break;
            }
            case ATTACK_N:{
                break;
            }
            case ATTACK_A:{
                me.ski_attack_monster_HP(oppoSkill,1);
                break;
            }
            }//end switch
             emit on_opp_skill(myAttPos, oppoSkiNo);
             delay(1000);
             emit on_update_my_HP();
             delay(1000);
        }
    }
    me.monster_revive(mySkill);
    opponent.monster_revive(oppoSkill);


    me.inc_MP(mySkill);
    opponent.inc_MP(oppoSkill);
    me.add_HP(mySkill,myAttPos);
    opponent.add_HP(oppoSkill,oppoAttPos);


    emit on_update_my_MP();
    emit on_update_my_HP();
    emit on_update_opp_MP();
    emit on_update_opp_HP();
    delay(1000);

    vector<int> myDeadMon;
    me.get_dead_monster(myDeadMon);
    vector<int> oppDeadMon;
    opponent.get_dead_monster(oppDeadMon);
    for(int i = 0; i < myDeadMon.size(); i++)
        emit on_my_monster_dead(myDeadMon[i]);
    for(int i = 0; i < oppDeadMon.size(); i++)
        emit on_opp_monster_dead(oppDeadMon[i]);

    me.delete_dead_monster();
    opponent.delete_dead_monster();

}
void Fightenv::skill_surrender(json& sendInfo, json& recvInfo){
    Player* sur_player, *ski_player;
    int side = -1;
    if(sendInfo["choice"].get<int>()==SURRENDER){
        sur_player = &me;
        ski_player = &opponent;
        side = 1;
    }
    else{
        sur_player = &opponent;
        ski_player = &me;
        side = 0;
    }

    sur_player->surrender();

    if(side)
    {
        emit on_my_surrender();
        delay(1000);
        emit on_my_lose();
        //delay(3000);
    }
    else{
        emit on_opp_surrender();
        delay(1000);
        emit on_my_win();
        //delay(3000);
    }
}

void Fightenv::surrender_surrender(json& sendInfo, json& recvInfo){
    me.surrender();
    opponent.surrender();

    emit on_my_lose();
    delay(3000);
}

int Fightenv::has_obj(int monsterPos, int skiPos)
{
    return  me.has_obj(monsterPos, skiPos);
}

int Fightenv::verify_option(int objSelect, int skiSelect, int eneMonsSelect)
{
    //1可以释放, 2MP不足， 3怪兽已满，4达到召唤上限次数,5非法操作(技能释放对象选择玩家本体)
    //召唤，判断现有怪兽个数和召唤上限
    if(objSelect == 0)
    {
        if(skiSelect == 0)
        {
            if(me.get_MP() < 1)
                return 2;
        }
        else if(skiSelect == 3)
        {
            if(me.monster_num() == 3)
                return 3;
            else if(sum_times[MONSTER[eneMonsSelect]] <= 0){
                return 4;
            }
            else if(me.get_MP() < monsterFactory->mons_called_MP(MONSTER[eneMonsSelect])){//判断MP是否足够
                return 2;
            }
        }
        return 1;
    }
    //技能，判断MP是否足够
    int monsNo = -1;
    std::vector<int> myMon(4, 0);
    me.get_mon_No(myMon);
    monsNo = myMon[objSelect];
    if(me.get_MP() < monsterFactory->mons_skill_MP(monsNo, skiSelect))
    {
        return 2;
    }
    return 1;
}

void Fightenv::get_my_monster(vector<int>& myMon)
{
    myMon.resize(4);
    myMon[0] = -1;
    me.get_mon_No(myMon);
}
void Fightenv::get_opp_monster(vector<int>& oppMon)
{
    oppMon.resize(4);
    oppMon[0] = 0;
    opponent.get_mon_No(oppMon);
}//int oppMonster[4];//对方什么位置是什么怪物
void Fightenv::get_my_blood(vector<int>& myHp)
{
    myHp.resize(4);
    me.get_HP_vec(myHp);
}//int myBlood[4];
void Fightenv::get_opp_blood(vector<int>& oppHp)
{
    oppHp.resize(4);
    opponent.get_HP_vec(oppHp);
}//int oppBlood[4];
void Fightenv::get_MP(vector<int>& vecMp)
{
    vecMp.resize(2);
    vecMp[0] = me.get_MP();
    vecMp[1] = opponent.get_MP();
}
int Fightenv::mon_maxHP(int monsNo)
{
    monsterFactory->mon_maxHP(monsNo);
}
void Fightenv::delay(int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}
