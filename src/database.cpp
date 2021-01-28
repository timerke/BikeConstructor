/*
 * Модуль содержит определения методов класса Database.
 */

#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include "const.h"
#include "database.h"
#include "functions.h"


/**
 * Конструктор по умолчанию.
 */
Database::Database()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DB_NAME);
    db.open();
}

/**
 * Деструктор.
 */
Database::~Database()
{
    // Закрываем соединение с базой данных
    db.close();
}

/**
 * Метод добавляет позицию в таблицу.
 * @param tbl: название таблицы;
 * @param a: добавляемая позиция.
 * @return: id, если позиция была добавлена, иначе -1.
 */
int Database::add(const QString &tbl, const Accessory &a) const
{
    // Чтобы проверить вставку позиции в таблицу, будем
    // проверять количество элементов в таблице
    int n0 = get_size(tbl);
    // Формируем текст запроса
    QString q = "INSERT INTO " + tbl + " VALUES (NULL, ";
    int tbl_index = find_tbl_index(tbl);
    int n = FIELDS_TYPES[tbl_index].size();
    for (int i = 1; i < n; i++)
    {
        if (FIELDS_TYPES[tbl_index][i] == "str")
            q += "'" + a.get(i).toString() + "'";
        else
            q += a.get(i).toString();
        if (i != n - 1)
            q += ", ";
    }
    q += ");";
    // Выполняем запрос
    QSqlQuery query;
    query.exec(q);
    if (n0 < get_size(tbl))
    {
        // Получаем id
        query.exec("SELECT MAX(id) FROM " + tbl + ";");
        query.next();
        return query.value(0).toInt();
    }
    return -1;
}

/**
 * Метод возвращает рамы для стиля катания.
 * @param style: стиль катания;
 * @param list: список для сохранения рам.
 */
void Database::get_chassis_for_style(const QString &style,
                                     QList<Accessory> &list) const
{
    QString q = "SELECT * FROM chassis WHERE style='" + style + "'";
    // Получаем индекс таблицы, имена полей и типы значений
    // таблицы
    int tbl_index = find_tbl_index("chassis");
    const QList<QString> NAMES = FIELDS_NAMES[tbl_index];
    const QList<QString> TYPES = FIELDS_TYPES[tbl_index];
    get_data(q, NAMES, TYPES, CHASSIS_RUS, list);
}

/**
 * Метод возвращает данные о компоненте по запросу к базе данных.
 * @param q: текст запроса;
 * @param n: список с именами полей;
 * @param t: список с типами значений полей;
 * @param type: тип компонентов;
 * @param list: список для сохранения компонентов.
 */
void Database::get_data(const QString &q, const QList<QString> &n,
                        const QList<QString> &t, const QString &type,
                        QList<Accessory> &list) const
{
    QSqlQuery query;
    query.exec(q);
    while (query.next())
        list.append(Accessory(n, t, query, type));
}

/**
 * Метод возвращает передние амортизаторы для рамы.
 * @param c: рама;
 * @param list: список для сохранения передних амортизаторов.
 */
void Database::get_front_absorbers(Accessory *c,
                                   QList<Accessory> &list) const
{
    QString rod = c->get("steering_rod").toString();
    QString min_t = c->get("min_travel").toString();
    QString max_t = c->get("max_travel").toString();
    QString rotor = c->get("front_rotor").toString();
    QString wheel = c->get("wheel").toString();
    QString q = "SELECT * FROM front_absorber WHERE steering_rod='"
                + rod + "' AND " + min_t + "<=travel AND travel<=" +
                max_t + " AND rotor=" + rotor + " AND wheel=" + wheel;
    // Получаем индекс таблицы, имена полей и типы значений
    // таблицы
    int tbl_index = find_tbl_index("front_absorber");
    const QList<QString> NAMES = FIELDS_NAMES[tbl_index];
    const QList<QString> TYPES = FIELDS_TYPES[tbl_index];
    get_data(q, NAMES, TYPES, FRONT_ABSORBER_RUS, list);
}

/**
 * Метод возвращает передние роторы для рамы.
 * @param c: рама;
 * @param list: список для сохранения роторов.
 */
void Database::get_front_rotors(Accessory *c,
                                QList<Accessory> &list) const
{
    QString s = c->get("front_rotor").toString();
    QString q = "SELECT * FROM rotor WHERE size=" + s;
    // Получаем индекс таблицы, имена полей и типы значений
    // таблицы
    int tbl_index = find_tbl_index("rotor");
    const QList<QString> NAMES = FIELDS_NAMES[tbl_index];
    const QList<QString> TYPES = FIELDS_TYPES[tbl_index];
    get_data(q, NAMES, TYPES, FRONT_ROTOR_RUS, list);
}

