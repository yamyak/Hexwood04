#include "Object.h"


Object::Object()
{

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