#pragma once
#include "Gerenciador_Grafico.hpp"

class Ente
{
protected:
	int id;
	static Gerenciadores::Gerenciador_Grafico* gerenciador_grafico;
public:
	Ente();
	~Ente();
	int getId();
};

