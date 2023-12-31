#pragma once

#include <SFML/Graphics.hpp>
#include "Ente.hpp"
#include <fstream>


using namespace sf;

namespace Entidades
{
	class Entidade : public Ente
	{
	protected:
		Sprite sprite;
		RectangleShape corpo;

	public:
		Entidade(const Vector2f pos, const Vector2f tam);
		virtual ~Entidade();
		RectangleShape* getCorpo();
		virtual void atualizar() = 0;
		Vector2f getPos();
		virtual void desenharSprite() = 0;
		virtual void salvar(int save) = 0;
		virtual void limparArquivo(int save) = 0;
	};
}



