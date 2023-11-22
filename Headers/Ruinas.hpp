#pragma once

#include "Fase.hpp"

namespace Fases
{
	class Ruinas : public Fase
	{
	private:
		bool concluida2;
		
	public:
		Ruinas():Fase(), concluida2(false)
		{
			fase = 2;

			if (!texturaFundo.loadFromFile("Assets/Cenario/background_2.png")) {
				exit(1);
			}
		}
		~Ruinas()
		{

		}

		void criarMapa()
		{
			std::ifstream arquivo;
			std::string linha;

			arquivo.open("Fases/fase2-1p.txt");
			if (!arquivo.is_open()) {
				std::cout << "O arquivo da fase nao foi aberto" << std::endl;
				exit(1);
			}

			int j = 0;
			while (std::getline(arquivo, linha)) {
				for (int i = 0; i < linha.size(); i++) {
					if (linha[i] != ' ') {
						instanciaEntidades(linha[i], Vector2f(i, j));
					}
				}
				j++;
			}
			arquivo.close();
			
			if (numJogadores == 1)
			{
				Inimigo::setJogador(jogador);
				Obstaculo::setJogador(jogador);
			}
			else if (numJogadores == 2)
			{
				Inimigo::setJogador(jogador);
				Obstaculo::setJogador(jogador);

				Inimigo::setJogador2(jogador2);
				Obstaculo::setJogador2(jogador2);
			}
			else
			{
				cout << "numero incomum de jogadores" << endl;
				exit(1);
			}

		}

		void criaFundo()
		{
			sf::Sprite sprite(texturaFundo);
			sprite.setScale(0.9f, 0.9f);

		}

		void executaFase()
		{
				criarMapa();
				while (gerenciador_grafico->getOpen())
				{
					gerenciador_eventos->Executar();
					gerenciador_grafico->limpaTela();
					atualizaCamera();
					executaLista();
					AtualizarPersonagens();
					desenhaElementos();
					gerenciador_colisoes->Executar();
					gerenciador_grafico->mostraElemento();
					//std::cout << "Entrando" << std::endl;
				}
		}

	};
}