/**
 * Метод возвращает задние амортизаторы для рамы.
 * @param c: рама;
 * @param list: список для сохранения задних амортизаторов.
 */
void Database::get_rear_absorbers(Accessory *c,
                                  QList<Accessory> &list) const
{
    QString s = c->get("rear_absorber").toString();
    QString q = "SELECT * FROM rear_absorber WHERE size='" +
                s + "'";
    // Получаем индекс таблицы, имена полей и типы значений
    // таблицы
    int tbl_index = find_tbl_index("rear_absorber");
    const QList<QString> NAMES = FIELDS_NAMES[tbl_index];
    const QList<QString> TYPES = FIELDS_TYPES[tbl_index];
    get_data(q, NAMES, TYPES, REAR_ABSORBER_RUS, list);
}

/**
 * Метод возвращает задние роторы для рамы.
 * @param c: рама;
 * @param list: список для сохранения роторов.
 */
void Database::get_rear_rotors(Accessory *c,
                               QList<Accessory> &list) const
{
    QString s = c->get("rear_rotor").toString();
    QString q = "SELECT * FROM rotor WHERE size=" + s;
    // Получаем индекс таблицы, имена полей и типы значений
    // таблицы
    int tbl_index = find_tbl_index("rotor");
    const QList<QString> NAMES = FIELDS_NAMES[tbl_index];
    const QList<QString> TYPES = FIELDS_TYPES[tbl_index];
    get_data(q, NAMES, TYPES, REAR_ROTOR_RUS, list);
}

/**
 * Метод возвращает стили катания.
 * @param list: список для сохранения стилей катания.
 */
void Database::get_riding_styles(QList<QString>& list) const
{
    QSqlQuery query;
    query.exec("SELECT DISTINCT style FROM chassis");
    while (query.next())
        list.append(query.value(0).toString());
}

/**
 * Метод возвращает количество элементов в таблице.
 * @param tbl: название таблицы.
 * @return: количество элементов.
 */
int Database::get_size(const QString& tbl) const
{
    QSqlQuery query;
    query.exec("SELECT COUNT(id) FROM " + tbl + ";");
    query.next();
    return query.value(0).toInt();
}

/**
 * Метод возвращает все данные из таблицы.
 * @param tbl_name: имя таблицы;
 * @param list: список для сохранения данных.
 */
void Database::get_tbl_data(const QString &tbl_name,
                            QList<Accessory> &list) const
{
    QString q = "SELECT * FROM " + tbl_name;
    // Получаем индекс таблицы, имена полей и типы значений
    // таблицы
    int tbl_index = find_tbl_index(tbl_name);
    const QList<QString> NAMES = FIELDS_NAMES[tbl_index];
    const QList<QString> TYPES = FIELDS_TYPES[tbl_index];
    get_data(q, NAMES, TYPES, WHEEL_RUS, list);
}

/**
 * Метод возвращает колеса для рамы.
 * @param c: рама;
 * @param list: список для сохранения колес.
 */
void Database::get_wheels(Accessory *c,
                          QList<Accessory> &list) const
{
    QString d = c->get("wheel").toString();
    QString q = "SELECT * FROM wheel WHERE diameter=" + d;
    // Получаем индекс таблицы, имена полей и типы значений
    // таблицы
    int tbl_index = find_tbl_index("wheel");
    const QList<QString> NAMES = FIELDS_NAMES[tbl_index];
    const QList<QString> TYPES = FIELDS_TYPES[tbl_index];
    get_data(q, NAMES, TYPES, WHEEL_RUS, list);
}

/**
 * Метод проверяет, открыто ли соединение с базой данных.
 * @return: true, если соединение открыто, иначе false.
 */
bool Database::is_open() const
{
    return db.isOpen();
}

/**
 * Метод удаляет позиции из таблицы.
 * @param tbl: название таблицы;
 * @param ids: список id позиций, которые нужно удалить.
 * @return: true, если позиции удалены, иначе false.
 */
bool Database::remove(const QString &tbl,
                      const QList<int> &ids) const
{
    // Чтобы проверить вставку позиции в таблицу, будем
    // проверять количество элементов в таблице
    int n0 = get_size(tbl);
    // Формируем текст запроса
    QString q = "DELETE FROM " + tbl + " WHERE id IN (";
    for (int i = 0; i < ids.size(); i++)
    {
        q += QString("%1").arg(ids[i]);
        if (i != ids.size() - 1)
            q += ", ";
    }
    q += ");";
    // Выполняем запрос
    QSqlQuery query;
    query.exec(q);
    if (n0 > get_size(tbl))
        return true;
    return false;
}
