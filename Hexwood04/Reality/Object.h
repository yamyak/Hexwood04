#pragma once

class Object
{
public:
	Object();
	~Object();

	virtual bool Run() = 0;
	int GetId();
	void SetId(int id);

private:
	int m_id;
};

