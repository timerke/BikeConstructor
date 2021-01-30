/*
 * Модуль содержит определения методов класса MainWindow.
 */

#include <iostream>
#include <QApplication>
#include <QComboBox>
#include <QFormLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include "mainwindow.h"


/**
 * Конструктор класса главного окна приложения.
 */
MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    // Устанавливаем иконку, название и размеры главного
    // окна приложения
    setWindowIcon(QIcon(ICON));
    setWindowTitle("Конструктор велосипедов");
    setGeometry(200, 300, 500, 400);
    // Соединяемся с базой данных
    db = new Database();
    // Создаем исходное окно приложения
    stacked_layout = new QStackedLayout;
    stacked_layout->addWidget(create_initial_wnd());
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(stacked_layout);
    // Создаем меню
    QMenuBar *main_menu = new QMenuBar();
    QMenu *mode_menu = new QMenu("Режим работы");
    /*
    mode_menu->addAction("Администратор", this,
                         &MainWindow::switch_to_admin);
    mode_menu->addAction("Пользователь", this,
                         &MainWindow::switch_to_user);
    */
    mode_menu->addAction("Администратор", this,
                         SLOT(switch_to_admin()));
    mode_menu->addAction("Пользователь", this,
                         SLOT(switch_to_user()));
    main_menu->addMenu(mode_menu);
    main_layout->setMenuBar(main_menu);
    setLayout(main_layout);
    stacked_layout->setCurrentIndex(0);
    // Приложение по умолчанию работает в режиме пользователя
    work_mode = USER_MODE;
    // Создаем объект-раму
    const QList<QString> NAMES = FIELDS_NAMES[1]; // имена полей
    const QList<QString> TYPES = FIELDS_TYPES[1]; // типы полей
    const QList<int> REQUIRED = {3, 4, 5, 7, 9}; // необходимые поля
    selected_chassis = new Accessory(NAMES, TYPES, REQUIRED, CHASSIS_RUS);
}

/**
 * Деструктор класса.
 */
MainWindow::~MainWindow()
{
    delete db;
    delete selected_chassis;
    delete stacked_layout;
}

/**
 * Метод добавляет позицию в таблицу.
 */
void MainWindow::add_item()
{
    // Количество полей в таблице
    int n = FIELDS_TYPES[tbl_index].size();
    Accessory a(FIELDS_NAMES[tbl_index], FIELDS_TYPES[tbl_index]);
    a.init();
    // Получаем значения всех полей, кроме id
    for (int i = 1; i < n; i++)
    {
        bool ok = false;
        QVariant info;
        if (FIELDS_TYPES[tbl_index][i] == "str")
            info = QInputDialog::getText(
                this, TBLS_RUS[tbl_index], FIELDS_NAMES_RUS[tbl_index][i],
                QLineEdit::Normal, "", &ok);
        else if (FIELDS_TYPES[tbl_index][i] == "int")
            info = QInputDialog::getInt(
                this, TBLS_RUS[tbl_index], FIELDS_NAMES_RUS[tbl_index][i],
                0, 0, 100000, 1, &ok);
        else if (FIELDS_TYPES[tbl_index][i] == "float")
            info = QInputDialog::getDouble(
                this, TBLS_RUS[tbl_index], FIELDS_NAMES_RUS[tbl_index][i],
                0, 0, 100000, 1, &ok);
        if (ok)
            a.set(i, info);
        else
            // Нажата кнопка Отмена, прекращаем сбор данных
            return;
    }
    // Записываем новую позицию в базу данных
    int id = db->add(TBLS_ENG[tbl_index], a);
    if (id != -1)
    {
        // Позиция была добавлена, добавляем ее в виджет
        int COLUMN = FIELDS_TYPES[tbl_index].size();
        int row = tbl->rowCount();
        tbl->setRowCount(row + 1);
        a.set(0, id);
        for (int column = 0; column < COLUMN; column++)
        {
            QString s = a.get(column).toString();
            QTableWidgetItem *item = new QTableWidgetItem(s);
            tbl->setItem(row, column, item);
        }
    }
    else
    {
        // Позиция не была добавлена
        QMessageBox msg_box;
        msg_box.setText("Ошибка! Позиция не была добавлена в таблицу '" +
                        TBLS_RUS[tbl_index] + "'");
        msg_box.setWindowTitle("Ошибка");
        msg_box.exec();
    }
}

