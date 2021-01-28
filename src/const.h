/*
 * Заголовочный файл с постоянными.
 */

#ifndef CONST_H
#define CONST_H

#include <QList>
#include <QString>
#include <QStringList>


// Тип режима работы приложения: администратор или пользователь
enum Mode {ADMIN_MODE, USER_MODE};

const QString CHASSIS = "chassis";
const QString CHASSIS_RUS = "Рама";
const QString FRONT_ABSORBER = "front_absorber";
const QString FRONT_ABSORBER_RUS = "Передний амортизатор";
const QString FRONT_ROTOR_RUS = "Передний ротор";
const QString REAR_ABSORBER = "rear_absorber";
const QString REAR_ABSORBER_RUS = "Задний амортизатор";
const QString REAR_ROTOR_RUS = "Задний ротор";
const QString ROTOR = "rotor";
const QString WHEEL = "wheel";
const QString WHEEL_RUS = "Колесо";

// Названия таблиц по-английски
const QStringList TBLS_ENG =
    {"", CHASSIS, FRONT_ABSORBER, REAR_ABSORBER, WHEEL,
     ROTOR, ROTOR};
// Названия таблиц по-русски
const QStringList TBLS_RUS =
    {"", CHASSIS_RUS, FRONT_ABSORBER_RUS, REAR_ABSORBER_RUS,
     WHEEL_RUS, FRONT_ROTOR_RUS, REAR_ROTOR_RUS};
// Названия полей таблиц по-английски
const QList<QStringList> FIELDS_NAMES =
    {{},
     {"id", "name", "style", "steering_rod", "min_travel",
      "max_travel", "rear_absorber", "wheel", "front_rotor",
      "rear_rotor", "price"},
     {"id", "name", "travel", "steering_rod", "rotor", "wheel",
      "price"},
     {"id", "name", "size", "travel", "type", "price"},
     {"id", "name", "diameter", "price"},
     {"id", "name", "size", "price"},
     {"id", "name", "size", "price"},
     };
// Названия полей таблиц по-русски
const QList<QStringList> FIELDS_NAMES_RUS =
    {{},
     {"Id", "Название", "Стиль катания", "Рулевой шток",
      "Ход переднего амортизатора (min)",
      "Ход переднего амортизатора (max)",
      "Размер заднего амортизатора", "Диаметр колеса",
      "Размер ротора спереди", "Размер ротора сзади", "Цена"},
     {"Id", "Название", "Ход", "Рулевой шток", "Размер ротора",
      "Диаметр колеса", "Цена"},
     {"Id", "Название", "Размер", "Ход", "Тип", "Цена"},
     {"Id", "Название", "Диаметр", "Цена"},
     {"Id", "Название", "Размер", "Цена"},
     {"Id", "Название", "Размер", "Цена"},
     };
// Типы значений полей таблиц
const QList<QStringList> FIELDS_TYPES =
    {{},
     {"int", "str", "str", "str", "int", "int", "str", "float",
      "int", "int", "int"},
     {"int", "str", "int", "str", "int", "float", "int"},
     {"int", "str", "str", "int", "str", "int"},
     {"int", "str", "float", "int"},
     {"int", "str", "int", "int"},
     {"int", "str", "int", "int"}};

#endif // CONST_H
