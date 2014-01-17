#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

#include <vector>
using namespace std;

class Demo {
public:
	double d1,d2;
	std::string name;
	std::string score;

	Demo(double x) { d1 = x; }
	Demo(double x, double y) { d1 = x; d2 = y; }
	Demo(std::string a){ name = a; }
	Demo(std::string a, std::string b){ name = a; score=b;}
};


class ScoreData {
public:

	std::string name;
	std::string score;

	ScoreData(std::string a, std::string b){ name = a; score=b;}
};


/***
class ScoreData
{
public:
	std::string name;
	std::string score;

	//ScoreData(){ name=0; score=0; }

	ScoreData( std::string a, std::string b ){ name = a; score = b;_}

};
***/


class HelloWorld : public cocos2d::CCLayerColor
{
public:
	HelloWorld();
	~HelloWorld();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, 
    // instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	virtual void menuCloseCallback(cocos2d::CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(HelloWorld);

	void spriteMoveFinished(cocos2d::CCNode* sender);

	void gameLogic(float dt);

	void updateGame(float dt);

	void registerWithTouchDispatcher();
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    
	//static
	static std::vector<ScoreData> vScore;
	static char label_buff[1024];
    
protected:
	cocos2d::CCArray *_targets;
	cocos2d::CCArray *_projectiles;
	int _projectilesDestroyed;

	void addTarget();


};

#endif  // __HELLOWORLD_SCENE_H__
