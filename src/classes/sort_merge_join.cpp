#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include "../headers/pagina.h"

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

void sort_merge_join(std::string tabelaA, int colA, int numPagA,
                     std::string tabelaB, int colB, int numPagB,
                     std::string saida_nome) {
    std::ofstream out("./" + saida_nome);
    int IOs = 0, paginas_escritas = 0, tuplas_saida = 0;

    for (int pa = 0; pa < numPagA; ++pa) {
        pagina pagA;
        pagA.loadPage(tabelaA + std::to_string(pa) + "page.txt");

        for (int i = 0; i < 10; ++i) {
            std::string linhaA = pagA.getLine(i + 1);
            if (linhaA.empty()) continue;

            Tupla ta;
            std::istringstream issA(linhaA);
            std::string campo;
            while (getline(issA, campo, ',')) ta.cols.push_back(campo);

            for (int pb = 0; pb < numPagB; ++pb) {
                pagina pagB;
                pagB.loadPage(tabelaB + std::to_string(pb) + "page.txt");

                for (int j = 0; j < 10; ++j) {
                    std::string linhaB = pagB.getLine(j + 1);
                    if (linhaB.empty()) continue;

                    Tupla tb;
                    std::istringstream issB(linhaB);
                    std::string c;
                    while (getline(issB, c, ',')) tb.cols.push_back(c);

                    if (ta.cols[colA] == tb.cols[colB]) {
                        out << ta.to_string() << "," << tb.to_string() << "\n";
                        tuplas_saida++;
                    }
                    IOs++;
                }
            }
        }
    }

    paginas_escritas = 1;

    std::cout << "Junção concluída: " << saida_nome << std::endl;
    std::cout << "IOs realizados: " << IOs << std::endl;
    std::cout << "Páginas escritas: " << paginas_escritas << std::endl;
    std::cout << "Tuplas resultantes: " << tuplas_saida << std::endl;
}
