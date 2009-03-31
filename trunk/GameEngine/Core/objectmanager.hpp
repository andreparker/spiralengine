#ifndef OBJECT_MANAGER_HPP
#define OBJECT_MANAGER_HPP

#include "coreobject.hpp"
#include "linkedlist.ipp"

namespace gameEngine {

	namespace common
	{
		struct Base_Info;
	}

	class objectManager : public common::coreObject
	{
		public:
			objectManager();
			~objectManager();

			static objectManager* instance() 
			{ 
				static objectManager* _instance = 0;

				if( !_instance ) _instance = new objectManager;

				_instance->reference();
				return _instance;
			}

			virtual bool initialize();

			virtual uint32 reference()
			{
				return ++m_referenceCount;
			}

			virtual uint32 unReference()
			{
				uint32 count;
				if( m_referenceCount )
				{
					count = --m_referenceCount;
				}
				else
				{
					delete this;
					count = 0;
				}

				return count;
			}

			virtual void destroy()
			{
				delete this;
			}

			virtual void addInfo( common::Base_Info* info )
			{ 
				m_classList.insertTail( info ); 
			}

		protected:
			uint32 m_referenceCount;
			common::linkedList< common::Base_Info* > m_classList;

	};
}
#endif