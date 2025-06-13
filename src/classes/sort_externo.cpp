#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>
#include <queue>
#include "../headers/pagina.h"
#include "../headers/tupla.h"
#include <iostream>

struct RunTupla {
    Tupla tupla;
    int run_idx;
    bool operator>(const RunTupla& other) const {
        // Comparação para min-heap
        return tupla.cols[0] > other.tupla.cols[0];
    }
};

std::vector<Tupla> carregarTuplas(std::istream& in) {
    std::string linha;
    std::vector<Tupla> tuplas;
    while (getline(in, linha)) {
        Tupla t;
        std::istringstream iss(linha);
        std::string campo;
        while (getline(iss, campo, ',')) {
            t.cols.push_back(campo);
        }
        if (!t.cols.empty())
            tuplas.push_back(t);
    }
    return tuplas;
}

void external_sort(std::string tabela_nome, std::string caminho_csv, int col_idx) {
    std::ifstream in(caminho_csv);
    std::string linha;
    std::vector<Tupla> buffer;
    int run_id = 0;
    const int TUPlAS_POR_RUN = 40; // 4 páginas * 10 tuplas

    // Fase 1: Criação dos runs
    while (getline(in, linha)) {
        Tupla t;
        std::istringstream iss(linha);
        std::string campo;
        while (getline(iss, campo, ',')) {
            t.cols.push_back(campo);
        }
        if (!t.cols.empty())
            buffer.push_back(t);

        if (buffer.size() == TUPlAS_POR_RUN) {
            std::sort(buffer.begin(), buffer.end(), [col_idx](const Tupla& a, const Tupla& b) {
                return a.cols[col_idx] < b.cols[col_idx];
            });
            std::ofstream runfile("./pages/" + tabela_nome + "_run" + std::to_string(run_id) + ".txt");
            for (const Tupla& tupla : buffer) {
                for (size_t i = 0; i < tupla.cols.size(); ++i) {
                    if (i > 0) runfile << ",";
                    runfile << tupla.cols[i];
                }
                runfile << "\n";
            }
            runfile.close();
            buffer.clear();
            run_id++;
        }
    }
    // Salva o último run se sobrou tupla
    if (!buffer.empty()) {
        std::sort(buffer.begin(), buffer.end(), [col_idx](const Tupla& a, const Tupla& b) {
            return a.cols[col_idx] < b.cols[col_idx];
        });
        std::ofstream runfile("./pages/" + tabela_nome + "_run" + std::to_string(run_id) + ".txt");
        for (const Tupla& tupla : buffer) {
            for (size_t i = 0; i < tupla.cols.size(); ++i) {
                if (i > 0) runfile << ",";
                runfile << tupla.cols[i];
            }
            runfile << "\n";
        }
        runfile.close();
        run_id++;
    }

    // Fase 2: Merge dos runs
    int num_runs = run_id;
    if (num_runs == 1) {
        std::cout << "Arquivo já está ordenado em um único run.\n";
        return;
    }

    // Abrir até 4 runs por vez e fazer merge
    int merge_pass = 0;
    while (num_runs > 1) {
        int new_run_id = 0;
        for (int i = 0; i < num_runs; i += 4) {
            int runs_to_merge = std::min(4, num_runs - i);
            std::vector<std::ifstream> run_files;
            std::vector<std::string> linhas;
            std::vector<bool> finished(runs_to_merge, false);
            std::vector<std::string> run_names;

            // Abre os arquivos dos runs
            for (int j = 0; j < runs_to_merge; ++j) {
                std::string run_name = "./pages/" + tabela_nome + "_run" + std::to_string(i + j) + ".txt";
                run_files.emplace_back(run_name);
                run_names.push_back(run_name);
                std::string l;
                if (getline(run_files[j], l)) {
                    linhas.push_back(l);
                } else {
                    linhas.push_back("");
                    finished[j] = true;
                }
            }

            std::string out_run_name = "./pages/" + tabela_nome + "_run_tmp" + std::to_string(new_run_id) + ".txt";
            std::ofstream out_run(out_run_name);

            while (true) {
                // Encontra o menor entre as linhas atuais
                int menor_idx = -1;
                Tupla menor_tupla;
                for (int j = 0; j < runs_to_merge; ++j) {
                    if (!finished[j]) {
                        Tupla t;
                        std::istringstream iss(linhas[j]);
                        std::string campo;
                        while (getline(iss, campo, ',')) t.cols.push_back(campo);
                        if (menor_idx == -1 || t.cols[col_idx] < menor_tupla.cols[col_idx]) {
                            menor_idx = j;
                            menor_tupla = t;
                        }
                    }
                }
                if (menor_idx == -1) break; // Todos acabaram

                // Escreve a menor tupla no novo run
                for (size_t k = 0; k < menor_tupla.cols.size(); ++k) {
                    if (k > 0) out_run << ",";
                    out_run << menor_tupla.cols[k];
                }
                out_run << "\n";

                // Avança no run de onde saiu a menor tupla
                if (getline(run_files[menor_idx], linhas[menor_idx])) {
                    // ok
                } else {
                    finished[menor_idx] = true;
                }
            }
            out_run.close();

            // FECHA todos os arquivos antes de remover!
            for (auto& f : run_files) {
                f.close();
            }
            // Após o merge, delete imediatamente os arquivos de input usados neste merge
            for (const auto& name : run_names) {
                std::filesystem::remove(name);
            }

            new_run_id++;
        }

        // Renomeia os novos runs temporários para runs normais
        for (int i = 0; i < new_run_id; ++i) {
            std::filesystem::rename(
                "./pages/" + tabela_nome + "_run_tmp" + std::to_string(i) + ".txt",
                "./pages/" + tabela_nome + "_run" + std::to_string(i) + ".txt"
            );
        }
        num_runs = new_run_id;
        merge_pass++;
    }

    // O arquivo final ordenado está em ./pages/tabela_nome_run0.txt
    std::cout << "Arquivo ordenado final: ./pages/" << tabela_nome << "_run0.txt" << std::endl;
}

// Gera páginas ordenadas a partir do arquivo run0.txt
void gerar_paginas_ordenadas(std::string tabela_nome, int colunas) {
    std::ifstream in("./pages/" + tabela_nome + "_run0.txt");
    if (!in.is_open()) {
        std::cerr << "Erro ao abrir ./pages/" << tabela_nome << "_run0.txt" << std::endl;
        return;
    }
    std::string linha;
    int pageNum = 0;
    int count = 0;
    pagina::makePage(colunas, tabela_nome, std::to_string(pageNum));
    pagina p;
    p.loadPage(tabela_nome + std::to_string(pageNum) + "page.txt");
    while (getline(in, linha)) {
        // Troca separador de , para ;
        std::replace(linha.begin(), linha.end(), ',', ';');
        p.writeLine(linha);
        count++;
        if (count % 10 == 0) {
            pageNum++;
            pagina::makePage(colunas, tabela_nome, std::to_string(pageNum));
            p.loadPage(tabela_nome + std::to_string(pageNum) + "page.txt");
        }
    }
    in.close();
}
