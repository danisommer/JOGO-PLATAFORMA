#pragma once

#include "Fase.hpp"

namespace Fases
{
	class Floresta : public Fase
	{
	private:
		bool concluida1;

	public:
		Floresta():Fase(), concluida1(false)
		{
			fase = 1;

			if (!texturaFundo.loadFromFile("Assets/Cenario/background_1.png")) {
				exit(1);
			}
		}
		~Floresta()
		{
		}
		void criarMapa()
		{
			std::ifstream arquivo;
			std::string linha;

			arquivo.open("Fases/fase1-1p.txt");
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
		}

	};
}