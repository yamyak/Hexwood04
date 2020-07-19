#include "Object.h"


/// <summary>
/// Constructor
/// </summary>
/// <returns></returns>
Object::Object()
{
	m_id = 0;
}

/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
Object::~Object()
{

}

/// <summary>
/// Get object id
/// </summary>
/// <returns>Object id</returns>
int Object::GetId()
{
	return m_id;
}

/// <summary>
/// Sets object id
/// </summary>
/// <param name="id">Integer id</param>
void Object::SetId(int id)
{
	m_id = id;
}