/**
 * Метод создает окно для выбора комплектующих.
 * @return: окно.
 */
QWidget* MainWindow::create_accessories_wnd()
{
    // Размещаем выпадающие списки в макете форм
    QFormLayout *form = new QFormLayout();
    QList<QList<Accessory>*> lists =
        {&front_absorbers, &rear_absorbers, &wheels,
         &front_rotors, &rear_rotors};
    QList<QString> names =
        {FRONT_ABSORBER_RUS, REAR_ABSORBER_RUS, WHEEL_RUS,
         FRONT_ROTOR_RUS, REAR_ROTOR_RUS};
    for (int i = 0; i < lists.size(); i++)
        create_combo_box(form, lists[i], names[i]);
    // Создаем макет окна
    QWidget *widget = new QWidget;
    QVBoxLayout *vbox = create_template_wnd(
        widget, "Выберите комплектующие");
    // Размещаем макет формы на окне
    vbox->addLayout(form, 1);
    // Размещаем кнопку для подсчета общей стоимости
    QPushButton *btn = new QPushButton("Вычислить стоимость");
    connect(btn, &QPushButton::clicked, this,
            &MainWindow::show_selected_accessories_wnd);
    btn->setFixedSize(200, 50);
    vbox->addWidget(btn, 1, Qt::AlignHCenter);
    widget->setLayout(vbox);
    return widget;
}

/**
 * Метод создает окно для выбора рамы из каталога для стиля
 * катания.
 * @param c: список рам.
 * @return: окно.
 */
QWidget* MainWindow::create_chassis_for_style_wnd(QList<Accessory> &c)
{
    // Размещаем кнопки со стилем катания и цены
    QGridLayout *grid = new QGridLayout();
    for (int i = 0; i < c.size(); i++)
    {
        // Создаем кнопку с названием рамы
        QPushButton *btn = new QPushButton(c[i].get("name").toString());
        btn->setProperty("type", "catalog");
        btn->setProperty("index", i);
        connect(btn, &QPushButton::clicked, this,
                &MainWindow::show_accessories_wnd);
        grid->addWidget(btn, i, 0);
        // Создаем метку с ценой
        QLabel *label = new QLabel("Цена " + c[i].get("price").toString());
        label->setAlignment(Qt::AlignHCenter);
        grid->addWidget(label, i, 1);
    }
    grid->setAlignment(Qt::AlignTop);
    // Создаем макет окна
    QWidget *widget = new QWidget;
    QVBoxLayout *vbox = create_template_wnd(widget, "Выберите раму");
    vbox->addLayout(grid, 1);
    widget->setLayout(vbox);
    return widget;
}

/**
 * Метод создает выпадающие списки.
 * @param l: макет, в который нужно добавить список;
 * @param list: список данных, которые нужно записать в выпадающий
 * список;
 * @param name: имя выпадающего списка.
 */
void MainWindow::create_combo_box(
    QFormLayout *l, QList<Accessory> *list, const QString &name)
{
    QComboBox *cbox = new QComboBox();
    cbox->addItem("");
    for (int i = 0; i < list->size(); i++)
    {
        QString str = (*list)[i].get("name").toString() + " - " +
                      (*list)[i].get("price").toString();
        cbox->addItem(str);
    }
    cbox->setObjectName(name);
    /*
    connect(cbox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::set_new_value);
    */
    connect(cbox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(set_new_value(int)));
    l->addRow(name, cbox);
}

/**
 * Метод создает исходное окно.
 * @return: исходное окно.
 */
