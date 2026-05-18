#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

namespace Gerenciadores
{
	// Cache central de texturas e fontes. Cada arquivo e lido do disco
	// uma unica vez; chamadas seguintes devolvem uma referencia estavel
	// ao recurso ja carregado. Isso elimina a releitura de disco a cada
	// spawn de entidade e a duplicacao de texturas iguais na memoria.
	class Gerenciador_Recursos
	{
	private:
		static Gerenciador_Recursos* pGerenciador;
		Gerenciador_Recursos() = default;

		// unordered_map garante estabilidade de referencia dos nos:
		// ponteiros para os recursos permanecem validos apos insercoes.
		std::unordered_map<std::string, sf::Texture> texturas;
		std::unordered_map<std::string, sf::Font> fontes;

	public:
		static Gerenciador_Recursos* getGerenciador();

		// Lanca std::runtime_error se o arquivo nao puder ser carregado.
		const sf::Texture& getTextura(const std::string& caminho);
		const sf::Font& getFonte(const std::string& caminho);
	};
}
