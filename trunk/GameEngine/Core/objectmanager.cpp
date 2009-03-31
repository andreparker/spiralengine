#include "objectmanager.hpp"
#include "common.hpp"

using namespace gameEngine;

objectManager::objectManager():m_referenceCount(0)
{
}

objectManager::~objectManager()
{
}

bool objectManager::initialize()
{
	typedef common::linkedList< common::Base_Info* >::iterator Itr;
	Itr itr = m_classList.head();

	for(; itr != m_classList.end(); ++itr )
	{
		(*itr)->SetupClassInfo();
	}

	return true;
}