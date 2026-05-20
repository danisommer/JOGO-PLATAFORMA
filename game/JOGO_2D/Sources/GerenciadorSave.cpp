#include "GerenciadorSave.hpp"
#include <fstream>
#include <sstream>

namespace Persistencia
{
	namespace
	{
		std::string caminho(int slot)
		{
			return "Saves/save" + std::to_string(slot) + ".txt";
		}
	}

	bool GerenciadorSave::salvar(int slot, const DadosSave& dados)
	{
		if (slot < 1 || slot > 3)
			return false;

		std::ofstream arquivo(caminho(slot), std::ios::out | std::ios::trunc);
		if (!arquivo.is_open())
			return false;

		arquivo << "VERSAO " << VERSAO << "\n";
		arquivo << "FASE " << dados.fase << "\n";
		arquivo << "JOGADORES " << dados.numJogadores << "\n";

		for (const EstadoJogador& j : dados.jogadores)
			arquivo << "JOGADOR " << j.vida << " " << j.x << " " << j.y << "\n";

		arquivo << "PONTUACAO " << dados.pontuacao[0] << " " << dados.pontuacao[1] << "\n";
		arquivo << "KILLS " << dados.kills << "\n";
		arquivo << "PONTOS_ARVORE " << dados.pontosArvore << "\n";

		arquivo << "SKILLS";
		for (bool s : dados.skillsDesbloqueadas)
			arquivo << " " << (s ? 1 : 0);
		arquivo << "\n";

		return true;
	}

	bool GerenciadorSave::carregar(int slot, DadosSave& dados)
	{
		std::ifstream arquivo(caminho(slot));
		if (!arquivo.is_open())
			return false;

		dados = DadosSave{};
		bool versaoValida = false;

		std::string linha;
		while (std::getline(arquivo, linha))
		{
			std::istringstream iss(linha);
			std::string tag;
			iss >> tag;

			if (tag == "VERSAO")
			{
				int v = 0;
				iss >> v;
				versaoValida = (v >= 1 && v <= VERSAO);
			}
			else if (tag == "FASE")
			{
				iss >> dados.fase;
			}
			else if (tag == "JOGADORES")
			{
				iss >> dados.numJogadores;
			}
			else if (tag == "JOGADOR")
			{
				EstadoJogador j;
				iss >> j.vida >> j.x >> j.y;
				dados.jogadores.push_back(j);
			}
			else if (tag == "PONTUACAO")
			{
				iss >> dados.pontuacao[0] >> dados.pontuacao[1];
			}
			else if (tag == "KILLS")
			{
				iss >> dados.kills;
			}
			else if (tag == "PONTOS_ARVORE")
			{
				iss >> dados.pontosArvore;
			}
			else if (tag == "SKILLS")
			{
				int v;
				while (iss >> v)
					dados.skillsDesbloqueadas.push_back(v != 0);
			}
			// Tags desconhecidas sao ignoradas (compatibilidade futura).
		}

		return versaoValida;
	}

	bool GerenciadorSave::existe(int slot)
	{
		std::ifstream arquivo(caminho(slot));
		return arquivo.is_open();
	}
}
