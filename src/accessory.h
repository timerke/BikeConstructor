/*
 * Заголовочный файл содержит объявление класса Accessory для
 * работы с комплектующими велосипеда.
 */

#ifndef CONTENT_H
#define CONTENT_H

#include <QList>
#include <QString>
#include <QSqlQuery>


/**
 * Класс для работы с компонентами велосипеда.
 */
class Accessory
{
private:
    // Список значений полей типа float
    QList<float> float_;
    // Список значений полей типа int
    QList<int> int_;
    // Список значений полей типа str
    QList<QString> str_;
    // Список названий полей
    QList<QString> names;
    // Список типов полей
    QList<QString> types;
    // Список необходимых полей
    QList<int> required;

public:
    // Тип компонента
    QString type;

    // Конструктор по умолчанию
    Accessory();
    // Конструктор инициализации
    Accessory(const QList<QString>&, const QList<QString>&);
    // Конструктор инициализации
    Accessory(const QList<QString>&, const QList<QString>&,
              const QList<int>&, const QString&);
    // Конструктор инициализации
    Accessory(const QList<QString>&, const QList<QString>&,
              const QSqlQuery&, const QString&);
    // Метод возвращает значение поля по его имени
    QVariant get(const QString&) const;
    // Метод возвращает значение поля по его индексу
    QVariant get(int) const;
    // Метод инициализирует нулевыми значениями все поля
    void init();
    // Метод задает значения полей по запросу из базы данных
    void init(const QSqlQuery&);
    // Метод проверяет, что все необходимые поля имеют значения
    bool is_empty() const;
    // Метод задает значение поля по его имени
    void set(const QString&, QVariant);
    // Метод задает значение поля по его индексу
    void set(int, QVariant);
    // Метод задает значения полей
    void set(const QList<int>&, const QList<float>&,
             const QList<QString>&);
    // Метод задает значения полей путем копирования аналогичного
    // объекта
    void set(const Accessory&);
    // Метод задает поля, значения которых должны быть заданы
    // обязательно
    void set_required(QList<int>&);
    // Метод возвращает количество полей
    int size() const;
};

#endif // CONTENT_H
