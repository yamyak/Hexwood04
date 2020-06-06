#pragma once

class Object
{
public:
	Object();
	~Object();

	virtual bool Run() = 0;
};

