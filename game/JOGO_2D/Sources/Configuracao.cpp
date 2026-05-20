#include "Configuracao.hpp"

#include <fstream>
#include <sstream>
#include <unordered_map>

namespace Gerenciadores
{
	Configuracao* Configuracao::pInstancia = nullptr;

	Configuracao::Configuracao() :
		largura(1600),
		altura(900),
		telaCheia(false),
		fpsMax(60)
	{
		aplicarPadrao();
		carregar();
	}

	Configuracao::~Configuracao() = default;

	Configuracao* Configuracao::getInstancia()
	{
		if (!pInstancia)
			pInstancia = new Configuracao();
		return pInstancia;
	}

	void Configuracao::aplicarPadrao()
	{
		teclasP1[ESQUERDA] = sf::Keyboard::A;
		teclasP1[DIREITA] = sf::Keyboard::D;
		teclasP1[PULAR] = sf::Keyboard::W;
		teclasP1[ATACAR] = sf::Keyboard::E;

		teclasP2[ESQUERDA] = sf::Keyboard::Left;
		teclasP2[DIREITA] = sf::Keyboard::Right;
		teclasP2[PULAR] = sf::Keyboard::Up;
		teclasP2[ATACAR] = sf::Keyboard::M;
	}

	void Configuracao::carregar()
	{
		std::ifstream arquivo("Saves/config.txt");
		if (!arquivo.is_open())
			return;

		std::string linha;
		while (std::getline(arquivo, linha))
		{
			const std::size_t pos = linha.find('=');
			if (pos == std::string::npos)
				continue;

			const std::string chave = linha.substr(0, pos);
			const std::string valor = linha.substr(pos + 1);

			if (chave == "largura")
				largura = static_cast<unsigned int>(std::stoul(valor));
			else if (chave == "altura")
				altura = static_cast<unsigned int>(std::stoul(valor));
			else if (chave == "tela_cheia")
				telaCheia = (valor == "1");
			else if (chave == "fps_max")
				fpsMax = static_cast<unsigned int>(std::stoul(valor));
			else if (chave == "p1_esq") teclasP1[ESQUERDA] = teclaPorNome(valor);
			else if (chave == "p1_dir") teclasP1[DIREITA] = teclaPorNome(valor);
			else if (chave == "p1_pulo") teclasP1[PULAR] = teclaPorNome(valor);
			else if (chave == "p1_atk") teclasP1[ATACAR] = teclaPorNome(valor);
			else if (chave == "p2_esq") teclasP2[ESQUERDA] = teclaPorNome(valor);
			else if (chave == "p2_dir") teclasP2[DIREITA] = teclaPorNome(valor);
			else if (chave == "p2_pulo") teclasP2[PULAR] = teclaPorNome(valor);
			else if (chave == "p2_atk") teclasP2[ATACAR] = teclaPorNome(valor);
		}
	}

	void Configuracao::salvar()
	{
		std::ofstream arquivo("Saves/config.txt", std::ofstream::out | std::ofstream::trunc);
		if (!arquivo.is_open())
			return;

		arquivo << "largura=" << largura << "\n";
		arquivo << "altura=" << altura << "\n";
		arquivo << "tela_cheia=" << (telaCheia ? "1" : "0") << "\n";
		arquivo << "fps_max=" << fpsMax << "\n";

		arquivo << "p1_esq=" << nomeTecla(teclasP1[ESQUERDA]) << "\n";
		arquivo << "p1_dir=" << nomeTecla(teclasP1[DIREITA]) << "\n";
		arquivo << "p1_pulo=" << nomeTecla(teclasP1[PULAR]) << "\n";
		arquivo << "p1_atk=" << nomeTecla(teclasP1[ATACAR]) << "\n";

		arquivo << "p2_esq=" << nomeTecla(teclasP2[ESQUERDA]) << "\n";
		arquivo << "p2_dir=" << nomeTecla(teclasP2[DIREITA]) << "\n";
		arquivo << "p2_pulo=" << nomeTecla(teclasP2[PULAR]) << "\n";
		arquivo << "p2_atk=" << nomeTecla(teclasP2[ATACAR]) << "\n";
	}

