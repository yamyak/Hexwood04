#include "Object.h"


Object::Object()
{
	m_id = 0;
}

Object::~Object()
{

}

int Object::GetId()
{
	return m_id;
}

void Object::SetId(int id)
{
	m_id = id;
}