#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
//#include "test/CKiiApiTest.h"

#include "kiiRanking/KiiRanking.h"	//Android
//#include "KiiRanking.h"	//iPhone

//#include "SimpleAudioEngine.h"


class HelloWorld : public cocos2d::LayerColor
{
public:
	HelloWorld();
	~HelloWorld();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, 
    // instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::Scene* scene();

	// a selector callback
	virtual void menuCloseCallback(cocos2d::Object* sender);

	// implement the "static node()" method manually
	CREATE_FUNC(HelloWorld);

	void spriteMoveFinished(cocos2d::Node* sender);

	void gameLogic(float dt);

	void updateGame(float dt);

	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    
    //追加
    void saveHIScore(int socre);
    CC_SYNTHESIZE_READONLY(cocos2d::LabelTTF*, _label_score, Label);
    int _score,_hiScore;
    char _score_buffer[256];
    cocos2d::CCUserDefault* _userDefault;
    
    KiiRanking *_pKiiRanking;
    
protected:
	cocos2d::Array *_targets;
	cocos2d::Array *_projectiles;
	int _projectilesDestroyed;

	void addTarget();


};

#endif  // __HELLOWORLD_SCENE_H__
