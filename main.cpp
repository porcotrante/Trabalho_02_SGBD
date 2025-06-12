#include <iostream>
#include "./src/headers/tabela.h"
#include "./src/headers/sort_externo.h"
#include "./src/headers/sort_merge_join.h"

int main() {
    tabela vinhos("vinhos", "./vinho.csv");
    tabela uvas("uvas", "./uva.csv");
    tabela pais("pais", "./pais.csv");
    external_sort("uva", "./uva.csv", 0);       // ordenar por uva_id
    external_sort("vinho", "./vinho.csv", 3);   // ordenar por uva_id                                            
    sort_merge_join("uva", 0, 1, "vinho", 3, 3, "resultado_join_uvaxvinho.csv");// Realiza junção uva.uva_id = vinho.uva_id
    sort_merge_join("pais", 0, 1, "vinho", 3, 3, "resultado_join_paisxvinho.csv"); 
}