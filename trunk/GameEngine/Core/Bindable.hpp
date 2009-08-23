/*!
*/
#ifndef BINDABLE_HPP
#define BINDABLE_HPP

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
}

#endif