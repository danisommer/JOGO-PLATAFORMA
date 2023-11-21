#include "../Headers/Obstaculo.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		Personagens::Jogador* Obstaculo::jogador1 = nullptr;
		Personagens::Jogador* Obstaculo::jogador2 = nullptr;

		Obstaculo::Obstaculo(const Vector2f pos, const Vector2f tam) :
			Entidade(pos, tam),
			danoso(false),
			colidir(true)
		{

		}

		Obstaculo::~Obstaculo()
		{
		}

		void Obstaculo::setJogador(Personagens::Jogador* j)
		{
			jogador1 = j;
		}		
		
		void Obstaculo::setJogador2(Personagens::Jogador* j)
		{
			jogador2 = j;
		}

		bool Obstaculo::getColidir()
		{
			return colidir;
		}

		bool Obstaculo::getDanoso()
		{
			return danoso;
		}

	}
}

