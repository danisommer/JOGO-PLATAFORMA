#include "ListaEntidade.hpp"
#include "Gerenciador_Grafico.hpp"

Lista::ListaEntidade::ListaEntidade()
{

}
Lista::ListaEntidade::~ListaEntidade()
{

}
void Lista::ListaEntidade::addEntidade(Entidades::Entidade* entidade)
{
	if (entidade) {
		LEs.addElemento(entidade);
	}
	
}
void Lista::ListaEntidade::removerEntidade(Entidades::Entidade* entidade)
{
	if (entidade)
	{
		LEs.removerElemento(entidade);
	}
}
void Lista::ListaEntidade::removerEntidade(int pos)
{

}
const int Lista::ListaEntidade::getTam() const
{
	return LEs.getTam();
}
Entidades::Entidade* Lista::ListaEntidade::operator[](int pos)
{
	return LEs.operator[](pos);
}

void Lista::ListaEntidade::limpar()
{
	LEs.limpar();
}
