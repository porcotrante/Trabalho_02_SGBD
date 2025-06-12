#include <iostream>
#include "./src/headers/tabela.h"
#include "./src/headers/sort_externo.h"
#include "./src/headers/sort_merge_join.h"

int main() {
    tabela vinhos("vinhos", "./vinho.csv");
    tabela uvas("uvas", "./uva.csv");
    tabela pais("pais", "./pais.csv");
    external_sort("uva", "./uva.csv", 0);
    gerar_paginas_ordenadas("uva", 5);

    external_sort("vinho", "./vinho.csv", 3);
    gerar_paginas_ordenadas("vinho", 5);

    external_sort("pais", "./pais.csv", 0);
    gerar_paginas_ordenadas("pais", 2); // ajuste o número de colunas conforme sua tabela pais

    sort_merge_join("uva", 0, uvas.pageNum - 1, "vinho", 3, vinhos.pageNum - 1, "resultado_join_uvaxvinho.txt");
    sort_merge_join("vinho", 3, vinhos.pageNum - 1, "uva", 0, uvas.pageNum - 1, "resultado_join_vinhaxuva.txt");
    sort_merge_join("uva", 4, uvas.pageNum - 1, "pais", 0, pais.pageNum - 1, "resultado_join_uvaxpais.txt");
}