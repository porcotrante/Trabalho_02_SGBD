#include <vector>
#include "pagina.h"
#ifndef TABELA_H
#define TABELA_H

class tabela
{
public:
    tabela(std::string name, std::string filePath);
    ~tabela();
    int pageNum;
    std::string name;
};

#endif