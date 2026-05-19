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
