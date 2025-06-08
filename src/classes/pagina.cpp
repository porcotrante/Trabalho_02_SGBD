#include <fstream>
#include <string>
#include "../headers/pagina.h"
#include <iostream>

void pagina::makePage(int colNum, std::string tableName, std::string pageNumber) {
    // Constrói o nome do arquivo: tableName + pageNumber + "page.txt"
    std::string fileName = tableName + pageNumber + "page.txt";

    // Cria o arquivo
    std::ofstream file("./pages/" + fileName);
    if (file.is_open()) {
        file << colNum << std::endl;  //escreve o número de colunas na primeira linha
        file.close();
    } else {
        // Pode adicionar um tratamento de erro, se desejar
        std::cerr << "Erro ao criar o arquivo: " << fileName << std::endl;
    }
}

void pagina::loadPage(std::string filePath){
    this->pageName = filePath;
    std::ifstream file("./pages/" + filePath);
    
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filePath << std::endl;
    }

    std::string linha;
    if (std::getline(file, linha)) {
        this->colNum = std::stoi(linha); // pega o número de colunas
    }
}

void pagina::writeLine(std::string line){
    std::ofstream file("./pages/" + this->pageName, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << pageName << std::endl;
    }

    file << line << std::endl;
}

std::string pagina::getLine(int number) {
    std::ifstream file("./pages/" + this->pageName);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << pageName << std::endl;
        return nullptr;
    }

    std::string linha;
    int linhaAtual = -1; // começa em -1 para ignorar o cabeçalho (linha 0)

    while (std::getline(file, linha)) {
        linhaAtual++;
        if (linhaAtual == number) {
            return linha;
        }
    }

    // Linha não encontrada
    std::cerr << "Linha " << number << " não encontrada no arquivo." << std::endl;
    return "";
}

pagina::pagina(){};
pagina::~pagina(){};