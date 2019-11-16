```
/*--------------------对战大厅client请求类型------------------------*/
登陆:
{"define" : LOG_IN, "username" : username, "password" : password}
{"define" : LOG_IN_SUCCESS/LOG_IN_FAIL_WP/LOG_IN_FAIL_AO/SERVER_ERROR}

注册:
{"define" : SIGN_UP, "username" : username, "password" : password}
{"define" : SIGN_UP_SUCCESS/SIGN_UP_FAIL/SERVER_ERROR}

获取在线用户列表:
{"define" : GET_ONLINE_LIST}
{"define" : QUERY_SUCCESS/SERVER_ERROR, "info" : [用户list]}

获取排行榜:
{"define" : GET_RANKING}
{"define" : QUERY_SUCCESS/SERVER_ERROR, "info" : [用户list]}

邀请玩家请求:
{"define" : INVITE, "username" : username}
{"define" : ACCEPT/REFUSE/SERVER_ERROR}

回复玩家邀请:
{"define" : REPLY, "username" : username}

/*--------------------对战大厅server回复类型------------------------*/
{"define" : 
LOG_IN_SUCCESS/
LOG_IN_FAIL_WP/
LOG_IN_FAIL_AO/
SIGN_UP_SUCCESS/
SIGN_UP_FAIL/
SERVER_ERROR/
QUERY_SUCCESS/
ACCEPT/
REFUSE}

/*--------------------游戏中client请求类型------------------------*/
公共header：
{"define" : OPTION, 
"choice" : ATTACK/SUMMON/ACCUMULATE/DEFEND/SKILL/SURRENDER/WIN/LOSE
}
switch(choice):
	case ATTACK:
    {"define" : OPTION, 
    "choice" : ATTACK
    "object" : POSITION_0/POSITION_1/POSITION_2/POSITION_3
    }
    case SUMMON:
    {"define" : OPTION, 
    "choice" : SUMMON
    "object" : 怪兽编号
    }
    case ACCUMULATE:
    {"define" : OPTION, 
    "choice" : ACCUMULATE
    }
    case DEFEND:
    {"define" : OPTION, 
    "choice" : DEFEND
    }
    case SKILL:
    {"define" : OPTION, 
    "choice" : SKILL
    "attacker" : POSITION_1/POSITION_2/POSITION_3
    "skill_no" : 技能编号int
    "object" : POSITION_0/POSITION_1/POSITION_2/POSITION_3
    }
    case SURRENDER:
    {"define" : OPTION, 
    "choice" : SURRENDER
    }
    case WIN:
    {"define" : OPTION, 
    "choice" : WIN
    }
    case LOSE:
    {"define" : OPTION, 
    "choice" : LOSE
    }
    


用户：
{
"username" : username,
"level" : level
}


```