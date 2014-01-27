#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

#include <vector>
using namespace std;

class RankingScene;	//–¼‘O‰ðŒˆ

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
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label_score, Label);
    int _score;
    char _score_buffer[256];
    
	//static
	/***
	static std::vector<ScoreData> vScore;
	static char label_buff[1024];
	static RankingScene *rankingScene;
	***/
    
protected:
	cocos2d::CCArray *_targets;
	cocos2d::CCArray *_projectiles;
	int _projectilesDestroyed;

	void addTarget();
    
    cocos2d::CCPoint _center;
};

#endif  // __HELLOWORLD_SCENE_H__