	unsigned int Configuracao::getLargura() const { return largura; }
	unsigned int Configuracao::getAltura() const { return altura; }
	bool Configuracao::getTelaCheia() const { return telaCheia; }
	unsigned int Configuracao::getFpsMax() const { return fpsMax; }

	sf::Keyboard::Key Configuracao::getTecla(int jogador, Acao acao) const
	{
		return (jogador == 0) ? teclasP1[acao] : teclasP2[acao];
	}

	void Configuracao::setTecla(int jogador, Acao acao, sf::Keyboard::Key k)
	{
		if (jogador == 0)
			teclasP1[acao] = k;
		else
			teclasP2[acao] = k;
	}

	void Configuracao::setResolucao(unsigned int l, unsigned int a)
	{
		largura = l;
		altura = a;
	}

	void Configuracao::setTelaCheia(bool tc) { telaCheia = tc; }
	void Configuracao::setFpsMax(unsigned int f) { fpsMax = f; }

	namespace
	{
		// Tabelas de conversao entre Keyboard::Key e nomes legiveis.
		// Apenas as teclas usaveis para os controles aparecem aqui;
		// teclas nao mapeadas viram "?".
		struct EntradaTecla
		{
			sf::Keyboard::Key codigo;
			const char* nome;
		};

		const EntradaTecla TABELA_TECLAS[] = {
			{sf::Keyboard::A, "A"}, {sf::Keyboard::B, "B"}, {sf::Keyboard::C, "C"},
			{sf::Keyboard::D, "D"}, {sf::Keyboard::E, "E"}, {sf::Keyboard::F, "F"},
			{sf::Keyboard::G, "G"}, {sf::Keyboard::H, "H"}, {sf::Keyboard::I, "I"},
			{sf::Keyboard::J, "J"}, {sf::Keyboard::K, "K"}, {sf::Keyboard::L, "L"},
			{sf::Keyboard::M, "M"}, {sf::Keyboard::N, "N"}, {sf::Keyboard::O, "O"},
			{sf::Keyboard::P, "P"}, {sf::Keyboard::Q, "Q"}, {sf::Keyboard::R, "R"},
			{sf::Keyboard::S, "S"}, {sf::Keyboard::T, "T"}, {sf::Keyboard::U, "U"},
			{sf::Keyboard::V, "V"}, {sf::Keyboard::W, "W"}, {sf::Keyboard::X, "X"},
			{sf::Keyboard::Y, "Y"}, {sf::Keyboard::Z, "Z"},
			{sf::Keyboard::Num0, "0"}, {sf::Keyboard::Num1, "1"},
			{sf::Keyboard::Num2, "2"}, {sf::Keyboard::Num3, "3"},
			{sf::Keyboard::Num4, "4"}, {sf::Keyboard::Num5, "5"},
			{sf::Keyboard::Num6, "6"}, {sf::Keyboard::Num7, "7"},
			{sf::Keyboard::Num8, "8"}, {sf::Keyboard::Num9, "9"},
			{sf::Keyboard::Left, "Left"}, {sf::Keyboard::Right, "Right"},
			{sf::Keyboard::Up, "Up"}, {sf::Keyboard::Down, "Down"},
			{sf::Keyboard::Space, "Space"}, {sf::Keyboard::Enter, "Enter"},
			{sf::Keyboard::LShift, "LShift"}, {sf::Keyboard::RShift, "RShift"},
			{sf::Keyboard::LControl, "LCtrl"}, {sf::Keyboard::RControl, "RCtrl"},
			{sf::Keyboard::Tab, "Tab"}, {sf::Keyboard::BackSpace, "Backspace"},
		};
	}

	std::string Configuracao::nomeTecla(sf::Keyboard::Key k)
	{
		for (const EntradaTecla& e : TABELA_TECLAS)
		{
			if (e.codigo == k)
				return e.nome;
		}
		return "?";
	}

	sf::Keyboard::Key Configuracao::teclaPorNome(const std::string& nome)
	{
		for (const EntradaTecla& e : TABELA_TECLAS)
		{
			if (nome == e.nome)
				return e.codigo;
		}
		return sf::Keyboard::Unknown;
	}
}
