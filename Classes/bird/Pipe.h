#include "cocos2d.h"
#include "Resource.h"

class Pipe:public cocos2d::Node
{
public:
	Pipe();
	cocos2d::Array* pipeList;
	virtual void onEnter();
	void update();
	int count = 0;
	~Pipe();
	void addOne();
};

