/*
 * Заголовочный файл содержит объявление класса Database для работы
 * с базой данных.
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <QList>
#include <QSqlDatabase>
#include "accessory.h"


/**
 * Класс для работы с базо данных.
 */
class Database
{
public:
    // Конструктор по умолчанию
    Database();
    // Деструктор
    ~Database();
    // Метод добавляет позицию в таблицу
    int add(const QString&, const Accessory&);
    // Метод возвращает рамы для стиля катания
    void get_chassis_for_style(const QString&, QList<Accessory>&);
    // Метод возвращает данные о компоненте по запросу к базе данных
    void get_data(const QString&, const QList<QString>&,
                  const QList<QString>&, const QString&,
                  QList<Accessory>&);
    // Метод возвращает передние амортизаторы для рамы
    void get_front_absorbers(Accessory*, QList<Accessory>&);
    // Метод возвращает передние роторы для рамы
    void get_front_rotors(Accessory*, QList<Accessory>&);
    // Метод возвращает задние амортизаторы для рамы
    void get_rear_absorbers(Accessory*, QList<Accessory>&);
    // Метод возвращает задние роторы для рамы
    void get_rear_rotors(Accessory*, QList<Accessory>&);
    // Метод возвращает стили катания
    void get_riding_styles(QList<QString>&);
    // Метод возвращает количество элементов в таблице
    int get_size(const QString&);
    // Метод возвращает все данные из таблицы
    void get_tbl_data(const QString&, QList<Accessory>&);
    // Метод возвращает колеса для рамы
    void get_wheels(Accessory*, QList<Accessory>&);
    // Метод проверяет, открыто ли соединение с базой данных
    bool is_open();
    // Метод удаляет позицию из таблицы
    bool remove(const QString&, const QList<int>&);

private:
    // Постоянные
    const QString DB_NAME = "data/bike1.db"; // путь к базе данных

    // Соединение с базой данных
    QSqlDatabase db;
};

#endif // DATABASE_H
