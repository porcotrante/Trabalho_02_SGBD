#include "../headers/tabela.h"
#include <fstream>
#include <iostream>
#include <sstream>

tabela::tabela(std::string name, std::string filePath){
    this->name = name;
    this->pageNum = 1;
    std::ifstream file(filePath); 

    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo.\n";
    }

    std::string firstLine;
    int cols = 0;
    //contando o número de colunas
    if (std::getline(file, firstLine)) {
        std::stringstream ss(firstLine);
        std::string token;

        while (std::getline(ss, token, ',')) {
            cols++;
        }
    }

    std::string line;
    int count = 0; //número de tuplas lidas
    pagina p = pagina();
    while (std::getline(file, line)) //lendo as linhas do arquivo
    {
        if (count%10 == 0)
        {
            pagina::makePage(cols, name, std::to_string(pageNum));
            p.loadPage(name + std::to_string(pageNum) + "page.txt");
            this->pageNum++;
        }

        std::stringstream ss(line);
        std::string token;
        std::string result;

        bool first = true;

        while (std::getline(ss, token, ',')) {
            if (!first) result += ";";
            result += token;
            first = false;
        }

        // Passa a string formatada para a funcao
        p.writeLine(result);
        count++;
    }
}

tabela::~tabela(){};