#pragma once
#include <iostream>

namespace Lista
{
	template <class TL>
	class Lista
	{
	private:
		//Classe aninhada
		template <class TE>
		class Elemento
		{
		private:
			Elemento<TE>* pProx;
			TE* pInfo;

		public:
			Elemento() :pProx(nullptr), pInfo(nullptr)
			{

			}
			~Elemento() 
			{
				if (pInfo)
					delete pInfo;
				pProx = nullptr;
			}
			TE* getInfo()
			{
				if (pInfo)
					return pInfo;
				else
					return nullptr;
			}
			Elemento<TE>* getProx()
			{
				if (pProx)
					return pProx;
				else 
					return nullptr;
			}
			void setInfo(TE* pI)
			{
				if (pI)
					pInfo = pI;
			}
			void setProx(Elemento<TE>* pP)
			{
				if (pP)
					pProx = pP;
			}
			//Fim da classe aninhada elemento
		};
		private:
			Elemento<TL>* pPrimeiro;
			Elemento<TL>* pUltimo;
			int tam;

		public:
			Lista() :pPrimeiro(nullptr), pUltimo(nullptr), tam(0)
			{

			}
			~Lista()
			{
				limpar();
			}
			void limpar()
			{
				Elemento<TL>* aux = nullptr;
				while (pPrimeiro)
				{
					aux = pPrimeiro;
					pPrimeiro = pPrimeiro->getProx();
					if (aux)
						delete aux;
				}
				tam = 0;
				pPrimeiro = nullptr;
			}
			void addElemento(TL* pE)
			{
				if (!pE)
					exit(1);

				Elemento<TL>* novoElemento = new Elemento<TL>();
				if (!novoElemento)
					exit(1);

				novoElemento->setInfo(pE);
				if (!pPrimeiro)
				{
					pPrimeiro = novoElemento;
					pUltimo = novoElemento;
				}
				else
				{
					pUltimo->setProx(novoElemento);
					pUltimo = novoElemento;
				}
				tam++;
			}
			void removerElemento(TL* pE)
			{
				if (!pE) {
					std::cout << "Elemento nulo" << std::endl;
					exit(1);
				}

				Elemento<TL>* aux = pPrimeiro;
				Elemento<TL>* aux2 = nullptr;

				while (aux != nullptr && aux->getInfo() != pE)
				{
					aux2 = aux;
					aux = aux->getProx();
				}
				if (aux->getInfo() == pE)
				{
					if (aux == pPrimeiro) {
						pPrimeiro = aux->getProx();
					}
					else if (aux == pUltimo) {
						pUltimo = aux;
					}
					else {
						aux2->setProx(aux->getProx());
					}
				}

				tam--;
			}
			const int getTam() const
			{
				return tam;
			}
			TL* operator[](int pos)
			{
				if (pos >= (int)tam || pos < 0) {
					std::cout << "ERROR::Lista pos eh maior que o tamanho da lista" << std::endl;
					exit(1);
				}
				Elemento<TL>* aux = pPrimeiro;
				for (int i = 0; i < pos; i++) {
					aux = aux->getProx();
				}
				return aux->getInfo();
			}
	};
}