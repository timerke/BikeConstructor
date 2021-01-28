/*
 * Модуль содержит определения методов класса Accessory.
 */

#include <QVariant>
#include "accessory.h"


/**
 * Конструктор по умолчанию.
 */
Accessory::Accessory() {}

/**
 * Конструктор инициализации.
 * @param names: список названий полей;
 * @param types: список типов полей.
 */
Accessory::Accessory(const QList<QString> &names,
                     const QList<QString> &types)
{
    this->names = names;
    this->types = types;
}

/**
 * Конструктор инициализации.
 * @param names: список названий полей;
 * @param types: список типов полей;
 * @param required: список необходимых полей;
 * @param type: тип компонента.
 */
Accessory::Accessory(const QList<QString> &names,
                     const QList<QString> &types,
                     const QList<int> &required,
                     const QString &type)
{
    this->names = names;
    this->types = types;
    this->required = required;
    this->type = type;
    init();
}

/**
 * Конструктор инициализации.
 * @param names: список названий полей;
 * @param types: список типов полей;
 * @param query: запрос к базе данных'
 * @param type: тип компонента.
 */
Accessory::Accessory(const QList<QString> &names,
                     const QList<QString> &types,
                     const QSqlQuery &query,
                     const QString &type)
{
    this->names = names;
    this->types = types;
    this->type = type;
    init(query);
}

/**
 * Метод возвращает значение поля по его имени.
 * @param name: имя поля.
 * @return: значение.
 */
QVariant Accessory::get(const QString &name) const
{
    int int_i = 0;
    int float_i = 0;
    int str_i = 0;
    for (int i = 0; i < names.size(); i++)
    {
        if (names[i] == name)
        {
            if (types[i] == "int")
                return int_[int_i];
            if (types[i] == "float")
                return float_[float_i];
            if (types[i] == "str")
                return str_[str_i];
        }
        if (types[i] == "int")
            int_i++;
        else if (types[i] == "float")
            float_i++;
        else if (types[i] == "str")
            str_i++;
    }
    return 0;
}

/**
 * Метод возвращает значение поля по его индексу.
 * @param index: индекс поля.
 * @return: значение.
 */
QVariant Accessory::get(int index) const
{
    int int_i = 0;
    int float_i = 0;
    int str_i = 0;
    for (int i = 0; i < types.size(); i++)
    {
        if (i == index)
        {
            if (types[i] == "int")
                return int_[int_i];
            if (types[i] == "float")
                return float_[float_i];
            if (types[i] == "str")
                return str_[str_i];
        }
        if (types[i] == "int")
            int_i++;
        else if (types[i] == "float")
            float_i++;
        else if (types[i] == "str")
            str_i++;
    }
    return 0;
}

/**
 * Метод инициализирует нулевыми значениями все поля.
 */
void Accessory::init()
{
    int_.clear();
    float_.clear();
    str_.clear();
    for (int i = 0; i < types.size(); i++)
    {
        if (types[i] == "int")
            int_.append(0);
        else if (types[i] == "float")
            float_.append(0);
        else if (types[i] == "str")
            str_.append(NULL);
    }
}

/**
 * Метод задает значения полей по запросу из базы данных.
 * @param query: запрос к базе данных.
 */
void Accessory::init(const QSqlQuery &query)
{
    for (int i = 0; i < types.size(); i++)
    {
        if (types[i] == "int")
        {
            int spam = query.value(i).toInt();
            int_.append(spam);
        }
        else if (types[i] == "float")
        {
            float spam = query.value(i).toFloat();
            float_.append(spam);
        }
        else if (types[i] == "str")
        {
            QString spam = query.value(i).toString();
            str_.append(spam);
        }
    }
}

/**
 * Метод проверяет, что все необходимые поля имеют значения.
 * @return: true, если некоторые поля пустые, иначе false.
 */
bool Accessory::is_empty()
{
    for (auto i : required)
    {
        if (types[i] == "int" && get(i).toInt() == 0)
            return true;
        if (types[i] == "float" && get(i).toFloat() == 0)
            return true;
        if (types[i] == "str" && get(i).toString() == NULL)
            return true;
    }
    return false;
}

/**
 * Метод задает значение поля по его имени.
 * @param name: имя поля;
 * @param value: значение.
 */
void Accessory::set(const QString& name, QVariant value)
{
    int int_i = 0;
    int float_i = 0;
    int str_i = 0;
    for (int i = 0; i < names.size(); i++)
    {
        if (names[i] == name)
        {
            if (types[i] == "int")
                int_[int_i] = value.toInt();
            if (types[i] == "float")
                float_[float_i] = value.toFloat();
            if (types[i] == "str")
                str_[str_i] = value.toString();
            return;
        }
        if (types[i] == "int")
            int_i++;
        else if (types[i] == "float")
            float_i++;
        else if (types[i] == "str")
            str_i++;
    }
}

/**
 * Метод задает значение поля по его индексу.
 * @param index: индекс поля;
 * @param value: значение.
 */
void Accessory::set(int index, QVariant value)
{
    int int_i = 0;
    int float_i = 0;
    int str_i = 0;
    for (int i = 0; i < types.size(); i++)
    {
        if (i == index)
        {
            if (types[i] == "int")
                int_[int_i] = value.toInt();
            if (types[i] == "float")
                float_[float_i] = value.toFloat();
            if (types[i] == "str")
                str_[str_i] = value.toString();
            return;
        }
        if (types[i] == "int")
            int_i++;
        else if (types[i] == "float")
            float_i++;
        else if (types[i] == "str")
            str_i++;
    }
}

/**
 * Метод задает значения полей.
 * @param i: список с int значениями полей;
 * @param f: список с float значениями полей;
 * @param s: список со str значениями полей.
 */
void Accessory::set(const QList<int> &i, const QList<float> &f,
                    const QList<QString> &s)
{
    int_ = i;
    float_ = f;
    str_ = s;
}

/**
 * Метод задает значения полей.
 * @param a: объект-компонент.
 */
void Accessory::set(const Accessory &a)
{
    int_ = a.int_;
    float_ = a.float_;
    str_ = a.str_;
}

/**
 * Метод задает поля, значения которых должны быть заданы
 * обязательно.
 * @param required: список с номерами индексов необходимых
 * полей.
 */
void Accessory::set_required(QList<int> &required)
{
    this->required = required;
}

/**
 * Метод возвращает количество полей.
 * @return: количество полей.
 */
int Accessory::size() const
{
    return types.size();
}