QWidget* MainWindow::create_initial_wnd()
{
    // Размещаем две кнопки рядом по горизонтали
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setSpacing(20);
    QPushButton *btn = new QPushButton(
        "Собрать велосипед с рамой из каталога");
    btn->setSizePolicy(QSizePolicy::Expanding,
                       QSizePolicy::Expanding);
    connect(btn, &QPushButton::clicked, this,
            &MainWindow::show_style_wnd);
    btn->setMinimumHeight(50);
    hbox->addWidget(btn);
    btn = new QPushButton("Собрать велосипед с собственной рамой");
    btn->setSizePolicy(QSizePolicy::Expanding,
                       QSizePolicy::Expanding);
    connect(btn, &QPushButton::clicked, this,
            &MainWindow::show_user_chassis_wnd);
    btn->setMinimumHeight(50);
    hbox->addWidget(btn);
    // Создаем макет окна
    QWidget *widget = new QWidget;
    QVBoxLayout *vbox = create_template_wnd(
        widget, "Как вы будете собирать велосипед?");
    vbox->addLayout(hbox);
    widget->setLayout(hbox);
    return widget;
}

/**
 * Метод создает окно с выбранными комплектующими.
 */
QWidget* MainWindow::create_selected_accessories_wnd()
{
    // Размещаем список всех комплектующих из корзины
    QTableWidget *tbl = new QTableWidget();
    tbl->setColumnCount(3);
    tbl->setRowCount(basket.get_size());
    tbl->setHorizontalHeaderLabels({"Тип", "Название", "Цена"});
    for (int i = 0; i < basket.get_size(); i++)
    {
        Accessory a = basket.at(i);
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item1->setText(a.type);
        tbl->setItem(i, 0, item1);
        QTableWidgetItem *item2 = new QTableWidgetItem();
        item2->setText(a.get("name").toString());
        tbl->setItem(i, 1, item2);
        QTableWidgetItem *item3 = new QTableWidgetItem();
        item3->setText(a.get("price").toString());
        tbl->setItem(i, 2, item3);
    }
    // Создаем макет окна
    QWidget *widget = new QWidget;
    QVBoxLayout *vbox = create_template_wnd(
        widget, "Выбранные комплектующие");
    vbox->addWidget(tbl);
    // Размещаем метку с полной стоимостью корзины
    QLabel *label = new QLabel("Полная стоимость " +
                               basket.get_price().toString());
    vbox->addWidget(label, 0, Qt::AlignHCenter);
    widget->setLayout(vbox);
    return widget;
}

/**
 * Метод создает окно с выбранной таблицей из базы данных.
 */
QWidget* MainWindow::create_selected_tbl_wnd()
{
    // Получаем имя таблицы и данные из этой таблицы
    QString tbl_name = TBLS_ENG[tbl_index];
    QList<Accessory> tbl_data;
    db->get_tbl_data(tbl_name, tbl_data);
    // Создаем таблицу-виджет
    tbl = new QTableWidget();
    const int ROW = tbl_data.size();
    const int COLUMN = tbl_data[0].size();
    tbl->setRowCount(ROW);
    tbl->setColumnCount(COLUMN);
    tbl->setHorizontalHeaderLabels(FIELDS_NAMES_RUS[tbl_index]);
    for (int row = 0; row < ROW; row++)
    {
        for (int column = 0; column < COLUMN; column++)
        {
            QString s = tbl_data[row].get(column).toString();
            QTableWidgetItem *item = new QTableWidgetItem(s);
            tbl->setItem(row, column, item);
        }
    }
    // Добавляем кнопки редактирования
    QHBoxLayout *hbox = new QHBoxLayout();
    QPushButton *btn = new QPushButton("Добавить позицию");
    btn->setFixedSize(200, 50);
    connect(btn, &QPushButton::clicked, this,
            &MainWindow::add_item);
    hbox->addWidget(btn);
    btn = new QPushButton("Удалить позицию");
    btn->setFixedSize(200, 50);
    connect(btn, &QPushButton::clicked, this,
            &MainWindow::delete_item);
    hbox->addWidget(btn);
    // Создаем макет окна
    QWidget *widget = new QWidget;
    QVBoxLayout *vbox = create_template_wnd(
        widget, TBLS_RUS[tbl_index]);
    vbox->addWidget(tbl);
    vbox->addLayout(hbox);
    widget->setLayout(vbox);
    return widget;
}

