//
//  CallFromCPP.h
//  SimpleGame
//
//  Created by Guest on 2014/01/22.
//
//

#ifndef SimpleGame_CallFromCPP_h
#define SimpleGame_CallFromCPP_h

class CallFromCpp
{
public:
    static void ranking_query_all();
    static void ranking_post(const char *name, int score);
    static void save_display_name(const char *name);
};

#endif
