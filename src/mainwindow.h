/*
 * Заголовочный файл содержит объявление класса MainWindow для
 * реализации главного окна приложения.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFormLayout>
#include <QPushButton>
#include <QStackedLayout>
#include <QTableWidget>
#include <QWidget>
#include "accessory.h"
#include "basket.h"
#include "const.h"
#include "database.h"


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    // Конструктор по умолчанию
    MainWindow(QWidget *parent = nullptr);
    // Деструктор
    ~MainWindow();

private:
    const QString ICON = "data/bike.png"; // путь к иконке

    Basket basket; // корзина
    QList<Accessory> chassis; // рамы
    QList<Accessory> front_absorbers; // передние амортизаторы
    QList<Accessory> front_rotors; // передние роторы
    QList<Accessory> rear_absorbers; // задние амортизаторы
    QList<Accessory> rear_rotors; // задние роторы
    QList<Accessory> wheels; // колеса

    Database *db; // объект для работы с базой данных
    QStackedLayout *stacked_layout;
    Accessory *selected_chassis; // выбранная рама
    int selected_row = -1; // выбранная строка в таблице
    int tbl_index; // индекс таблицы, выбранной из базы данных
    Mode work_mode = USER_MODE; // режим работы приложения

    QPushButton *btn; // переменная для кнопки
    QTableWidget *tbl; // переменная для таблицы

    // Метод добавляет позицию в таблицу
    void add_item();
    // Метод создает окно для выбора комплектующих
    QWidget* create_accessories_wnd();
    // Метод создает окно для выбора рамы из каталога для
    // стиля катания
    QWidget* create_chassis_for_style_wnd(QList<Accessory>&);
    // Метод создает выпадающие списки
    void create_combo_box(QFormLayout*, QList<Accessory>*,
                          const QString&);
    // Метод создает исходное окно
    QWidget* create_initial_wnd();
    // Метод создает текстовую метку из типа компонента, его
    // названия и цены
    QString create_label(QString&, int, int&);
    // Метод создает окно с выбранными комплектующими
    QWidget* create_selected_accessories_wnd();
    // Метод создает окно с выбранной таблицей из базы данных
    QWidget* create_selected_tbl_wnd();
    // Метод создает окно для выбора таблицы из базы данных для
    // администрирования
    QWidget* create_selection_for_tbls_wnd();
    // Метод создает окно для выбора стиля катания
    QWidget* create_style_wnd(QList<QString>&);
    // Метод создает шаблон окна с кнопками 'В начало' и 'Назад'
    // и текстом
    QVBoxLayout* create_template_wnd(QWidget*, const QString&);
    // Метод создает окно для ввода характеристик собственной
    // рамы пользователя
    QWidget* create_user_chassis_wnd();
    // Метод удаляет позицию из таблицы
    void delete_item();
    // Метод отображает окно для выбора комплектующих для
    // выбранной рамы
    void show_accessories_wnd();
    // Метод отображает окно для выбора рамы из каталога для
    // выбранного пользователем стиля катания
    void show_chassis_for_style_wnd();
    // Метод отображает исходное окно
    void show_initial_wnd();
    // Метод отображает предыдущее окно
    void show_previous_wnd();
    // Метод отображает окно с выбранными комплектующими
    void show_selected_accessories_wnd();
    // Метод отображает окно с выбранной таблицей из базы данных
    void show_selected_tbl_wnd();
    // Метод отображает окно для выбора стиля катания
    void show_style_wnd();
    // Метод отображает окно для ввода характеристик
    // собственной рамы пользователя
    void show_user_chassis_wnd();
    // Метод завершает работу приложения, когда нет соединения
    // с базой данных
    void quit();

private slots:
    // Слот для чтения данных из текстового поля
    void read_value_from_line_edit(QString);
    // Слот задает новую таблицу из базы данных для редактирования
    void set_new_tbl(int);
    // Слот задает новое значение для комплектующего, выбранного
    // пользователем
    void set_new_value(int);
    // Слот для переключения режима работы в режим администратора
    void switch_to_admin();
    // Слот для переключения режима работы в режим пользователя
    void switch_to_user();
};

#endif // MAINWINDOW_H
