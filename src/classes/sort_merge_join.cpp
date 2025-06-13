// sort_merge_join.cpp - Implementacao corrigida
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include "../headers/pagina.h"
#include "../headers/tupla.h"

// Funcao para juntar os campos das tuplas (usando virgula)
std::string join_tuplas(const Tupla& ta, const Tupla& tb) {
    std::ostringstream oss;
    for (size_t i = 0; i < ta.cols.size(); ++i) {
        if (i > 0) oss << ",";
        oss << ta.cols[i];
    }
    for (size_t i = 0; i < tb.cols.size(); ++i) {
        oss << "," << tb.cols[i];
    }
    return oss.str();
}

void sort_merge_join(std::string tabelaA, int colA, int numPagA,
                     std::string tabelaB, int colB, int numPagB,
                     std::string saida_nome) {
    std::ofstream out("./" + saida_nome);
    int IOs = 0, paginas_escritas = 0, tuplas_saida = 0;

    // Ponteiros para as paginas e linhas atuais
    int currentPageA = 0, currentPageB = 0;
    int currentLineA = 1, currentLineB = 1; // Linha 0 é cabecalho

    pagina pagA, pagB;
    pagA.loadPage(tabelaA + std::to_string(currentPageA) + "page.txt");
    pagB.loadPage(tabelaB + std::to_string(currentPageB) + "page.txt");
    IOs += 2;

    std::vector<std::string> buffer_saida;

    while (currentPageA < numPagA && currentPageB < numPagB) {
        std::string linhaA = pagA.getLine(currentLineA);
        std::string linhaB = pagB.getLine(currentLineB);

        if (linhaA.empty() || linhaB.empty()) break;

        Tupla ta, tb;
        std::istringstream issA(linhaA), issB(linhaB);
        std::string campo;

        // Atencao: separador das paginas é ';'
        while (getline(issA, campo, ';')) ta.cols.push_back(campo);
        while (getline(issB, campo, ';')) tb.cols.push_back(campo);

        if (ta.cols[colA] == tb.cols[colB]) {
            // Join encontrado
            buffer_saida.push_back(join_tuplas(ta, tb));
            tuplas_saida++;
            currentLineA++;
            currentLineB++;
        } else if (ta.cols[colA] < tb.cols[colB]) {
            currentLineA++;
        } else {
            currentLineB++;
        }

        // Troca de pagina se necessario
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

        // Quando buffer de saida enche (10 tuplas), grava uma pagina
        if (buffer_saida.size() == 10) {
            for (const auto& l : buffer_saida) out << l << "\n";
            buffer_saida.clear();
            paginas_escritas++;
        }
    }

    // Grava o que sobrou no buffer de saida
    if (!buffer_saida.empty()) {
        for (const auto& l : buffer_saida) out << l << "\n";
        paginas_escritas++;
    }

    std::cout << "Juncao concluida: " << saida_nome << std::endl;
    std::cout << "IOs realizados: " << IOs << std::endl;
    std::cout << "Paginas escritas: " << paginas_escritas << std::endl;
    std::cout << "Tuplas resultantes: " << tuplas_saida << std::endl;
}