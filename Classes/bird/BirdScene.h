#include "cocos2d.h"
#include "BirdDef.h"
#include "Pipe.h"

class BirdScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(BirdScene);
	void initUI();
	void startGame();
	void update(float dt);
	void menuCloseCallback(Ref* pSender);
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	int gameStatus;
	float velocity = -2.0f;
	float gravity = 0.2f;
	Pipe* pipe;
};