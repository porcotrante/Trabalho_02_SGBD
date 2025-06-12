// sort_merge_join.cpp - Implementação corrigida
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include "../headers/pagina.h"
#include "../headers/tupla.h"

void sort_merge_join(std::string tabelaA, int colA, int numPagA,
                     std::string tabelaB, int colB, int numPagB,
                     std::string saida_nome) {
    std::ofstream out("./" + saida_nome);
    int IOs = 0, paginas_escritas = 0, tuplas_saida = 0;
    
    // Ponteiros para as páginas atuais
    int currentPageA = 0, currentPageB = 0;
    int currentLineA = 1, currentLineB = 1; // Linha 0 é cabeçalho
    
    // Carrega as primeiras páginas
    pagina pagA, pagB;
    pagA.loadPage(tabelaA + std::to_string(currentPageA) + "_page.txt");
    pagB.loadPage(tabelaB + std::to_string(currentPageB) + "_page.txt");
    IOs += 2;
    
    while (currentPageA < numPagA && currentPageB < numPagB) {
        std::string linhaA = pagA.getLine(currentLineA);
        std::string linhaB = pagB.getLine(currentLineB);
        
        if (linhaA.empty() || linhaB.empty()) break;
        
        Tupla ta, tb;
        std::istringstream issA(linhaA), issB(linhaB);
        std::string campo;
        
        while (getline(issA, campo, ',')) ta.cols.push_back(campo);
        while (getline(issB, campo, ',')) tb.cols.push_back(campo);
        
        if (ta.cols[colA] == tb.cols[colB]) {
            // Match encontrado - produzir saída
            out << ta.to_string() << "," << tb.to_string() << "\n";
            tuplas_saida++;
            
            // Avançar em ambas as tabelas
            currentLineA++;
            currentLineB++;
            
            // Verificar se precisa carregar próxima página
            if (currentLineA > 10) {
                currentPageA++;
                currentLineA = 1;
                if (currentPageA < numPagA) {
                    pagA.loadPage(tabelaA + std::to_string(currentPageA) + "page.txt");
                    IOs++;
                }
            }
            if (currentLineB > 10) {
                currentPageB++;
                currentLineB = 1;
                if (currentPageB < numPagB) {
                    pagB.loadPage(tabelaB + std::to_string(currentPageB) + "page.txt");
                    IOs++;
                }
            }
        } 
        else if (ta.cols[colA] < tb.cols[colB]) {
            // Avançar na tabela A
            currentLineA++;
            if (currentLineA > 10) {
                currentPageA++;
                currentLineA = 1;
                if (currentPageA < numPagA) {
                    pagA.loadPage(tabelaA + std::to_string(currentPageA) + "page.txt");
                    IOs++;
                }
            }
        }
        else {
            // Avançar na tabela B
            currentLineB++;
            if (currentLineB > 10) {
                currentPageB++;
                currentLineB = 1;
                if (currentPageB < numPagB) {
                    pagB.loadPage(tabelaB + std::to_string(currentPageB) + "page.txt");
                    IOs++;
                }
            }
        }
    }
    
    paginas_escritas = 1; // Arquivo de saída
    
    std::cout << "Junção concluída: " << saida_nome << std::endl;
    std::cout << "IOs realizados: " << IOs << std::endl;
    std::cout << "Páginas escritas: " << paginas_escritas << std::endl;
    std::cout << "Tuplas resultantes: " << tuplas_saida << std::endl;
}