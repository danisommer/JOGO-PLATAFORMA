#pragma once
#include "Obstaculo.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		class Plataforma : public Obstaculo
		{
		private:
			const sf::Vector2f pos;
			int fase;
			bool grande;

		public:
			Plataforma(const Vector2f p, const Vector2f tam, bool g);
			~Plataforma();
			void atualizar();
			void danar(int jogador);
			void setFase(int f);
			void salvar(int save);
			void limparArquivo(int save);

		};
	}
}