/**
 * Метод создает окно для выбора таблицы из базы данных для
 * администрирования.
 */
QWidget* MainWindow::create_selection_for_tbls_wnd()
{
    // Создаем макет окна
    QWidget *widget = new QWidget;
    QVBoxLayout *vbox = create_template_wnd(
        widget, "Выберите таблицу для редактрирования");
    // Создаем список с таблицами
    QComboBox *combo = new QComboBox(widget);
    combo->addItems(TBLS_RUS);
    /*
    connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::set_new_tbl);
    */
    connect(combo, SIGNAL(currentIndexChanged(int)), this,
            SLOT(set_new_tbl(int)));
    vbox->addWidget(combo, 1, Qt::AlignVCenter);
    // Создаем кнопку выбора
    btn = new QPushButton("Выбрать", widget);
    connect(btn, &QPushButton::clicked, this,
            &MainWindow::show_selected_tbl_wnd);
    btn->setEnabled(false);
    btn->setFixedSize(100, 50);
    vbox->addWidget(btn, 0, Qt::AlignHCenter);
    widget->setLayout(vbox);
    return widget;
}

/**
 * Метод создает окно для выбора стиля катания.
 * @param styles: список стилей катания.
 * @return: окно.
 */
QWidget* MainWindow::create_style_wnd(QList<QString> &styles)
{
    // Создаем макет окна
    QWidget *widget = new QWidget;
    QVBoxLayout *vbox = create_template_wnd(
        widget, "Выберите стиль катания");
    // Размещаем кнопки со стилями катания по вертикали
    for (auto style: styles)
    {
        QPushButton *btn = new QPushButton(style, widget);
        btn->setSizePolicy(QSizePolicy::Expanding,
                           QSizePolicy::Expanding);
        btn->setProperty("style", style);
        connect(btn, &QPushButton::clicked, this,
                &MainWindow::show_chassis_for_style_wnd);
        vbox->addWidget(btn);
    }
    widget->setLayout(vbox);
    return widget;
}

/**
 * Метод создает шаблон окна с кнопками 'В начало' и 'Назад'
 * и текстом.
 * @param w: виджет, для которого создается шаблон;
 * @param text: текст.
 */
QVBoxLayout* MainWindow::create_template_wnd(
    QWidget *w, const QString &text)
{
    // Создаем макет вертикального расположения виджетов
    QVBoxLayout *vbox = new QVBoxLayout(w);
    vbox->setSpacing(20);
    // Размещаем кнопки 'В начало' и 'Назад'
    QHBoxLayout *hbox = new QHBoxLayout();
    QPushButton *btn = new QPushButton("В начало", w);
    btn->setFixedSize(100, 50);
    connect(btn, &QPushButton::clicked, this,
            &MainWindow::show_initial_wnd);
    hbox->addWidget(btn);
    btn = new QPushButton("Назад", w);
    btn->setFixedSize(100, 50);
    connect(btn, &QPushButton::clicked, this,
            &MainWindow::show_previous_wnd);
    hbox->addWidget(btn);
    vbox->addLayout(hbox);
    // Размещаем сообщение
    QLabel *label = new QLabel(text, w);
    vbox->addWidget(label, 0, Qt::AlignHCenter | Qt::AlignTop);
    return vbox;
}

/**
 * Метод создает окно для ввода характеристик собственной
 * рамы пользователя.
 * @return: окно.
 */
