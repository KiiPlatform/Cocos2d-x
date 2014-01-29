//
//  KillIF.h
//  SimpleGame
//
//  Created by Guest on 2014/01/22.
//
//

#ifndef SimpleGame_KillIF_h
#define SimpleGame_KillIF_h

#include "cocos2d.h"

//#include <vector>

class ScoreData {
public:
    
	std::string name;
	std::string score;
    
	ScoreData(std::string a, std::string b){ name = a; score=b;}
};

#endif
