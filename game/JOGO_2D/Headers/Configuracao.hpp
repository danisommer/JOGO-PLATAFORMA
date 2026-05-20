#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <string>

namespace Gerenciadores
{
	// Singleton de configuracoes persistentes do jogo. Carrega/salva o
	// arquivo Saves/config.txt e expoe acesso ao mapeamento de teclas
	// (4 acoes para cada jogador) e parametros graficos (resolucao,
	// tela cheia, fps maximo). Mantem a configuracao em memoria pelo
	// tempo de vida do processo.
	class Configuracao
	{
	public:
		enum Acao
		{
			ESQUERDA = 0,
			DIREITA = 1,
			PULAR = 2,
			ATACAR = 3,
			N_ACOES = 4
		};

	private:
		static Configuracao* pInstancia;
		Configuracao();

		unsigned int largura;
		unsigned int altura;
		bool telaCheia;
		unsigned int fpsMax;

		sf::Keyboard::Key teclasP1[N_ACOES];
		sf::Keyboard::Key teclasP2[N_ACOES];

		void aplicarPadrao();

	public:
		~Configuracao();
		static Configuracao* getInstancia();

		void carregar();
		void salvar();

		unsigned int getLargura() const;
		unsigned int getAltura() const;
		bool getTelaCheia() const;
		unsigned int getFpsMax() const;

		sf::Keyboard::Key getTecla(int jogador, Acao acao) const;
		void setTecla(int jogador, Acao acao, sf::Keyboard::Key k);

		void setResolucao(unsigned int l, unsigned int a);
		void setTelaCheia(bool tc);
		void setFpsMax(unsigned int f);

		// Conversao entre o enum Keyboard::Key e nomes legiveis para
		// gravar/exibir. Retorna "?" para teclas nao mapeadas.
		static std::string nomeTecla(sf::Keyboard::Key k);
		static sf::Keyboard::Key teclaPorNome(const std::string& nome);
	};
}
