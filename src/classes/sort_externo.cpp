#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>
#include "../headers/pagina.h"

// Estrutura auxiliar
struct Tupla {
    std::vector<std::string> cols;
    std::string to_string() const {
        std::ostringstream oss;
        for (size_t i = 0; i < cols.size(); ++i) {
            if (i > 0) oss << ",";
            oss << cols[i];
        }
        return oss.str();
    }
};

std::vector<Tupla> carregarTuplas(std::string caminho_csv) {
    std::ifstream in(caminho_csv);
    std::string linha;
    std::vector<Tupla> tuplas;

    while (getline(in, linha)) {
        Tupla t;
        std::istringstream iss(linha);
        std::string campo;
        while (getline(iss, campo, ',')) {
            t.cols.push_back(campo);
        }
        tuplas.push_back(t);
    }
    return tuplas;
}

void external_sort(std::string tabela_nome, std::string caminho_csv, int col_idx) {
    auto tuplas = carregarTuplas(caminho_csv);

    std::sort(tuplas.begin(), tuplas.end(), [col_idx](const Tupla& a, const Tupla& b) {
        return a.cols[col_idx] < b.cols[col_idx];
    });

    int pagina_id = 0;
    int colunas = tuplas[0].cols.size();
    pagina::makePage(colunas, tabela_nome, std::to_string(pagina_id));
    pagina pag;
    pag.loadPage(tabela_nome + std::to_string(pagina_id) + "page.txt");

    int contador = 0;
    for (const Tupla& t : tuplas) {
        if (contador == 10) {
            pagina_id++;
            pagina::makePage(colunas, tabela_nome, std::to_string(pagina_id));
            pag.loadPage(tabela_nome + std::to_string(pagina_id) + "page.txt");
            contador = 0;
        }
        pag.writeLine(t.to_string());
        contador++;
    }

    std::cout << "Tabela " << tabela_nome << " ordenada e gravada em " << (pagina_id + 1) << " páginas." << std::endl;
}
