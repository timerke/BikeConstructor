/*
 * Модуль содержит определения полезных функций.
 */

#include "const.h"
#include "functions.h"


/**
 * Функция находит индекс таблицы по ее имени на английском.
 * @param tbl_name: имя таблицы.
 * @return: индекс таблицы.
 */
int find_tbl_index(const QString &tbl_name)
{
    for (int i = 0; i < TBLS_ENG.size(); i++)
    {
        if (TBLS_ENG[i] == tbl_name)
            return i;
    }
    return -1;
}
