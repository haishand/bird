#include "cocos2d.h"
#include "BirdScene.h"
#include "Resource.h"

USING_NS_CC;

Scene* BirdScene::createScene()
{
	auto scene = Scene::create();
	auto layer = BirdScene::create();
	scene->addChild(layer);
	return scene;
}

bool BirdScene::init()
{
	if (!Layer::init()) {
		return false;
	}
    
	BirdScene::initUI();

	this->scheduleUpdate();

	// touch
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesEnded = CC_CALLBACK_2(BirdScene::onTouchesEnded, this);
	listener->onTouchesBegan = CC_CALLBACK_2(BirdScene::onTouchesBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void BirdScene::initUI()
{

	// win size
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// game background
	auto bg = Sprite::create(RES_BIRD_BG);
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
	this->addChild(bg);

	// logo
	auto logo = Sprite::create(RES_BIRD_LOGO);
	logo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	logo->setTag(TAG_START_LOGO);
	this->addChild(logo);

	// game over logo
	auto overLogo = Sprite::create(RES_BIRD_OVER_LOGO);
	overLogo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	overLogo->setTag(TAG_OVER_LOGO);
	overLogo->setVisible(false);
	this->addChild(overLogo);

	// start button
	auto startBtnItem = MenuItemImage::create(RES_START_BTN, RES_START_BTN_PRESSED, CC_CALLBACK_0(BirdScene::startGame, this));
	auto menuStart = Menu::create(startBtnItem, NULL);
	menuStart->setTag(TAG_BTN_START);
	menuStart->setPosition(Vec2(visibleSize.width/2, startBtnItem->getContentSize().height));
	menuStart->setVisible(true);
	this->addChild(menuStart);

	// close button
	auto closeItem = MenuItemImage::create(
		"bird/CloseNormal.png",
		"bird/CloseSelected.png",
		CC_CALLBACK_1(BirdScene::menuCloseCallback, this));
	auto menuClose = Menu::create(closeItem, NULL);
	menuClose->setPosition(Vec2(visibleSize.width / 2, startBtnItem->getContentSize().height));
	menuClose->setVisible(false);
	menuClose->setTag(TAG_BTN_CLOSE);
	this->addChild(menuClose);

	// hero
	auto hero = Sprite::create(RES_BIRD_HERO);
	hero->setPosition(visibleSize.width / 5, visibleSize.height*0.8);
	hero->setVisible(false);
	hero->setTag(TAG_HERO);
	this->addChild(hero);

	Animation* anim = Animation::create();
	anim->addSpriteFrameWithFile(RES_BIRD_HERO);
	anim->addSpriteFrameWithFile(RES_BIRD_HERO2);
	anim->addSpriteFrameWithFile(RES_BIRD_HERO3);
	anim->setDelayPerUnit(0.06f);
	anim->setLoops(-1);
	Animate* animate = Animate::create(anim);
	hero->runAction(animate);

	// pipe
	pipe = new Pipe();
	this->addChild(pipe);

}

void BirdScene::startGame()
{
	gameStatus = GAME_START;

}

void BirdScene::update(float dt)
{
	auto hero = this->getChildByTag(TAG_HERO);
	auto logo = this->getChildByTag(TAG_START_LOGO);
	auto overLogo = this->getChildByTag(TAG_OVER_LOGO);
	auto btnClose = this->getChildByTag(TAG_BTN_CLOSE);
	auto btnStart = this->getChildByTag(TAG_BTN_START);
	auto visibleSize = Director::getInstance()->getVisibleSize();

	switch (gameStatus) {
	case GAME_START:
		hero->setVisible(true);
		btnStart->setVisible(false);
		logo->setVisible(false);
		overLogo->setVisible(false);
		btnClose->setVisible(false);
		gameStatus = GAME_PLAY;
		break;
	case GAME_PLAY:
		// update pipe
		pipe->update();

		// update hero
		if (hero->getPositionY() >= 0 && hero->getPositionY() <= visibleSize.height) {
			velocity -= gravity;
			hero->setPositionY(hero->getPositionY() + velocity);
		}
		else {
			gameStatus = GAME_OVER;
		}

		// check collision
		for (int i = 0; i < pipe->pipeList->count(); i++) {
			Sprite* pipeSprite = (Sprite *)pipe->pipeList->getObjectAtIndex(i);
			bool isCollided = hero->getBoundingBox().intersectsRect(pipeSprite->getBoundingBox());
			if (isCollided) {
				gameStatus = GAME_OVER;
				break;
			}
		}

		// score

		break;
	case GAME_RESTART:
		// clear objs
		pipe->removeAllChildren();
		pipe->pipeList->removeAllObjects();

		// reset
		hero->setPosition(visibleSize.width / 5, visibleSize.height*0.8);
		gameStatus = GAME_START;
		break;
	case GAME_OVER:
		this->getChildByTag(TAG_OVER_LOGO)->setVisible(true);
		this->getChildByTag(TAG_BTN_CLOSE)->setVisible(true);
		break;
	}

}

void BirdScene::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	switch (gameStatus) {
	case GAME_PLAY:
		this->velocity = 5;
		break;
	case GAME_OVER:
		this->gameStatus = GAME_RESTART;
		break;
	}
}

void BirdScene::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
	// nothing
}

void BirdScene::menuCloseCallback(Ref* pSender)
{
    delete this->pipe;
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}