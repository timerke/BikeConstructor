/*
 * Заголовочный файл с объявлением класса Basket для работы
 * с корзиной, в которую помещаются выбранные пользователем
 * компоненты.
 */

#ifndef BASKET_H
#define BASKET_H

#include <QList>
#include <QString>
#include <QVariant>
#include "accessory.h"


/**
 * Класс для работы с корзиной.
 */
class Basket
{
public:
    // Конструктор по умолчанию
    Basket();
    // Метод добавляет продукт в корзину
    void add(Accessory&);
    // Метод добавляет продукт во временный список
    void add_to_temp(Accessory&);
    // Метод возвращает продукт из корзины
    Accessory at(int) const;
    // Метод очищает корзину
    void clear();
    // Метод возвращает стоимость корзины
    QVariant get_price() const;
    // Метод возвращает количество продуктов в корзине
    int get_size() const;
    // Метод перемещает продукты из временного списка в корзину
    void move();
    // Метод удаляет продукт из временного списка
    void remove_from_temp(const QString&);

private:
    // Список продуктов в корзине
    QList<Accessory> products;
    // Временный список продуктов
    QList<Accessory> temp;
};

#endif // BASKET_H
