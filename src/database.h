/*
 * Заголовочный файл содержит объявление класса Database для
 * работы с базой данных.
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <QList>
#include <QSqlDatabase>
#include "accessory.h"


/**
 * Класс для работы с базой данных.
 */
class Database
{
public:
    // Конструктор по умолчанию
    Database();
    // Деструктор
    ~Database();
    // Метод добавляет позицию в таблицу
    int add(const QString&, const Accessory&) const;
    // Метод возвращает рамы для стиля катания
    void get_chassis_for_style(const QString&,
                               QList<Accessory>&) const;
    // Метод возвращает данные о компоненте по запросу к базе данных
    void get_data(const QString&, const QList<QString>&,
                  const QList<QString>&, const QString&,
                  QList<Accessory>&) const;
    // Метод возвращает передние амортизаторы для рамы
    void get_front_absorbers(Accessory*, QList<Accessory>&) const;
    // Метод возвращает передние роторы для рамы
    void get_front_rotors(Accessory*, QList<Accessory>&) const;
    // Метод возвращает задние амортизаторы для рамы
    void get_rear_absorbers(Accessory*, QList<Accessory>&) const;
    // Метод возвращает задние роторы для рамы
    void get_rear_rotors(Accessory*, QList<Accessory>&) const;
    // Метод возвращает стили катания
    void get_riding_styles(QList<QString>&) const;
    // Метод возвращает количество элементов в таблице
    int get_size(const QString&) const;
    // Метод возвращает все данные из таблицы
    void get_tbl_data(const QString&, QList<Accessory>&) const;
    // Метод возвращает колеса для рамы
    void get_wheels(Accessory*, QList<Accessory>&) const;
    // Метод проверяет, открыто ли соединение с базой данных
    bool is_open() const;
    // Метод удаляет позицию из таблицы
    bool remove(const QString&, const QList<int>&) const;

private:
    const QString DB_NAME = "data/bike.db"; // путь к базе данных
    QSqlDatabase db; // соединение с базой данных
};

#endif // DATABASE_H
