/*!
*/
#ifndef BINDABLE_HPP
#define BINDABLE_HPP

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace Spiral
{

	class Bindable
	{
	public:

		virtual ~Bindable();

		/*!
		   @function  Bind
		   @brief     binda
		   @return    void
		*/
		void Bind();

		/*!
		   @function  UnBind
		   @brief     unbinds
		   @return    void
		*/
		void UnBind();

		bool IsBound()const
		{
			return m_isBound;
		}

	protected:
		Bindable();

	private:
		virtual void DoBind() = 0;
		virtual void DoUnBind() = 0;

	private:
		bool m_isBound;
	};

	class Bind : boost::noncopyable
	{
	public:
		explicit Bind( const boost::shared_ptr<Bindable>& bindable ):
		m_object( bindable )
		{
			if( m_object )
			{
				m_object->Bind();
			}
		}

		Bind::~Bind()
		{
			if( m_object )
			{
				m_object->UnBind();
			}
		}

	private:
		boost::shared_ptr<Bindable> m_object;
	};

	
	
}

#endif