QWidget* MainWindow::create_user_chassis_wnd()
{
    // Размещаем текстовые поля в макете для формы
    QFormLayout *form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    // Названия текстовых полей
    const QList<QString> TITLES =
        {"Тип рулевого штока",
         "Длина хода переднего амортизатора",
         "Размер заднего амортизатора",
         "Диаметр колеса",
         "Размер переднего ротора",
         "Размер заднего ротора"};
    // Регулярные выражения для валидаторов текстовых полей
    const QList<QString> REGS =
        {"^[а-яА-Я]+$", "^[0-9]+-?[0-9]*$", "^[0-9]+x[0-9]+$",
         "^[0-9]+[\\.]?[0-9]*$", "^[0-9]*$", "^[0-9]+$"};
    // Индексы полей
    const QList<int> INDICES = {3, 4, 6, 7, 8, 9};
    // В цикле создаем текстовые поля и добавляем в макет
    for (int i = 0; i < TITLES.size(); i++)
    {
        QLineEdit *line_edit = new QLineEdit();
        line_edit->setProperty("index", INDICES[i]);
        connect(line_edit, &QLineEdit::textChanged, this,
                &MainWindow::read_value_from_line_edit);
        // Задаем валидатор
        QRegularExpression reg(REGS[i]);
        QValidator *v = new QRegularExpressionValidator(reg);
        line_edit->setValidator(v);
        form->addRow(TITLES[i], line_edit);
    }
    form->setAlignment(Qt::AlignTop);
    // Создаем макет окна
    QWidget *widget = new QWidget;
    QVBoxLayout *vbox = create_template_wnd(
        widget, "Введите характеристики рамы");
    vbox->addLayout(form, 1);
    // Размещаем кнопку для поиска комплектующих
    btn = new QPushButton("Искать комплектующие");
    btn->setFixedSize(200, 50);
    btn->setEnabled(false);
    connect(btn, &QPushButton::clicked, this,
            &MainWindow::show_accessories_wnd);
    vbox->addWidget(btn, 0, Qt::AlignHCenter);
    widget->setLayout(vbox);
    return widget;
}

/**
 * Метод удаляет позицию из таблицы.
 */
void MainWindow::delete_item()
{
    QList<QTableWidgetSelectionRange> s = tbl->selectedRanges();
    if (s.isEmpty())
        // В таблице не выбрано ничего
        return;
    QList<int> ids; // список id выбранных позиций
    QList<int> rows; // список номеров выбранных строк
    for (auto i : s)
    {
        for (int row = i.topRow(); row <= i.bottomRow(); row++)
        {
            if (rows.contains(row))
                continue;
            ids.append(tbl->item(row, 0)->text().toInt());
            rows.append(row);
        }
    }
    // Сортируем список строк по убыванию
    std::sort(rows.begin(), rows.end(), std::greater<int>());
    // qSort(rows.begin(), rows.end(), qGreater<int>());
    if (db->remove(TBLS_ENG[tbl_index], ids))
    {
        // Позиции были удалены, удаляем из виджета
        for (auto row : rows)
            tbl->removeRow(row);
    }
    else
    {
        // Позиции не были удалены
        QMessageBox msg_box;
        msg_box.setText(
            "Ошибка! Позиции не были удалены из таблицы '" +
            TBLS_RUS[tbl_index] + "'");
        msg_box.setWindowTitle("Ошибка");
        msg_box.exec();
    }
}

/**
 * Слот считывает значения, введенные в текстовые поля
 * пользователем.
 * @param text: новое содержимое поле.
 */
void MainWindow::read_value_from_line_edit(QString text)
{
    // Получаем индекс поля
    int i = sender()->property("index").toInt();
    if (i == 4)
    {
        // Длина хода переднего амортизатора
        if (text.isNull())
        {
            // Нулевое значение
            selected_chassis->set("min_travel", 0);
            selected_chassis->set("max_travel", 0);
        }
        else
        {
            // Ненулевое значение
            QStringList s = text.split("-");
            if (s.size() == 1)
            {
                selected_chassis->set("min_travel", s[0]);
                selected_chassis->set("max_travel", s[0]);
            }
            else
            {
                selected_chassis->set("min_travel", s[0]);
                selected_chassis->set("max_travel", s[1]);
            }
        }
    }
    else
        selected_chassis->set(i, text);
    // Проверяем, введены ли значения всех необходимых полей
    if (selected_chassis->is_empty())
        btn->setEnabled(false);
    else
        btn->setEnabled(true);
}

