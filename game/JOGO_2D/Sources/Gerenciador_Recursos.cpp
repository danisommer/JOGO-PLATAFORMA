#include "Gerenciador_Recursos.hpp"
#include <stdexcept>

namespace Gerenciadores
{
	Gerenciador_Recursos* Gerenciador_Recursos::pGerenciador = nullptr;

	Gerenciador_Recursos* Gerenciador_Recursos::getGerenciador()
	{
		if (pGerenciador == nullptr)
			pGerenciador = new Gerenciador_Recursos();

		return pGerenciador;
	}

	const sf::Texture& Gerenciador_Recursos::getTextura(const std::string& caminho)
	{
		auto it = texturas.find(caminho);
		if (it != texturas.end())
			return it->second;

		sf::Texture textura;
		if (!textura.loadFromFile(caminho))
			throw std::runtime_error("Falha ao carregar textura: " + caminho);

		return texturas.emplace(caminho, std::move(textura)).first->second;
	}

	const sf::Font& Gerenciador_Recursos::getFonte(const std::string& caminho)
	{
		auto it = fontes.find(caminho);
		if (it != fontes.end())
			return it->second;

		sf::Font fonte;
		if (!fonte.loadFromFile(caminho))
			throw std::runtime_error("Falha ao carregar fonte: " + caminho);

		return fontes.emplace(caminho, std::move(fonte)).first->second;
	}
}
