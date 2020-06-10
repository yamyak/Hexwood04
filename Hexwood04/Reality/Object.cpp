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

void Object::Lock()
{
	std::unique_lock<std::mutex> lock(m_object_mutex);
	m_lock.swap(lock);
}

void Object::Unlock()
{
	if (m_lock.owns_lock())
	{
		m_lock.unlock();
	}
}