/**
 * Слот задает новую таблицу из базы данных для редактирования.
 * @param index: новый индекс таблицы из выпадающего списка.
 */
void MainWindow::set_new_tbl(int index)
{
    tbl_index = index;
    if (index > 0)
        btn->setEnabled(true);
    else
        btn->setEnabled(false);
}

/**
 * Слот задает новое значение для комплектующего, выбранного
 * пользователем.
 * @param index: новый индекс из выпадающего списка, выбранный
 * пользователем.
 */
void MainWindow::set_new_value(int index)
{
    // Определяем, какое поле было изменено и добавляем во
    // временное хранение в корзину
    QString name = sender()->objectName();
    if (index - 1 > -1)
    {
        // Во временную корзину добавляется компонент
        Accessory a;
        int i = index - 1;
        if (name == FRONT_ABSORBER_RUS)
            a = front_absorbers[i];
        else if (name == FRONT_ROTOR_RUS)
            a = front_rotors[i];
        else if (name == REAR_ABSORBER_RUS)
            a = rear_absorbers[i];
        else if (name == REAR_ROTOR_RUS)
            a = rear_rotors[i];
        else if (name == WHEEL_RUS)
            a = wheels[i];
        basket.add_to_temp(a);
    }
    else
        // Из временной корзины удаляется компонент
        basket.remove_from_temp(name);
}

/**
 * Метод отображает окно для выбора комплектующих для рамы.
 */
void MainWindow::show_accessories_wnd()
{
    // Определяем выбранную раму
    QString type = sender()->property("type").toString();
    if (type == "catalog")
    {
        // Пользователь выбрал раму из каталога. Добавляем
        // раму в корзину
        int i = sender()->property("index").toInt();
        basket.add(chassis[i]);
        selected_chassis->set(chassis.at(i));
    }
    // Получаем из базы данных задние амортизаторы
    rear_absorbers.clear();
    db->get_rear_absorbers(selected_chassis, rear_absorbers);
    // Получаем из базы данных передние амортизаторы
    front_absorbers.clear();
    db->get_front_absorbers(selected_chassis, front_absorbers);
    // Получаем из базы данных колеса
    wheels.clear();
    db->get_wheels(selected_chassis, wheels);
    // Получаем из базы данных передние роторы
    front_rotors.clear();
    db->get_front_rotors(selected_chassis, front_rotors);
    // Получаем из базы данных задние роторы
    rear_rotors.clear();
    db->get_rear_rotors(selected_chassis, rear_rotors);
    // Создаем окно
    stacked_layout->addWidget(create_accessories_wnd());
    int i = stacked_layout->currentIndex();
    stacked_layout->setCurrentIndex(i + 1);
}

/**
 * Метод отображает окно с рамами из каталога для выбранного
 * пользователем стиля катания.
 */
void MainWindow::show_chassis_for_style_wnd()
{
    // Определяем выбранный стиль катания
    QString style = sender()->property("style").toString();
    // Получаем из базы данных все рамы для выбранного стиля
    chassis.clear();
    db->get_chassis_for_style(style, chassis);
    // Создаем окно с рамами выбранного стиля и показываем его
    stacked_layout->addWidget(create_chassis_for_style_wnd(chassis));
    int i = stacked_layout->currentIndex();
    stacked_layout->setCurrentIndex(i + 1);
}

/**
 * Метод отображает исходное окно.
 */
