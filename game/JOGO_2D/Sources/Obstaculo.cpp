#include "Obstaculo.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		Obstaculo::Obstaculo(const Vector2f pos, const Vector2f tam) :
			Entidade(pos, tam),
			danoso(false),
			colidir(true)
		{

		}

		Obstaculo::~Obstaculo()
		{
		}

		bool Obstaculo::getColidir()
		{
			return colidir;
		}

		bool Obstaculo::getDanoso()
		{
			return danoso;
		}

		void Obstaculo::desenharSprite()
		{
			gerenciador_grafico->desenhaSprite(sprite);
		}

	}
}

