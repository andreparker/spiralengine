#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <boost/shared_ptr.hpp>

#include "../Core/Rect.hpp"
#include "../Core/Sp_DataTypes.hpp"
#include "../Math/Math.hpp"

namespace Spiral
{
	class Texture;
	class VertexBuffer;

	class Sprite
	{
	public:
		/*!
		   @function  Sprite
		   @brief     
		   @return    
		   @param     boost::shared_ptr< Texture > & texture
		   @param     const Rect< SpReal > & spriteTexCoords - texture coords to allow animation
		   @param     const Rect< SpReal > & spriteInfo - (left,top - offsets) (right, bottom - width,height)
		*/
		Sprite( boost::shared_ptr< Texture >& texture, const Rect< SpReal >& spriteTexCoords, const Rect< SpReal >& spriteInfo );

		void SetTexCoords( const Rect< SpReal >& rect )
		{
			m_spriteTexCoords = rect;
		}

		void GetTexCoords( Rect< SpReal >& rect )
		{
			rect = m_spriteTexCoords;
		}

		void SetWidth( SpReal width )
		{
			m_width = width;
		}

		void SetHeight( SpReal height )
		{
			m_height = height;
		}

		SpReal GetWidth()const
		{
			return m_width;
		}

		SpReal GetHeight()const
		{
			m_height;
		}

		void SetAngle( SpReal angle )
		{
			m_angle = angle;
		}

		SpReal GetAngle()const
		{
			return m_angle;
		}

		void SetPosition( SpReal x, SpReal y )
		{
			m_position[0] = x;
			m_position[1] = y;
		}

		const Math::SpVector2r& GetPosition()const
		{
			return m_position;
		}

		void SetTexture( boost::shared_ptr< Texture >& texture )
		{
			m_texture = texture;
		}

		const boost::shared_ptr< VertexBuffer > GetVertexBuffer()const
		{
			return m_buffer;
		}

		const boost::shared_ptr< Texture > GetTexture()const
		{
			return m_texture;
		}

		void SetVertexBuffer( boost::shared_ptr< VertexBuffer >& buffer )
		{
			m_buffer = buffer;
		}

		void SetAlphaBlend( bool enable )
		{
			m_alphaBlend = enable;
		}

		/*!
		   @function  SetAdditiveBlend
		   @brief     sets additive, this overrides alpha blending
		   @return    void
		   @param     bool enable
		*/
		void SetAdditiveBlend( bool enable )
		{
			m_additiveBlend = enable;
		}

		bool IsAdditive()const
		{
			return m_additiveBlend;
		}

		bool hasAlpha()const
		{
			return m_alphaBlend;
		}

		SpReal GetRotOffsetX()const
		{
			return m_rotOffsetX;
		}

		SpReal GetRotOffsetY()const
		{
			return m_rotOffsetY;
		}
		/*
			new operator used for Math data types
		*/
		MAKE_ALIGNED_NEW
	private:
		boost::shared_ptr< Texture > m_texture;
		boost::shared_ptr< VertexBuffer > m_buffer;
		Rect< SpReal > m_spriteTexCoords;

		Math::SpVector2r m_position;
		SpReal m_angle,m_width,m_height;
		bool m_alphaBlend;
		bool m_additiveBlend;
		SpReal m_rotOffsetX,m_rotOffsetY;		// offset to be the rotation pivot point

	};
}
#endif