/*
 * Модуль с определением методов класса Basket.
 */

#include "basket.h"


/**
 * Конструктор по умолчанию.
 */
Basket::Basket() {}

/**
 * Метод добавляет продукт в корзину.
 * @param p: добавляемый продукт.
 */
void Basket::add(Accessory &p)
{
    // Добавляем в корзину продукт, при этом удаляем из
    // корзины продукты того же типа, что и добавляемый
    for (int i = 0; i < products.size(); i++)
    {
        if (products[i].type == p.type)
            products.removeAt(i);
    }
    products.append(p);
}

/**
 * Метод добавляет продукт во временный список.
 * @param p: добавляемый продукт.
 */
void Basket::add_to_temp(Accessory &p)
{
    for (int i = 0; i < temp.size(); i++)
    {
        if (p.type == temp[i].type)
        {
            temp.removeAt(i);
            break;
        }
    }
    temp.append(p);
}

/**
 * Метод возвращает продукт из корзины.
 * @param i: индекс продукта.
 * @return: объект-комплект, находящийся под индексом i,
 * или пустой объект.
 */
Accessory Basket::at(int i) const
{
    if (0 <= i && i < products.size())
        return products.at(i);
    return Accessory();
}

/**
 * Метод очищает корзину.
 */
void Basket::clear()
{
    products.clear();
    temp.clear();
}

/**
 * Метод возвращает стоимость корзины.
 * @return: стоимость корзины.
 */
QVariant Basket::get_price() const
{
    int price = 0;
    for (int i = 0; i < products.size(); i++)
        price += products[i].get("price").toInt();
    return price;
}

/**
 * Метод возвращает количество продуктов в корзине.
 * @return: количество элементов.
 */
int Basket::get_size() const
{
    return products.size();
}

/**
 * Метод перемещает продукты из временного списка в корзину.
 */
void Basket::move()
{
    // Перемещаем продукты из временной корзины в корзину.
    // При этом удаляем из корзины все продукты того же типа,
    // что и у добавляемого продукта
    for (int i = 0; i < temp.size(); i++)
        add(temp[i]);
    temp.clear();
}

/**
 * Метод удаляет продукт из временного списка.
 * @param type: тип компонента, который нужно удалить.
 */
void Basket::remove_from_temp(const QString &type)
{
    for (int i = 0; i < temp.size(); i++)
    {
        if (temp[i].type == type)
        {
            temp.removeAt(i);
            return;
        }
    }
}
