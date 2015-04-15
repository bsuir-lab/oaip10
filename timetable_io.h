#ifndef _TIMETABLE_IO_H
#define _TIMETABLE_IO_H 1

#include <stdio.h>
#include "timetable.h"

/* Коды символов */
#define KEY_ESC 27
#define KEY_SPACE 32
#define KEY_ENTER 10
#define KEY_ZERO 48
#define KEY_0 KEY_ZERO
#define KEY_1 1 + KEY_ZERO
#define KEY_2 2 + KEY_ZERO
#define KEY_3 3 + KEY_ZERO
#define KEY_4 4 + KEY_ZERO
#define KEY_5 5 + KEY_ZERO
#define KEY_6 6 + KEY_ZERO
#define KEY_7 7 + KEY_ZERO
#define KEY_8 8 + KEY_ZERO
#define KEY_9 9 + KEY_ZERO
#define KEY_q 113
/* Коды меню */
#define LOAD   KEY_1
#define SAVE   KEY_2
#define SHOW   KEY_3
#define APPEND KEY_4
#define FILTER KEY_5
#define EXIT   KEY_q

/************** int get_int(int min, int max); ************************
 * Фильтр диапазонов чисел вводимых с клавиатуры
 **********************************************************************/
int get_int(int min, int max);

/************** int get_number(); *************************************
 * Фильтр номера рейса
 **********************************************************************/
int get_number();

/************** int get_hours(); **************************************
 * Фильтр часа рейса
 **********************************************************************/
int get_hours();

/************** int get_minutes(); ************************************
 * Фильтр минут рейса
 **********************************************************************/
int get_minutes();

/*************** char * get_destination(); ****************************
 * Ввод пункта назначения
 **********************************************************************/
char * get_destination();

/*************** int check_file_name(const char *name); ***************
 * Проверка расширения .dat у имени файла расписания
 **********************************************************************/
int check_file_name(const char *name);

/*************** const char * make_file_name(const char *name); ********
 * Добавление расширения .dat к имени файла, если нужно
 **********************************************************************/
const char * make_file_name(const char *name);

/*************** int file_exists(const char *name); *******************
 * Проверка существования файла
 **********************************************************************/
int file_exists(const char *name);

/*************** TabPtr read_table(const char *file_name); ************
 * Чтение файла расписания
 **********************************************************************/
TabPtr read_table(const char *file_name);

/****** int write_table(const char *file_name, const TabPtr tab); *****
 * Запись файла расписания
 **********************************************************************/
int write_table(const char *file_name, const TabPtr tab);

/*************** TabPtr append_row(TabPtr tab); ***********************
 * Добавление рейса в расписание
 **********************************************************************/
TabPtr append_row(TabPtr tab);

/*************** void show_table(const TabPtr tab, FILE *f); *******************
 * Вывод расписания на экран или в файл
 **********************************************************************/
void show_table(const TabPtr tab, FILE *f);

/*************** void show_menu(); ************************************
 * Вывод меню на экран
 **********************************************************************/
void show_menu();

/*************** const char * get_file_name(); ***********************
 * Ввод имени файла
 **********************************************************************/
const char * get_file_name();

/*************** char press_next(); ***********************************
 * Ожидание ввода
 **********************************************************************/
char press_next();

/*************** void clear_scr(); ************************************
 * Очистка экрана
 **********************************************************************/
void clear_scr();

#endif /* timetable_io.h  */

