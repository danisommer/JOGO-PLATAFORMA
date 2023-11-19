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
void Lista::ListaEntidade::executar(sf::RenderWindow* window)
{
	int tam = LEs.getTam();
	Entidades::Entidade* aux = nullptr;
	
	for (int i = 0; i < tam; i++)
	{
		aux = LEs.operator[](i);
		//aux->atualizar();
	
		//window->draw(aux->getSprite());
	}
}
void Lista::ListaEntidade::limpar()
{
	LEs.limpar();
}
void Lista::ListaEntidade::desenharEntidades(Gerenciadores::Gerenciador_Grafico* pGG)
{
	for (int i = 0; i < LEs.getTam(); i++)
	{
		if (LEs.operator[](i)) {
			//std::cout << "Desenhando" << std::endl;
			pGG->desenhaSprite(LEs.operator[](i)->getSprite());
			//pGG->desenhaHitbox(LEs.operator[](i)->getHitbox());
		}
	}
}