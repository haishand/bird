#include "Pipe.h"
#include "BirdDef.h"

USING_NS_CC;

Pipe::Pipe()
{
	Pipe::pipeList = Array::create();
	pipeList->retain();
}

void Pipe::onEnter()
{

}

void Pipe::update()
{
	++count;
	if (60 == count) {
		addOne();
		count = 0;
	}

	// update rest and remove unseen pipes
	for (int i = pipeList->count() - 1; i >= 0; i--) {
		Sprite* pipe = (Sprite*)pipeList->getObjectAtIndex(i);
		pipe->setPositionX(pipe->getPositionX() - 3);
		if (pipe->getPositionX() < 0) {
			pipeList->fastRemoveObjectAtIndex(i);
			this->removeChild(pipe);
		}
	}

}

Pipe::~Pipe()
{
	if (pipeList != nullptr)
		pipeList->release();
}

void Pipe::addOne()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto pipeUp = Sprite::create(RES_PIPE_UP);
	auto pipeDown = Sprite::create(RES_PIPE_DOWN);
	pipeList->addObject(pipeUp);
	pipeList->addObject(pipeDown);
	this->addChild(pipeUp);
	this->addChild(pipeDown);

	float pipeUpX = visibleSize.width + pipeUp->getContentSize().width/2;
	float pipeUpY = visibleSize.height - CCRANDOM_0_1() * pipeUp->getContentSize().height/2;
	pipeUp->setPosition(Vec2(pipeUpX, pipeUpY));

	float pipeDownX = pipeUpX;
	float pipeDownY = 0 + CCRANDOM_0_1() * pipeDown->getContentSize().height / 2;
	pipeDown->setPosition(Vec2(pipeDownX, pipeDownY));

}
