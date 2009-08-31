/*!
*/
#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <boost/shared_ptr.hpp>

#include "Color.hpp"

namespace Spiral
{
	class Texture;
	class Material
	{
	public:
		Material();
		Material( const Rgba& ambient, const Rgba& diffuse, const Rgba& specular, const Rgba& emissive );

		const Rgba& GetAmbient()const;
		const Rgba& GetDiffuse()const;
		const Rgba& GetSpecular()const;
		const Rgba& GetEmissive()const;

		void SetAmbient( const Rgba& ambient );
		void SetDiffuse( const Rgba& diffuse );
		void SetSpecular( const Rgba& specular );
		void SetEmissive( const Rgba& emissive );

		void SetDiffuseTexture( boost::shared_ptr<Texture>& diffuse );
		void SetEffectTexture( boost::shared_ptr<Texture>& effect );

		boost::shared_ptr<Texture> GetDiffuseTexture()const;
		boost::shared_ptr<Texture> GetEffectTexture()const;

	private:
		Rgba m_ambient;
		Rgba m_diffuse;
		Rgba m_specular;
		Rgba m_emissive;
	private:
		boost::shared_ptr<Texture> m_diffuseTexture;
		boost::shared_ptr<Texture> m_effectTexture;	// e.g normal maps etc
	};

	inline Material::Material():
	m_ambient(),m_diffuse(),m_specular(),m_emissive(),m_diffuseTexture(){}

	inline Material::Material( const Rgba& ambient, const Rgba& diffuse, const Rgba& specular, const Rgba& emissive ):
	m_ambient( ambient ), m_diffuse( diffuse ), m_specular( specular ), m_emissive( emissive ), m_diffuseTexture(),
	m_effectTexture(){}

	inline const Rgba& Material::GetAmbient() const
	{
		return m_ambient;
	}

	inline const Rgba& Material::GetDiffuse() const
	{
		return m_diffuse;
	}

	inline const Rgba& Material::GetSpecular() const
	{
		return m_specular;
	}

	inline const Rgba& Material::GetEmissive() const
	{
		return m_emissive;
	}

	inline void Material::SetAmbient( const Rgba& ambient )
	{
		m_ambient = ambient;
	}

	inline void Material::SetDiffuse( const Rgba& diffuse )
	{
		m_diffuse = diffuse;
	}

	inline void Material::SetSpecular( const Rgba& specular )
	{
		m_specular = specular;
	}

	inline void Material::SetEmissive( const Rgba& emissive )
	{
		m_emissive = emissive;
	}

	inline void Material::SetDiffuseTexture( boost::shared_ptr<Texture>& diffuse )
	{
		m_diffuseTexture = diffuse;
	}

	inline void Material::SetEffectTexture( boost::shared_ptr<Texture>& effect )
	{
		m_effectTexture = effect;
	}

	inline boost::shared_ptr<Texture> Material::GetDiffuseTexture() const
	{
		return m_diffuseTexture;
	}

	inline boost::shared_ptr<Texture> Material::GetEffectTexture() const
	{
		return m_effectTexture;
	}
}
#endif