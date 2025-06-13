#ifndef SORT_MERGE_JOIN_H
#define SORT_MERGE_JOIN_H

#include <string>

void sort_merge_join(std::string tabelaA, int colA, int numPagA,
                     std::string tabelaB, int colB, int numPagB,
                     std::string saida_nome);

#endif