void MainWindow::show_initial_wnd()
{
    // Очищаем корзину
    basket.clear();
    int i = stacked_layout->currentIndex();
    while (i > 0)
    {
        QWidget* widget = stacked_layout->widget(i);
        stacked_layout->removeWidget(widget);
        i--;
    }
    stacked_layout->setCurrentIndex(0);
}

/**
 * Метод отображает предыдущее окно.
 */
void MainWindow::show_previous_wnd()
{
    int i = stacked_layout->currentIndex();
    if (i > 0)
    {
        QWidget *widget = stacked_layout->currentWidget();
        stacked_layout->removeWidget(widget);
        stacked_layout->setCurrentIndex(i - 1);
    }
}

/**
 * Метод отображает окно с выбранными комплектующими.
 */
void MainWindow::show_selected_accessories_wnd()
{
    // Переводим комплектующие из временного списка в корзине
    // в саму корзину
    basket.move();
    // Создаем окно, на котором будут показаны выбранные
    // комплектующие
    stacked_layout->addWidget(create_selected_accessories_wnd());
    int i = stacked_layout->currentIndex();
    stacked_layout->setCurrentIndex(i + 1);
}

/**
 * Метод отображает окно с выбранной таблицей из базы данных.
 */
void MainWindow::show_selected_tbl_wnd()
{
    // Создаем окно, на котором будет показана выбранная таблица
    stacked_layout->addWidget(create_selected_tbl_wnd());
    int i = stacked_layout->currentIndex();
    stacked_layout->setCurrentIndex(i + 1);
}

/**
 * Метод отображает окно для выбора стиля катания.
 */
void MainWindow::show_style_wnd()
{
    // Получаем из базы данных список стилей катания
    QList<QString> styles;
    db->get_riding_styles(styles);
    // Создаем окно для выбора стиля катания
    stacked_layout->addWidget(create_style_wnd(styles));
    int i = stacked_layout->currentIndex();
    stacked_layout->setCurrentIndex(i + 1);
}

/**
 * Метод отображает окно для ввода характеристик собственной
 * рамы пользователя.
 */
void MainWindow::show_user_chassis_wnd()
{
    selected_chassis->init();
    // Создаем окно для ввода характеристик собственной
    // рамы пользователя
    stacked_layout->addWidget(create_user_chassis_wnd());
    int i = stacked_layout->currentIndex();
    stacked_layout->setCurrentIndex(i + 1);
}

/**
 * Слот для переключения режима работы в режим администратора.
 */
void MainWindow::switch_to_admin()
{
    if (work_mode == ADMIN_MODE)
        // Приложение уже работает в режиме администратора
        return;
    work_mode = ADMIN_MODE;
    // Удаляем все макеты окна для режима пользователя
    int i = stacked_layout->currentIndex();
    while (i >= 0)
    {
        QWidget* widget = stacked_layout->widget(i);
        stacked_layout->removeWidget(widget);
        i--;
    }
    // Создаем окно для выбора таблицы
    stacked_layout->addWidget(create_selection_for_tbls_wnd());
    stacked_layout->setCurrentIndex(0);
}

/**
 * Слот для переключения режима работы в режим пользователя.
 */
void MainWindow::switch_to_user()
{
    if (work_mode == USER_MODE)
        // Приложение уже работает в режиме пользователя
        return;
    work_mode = USER_MODE;
    // Удаляем все макеты окна для режима администратора
    int i = stacked_layout->currentIndex();
    while (i >= 0)
    {
        QWidget* widget = stacked_layout->widget(i);
        stacked_layout->removeWidget(widget);
        i--;
    }
    // Создаем исходное окно для режима пользователя
    stacked_layout->addWidget(create_initial_wnd());
    stacked_layout->setCurrentIndex(0);
}

/**
 * Метод завершает работу приложения, когда нет соединения
 * с базой данных.
 */
void MainWindow::quit()
{
    QMessageBox msg_box;
    msg_box.setText("Нет связи с базой данных. Попробуйте "
                    "перезапустить приложение");
    msg_box.exec();
    QApplication::quit();
    QApplication::exit();
}
