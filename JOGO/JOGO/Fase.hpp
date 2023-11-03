/*
#pragma once
#include <vector>
#include "Entidade.hpp"

class Fase {
private:
    std::vector<Entidade*> entidades; 
    std::string nome; 
    

public:
    Fase(const std::string& nome) : nome(nome) {
       
    }

    void adicionarEntidade(Entidade* entidade) {
        entidades.push_back(entidade);
    }

    void removerEntidade(Entidade* entidade) {
    }

    void atualizar() {
        for (Entidade* entidade : entidades) {
            entidade->atualizar();
        }
    }

    void renderizar() {
        // Renderize as entidades da fase
        for (Entidade* entidade : entidades) {
            entidade->renderizar();
        }
    }
};
*/