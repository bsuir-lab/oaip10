#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>
#include <errno.h>

#include "timetable.h"
#include "timetable_io.h"

/************** int get_int(int min, int max); ************************
 * Фильтр диапазонов чисел вводимых с клавиатуры
 **********************************************************************/
int get_int(int min, int max) {
	int digit;
	do {
		if(min > -10 && max < 10) scanf(" %1d", &digit);
		else if(min > -100 && max < 100) scanf(" %2d", &digit);
		else if(min > -1000 && max < 1000) scanf(" %3d", &digit);
		else if(min > -10000 && max < 10000) scanf(" %4d", &digit);
		else scanf(" %5d", &digit);
		if(digit < min || digit > max) {
			fprintf(
				stderr,
				"\nmin: %d; max: %d; cur:%d\
				\nФайл: %s. Строка: %u; Функция: %s\n",
				min, max, digit, __FILE__, __LINE__, __func__
			);
		}
	} while ((digit < min) || (digit > max));
	return digit;
}

/************** int get_number(); *************************************
 * Фильтр номера рейса
 **********************************************************************/
int get_number() {
	return get_int(MIN_NUMBER, MAX_NUMBER);
}

/************** int get_hours(); ***************************************
 * Фильтр часа рейса
 **********************************************************************/
int get_hours() {
	return get_int(MIN_HOURS, MAX_HOURS);
}

/************** int get_minutes(); ************************************
 * Фильтр минут рейса
 **********************************************************************/
int get_minutes() {
	return get_int(MIN_MINUTES, MAX_MINUTES);
}

/*************** char * get_destination(); ****************************
 * Ввод пункта назначения
 **********************************************************************/
char * get_destination(){
	errno = 0;
	char format[20];
	char * destination = (char *)malloc(sizeof(char) * DESTINATION_SIZE);
	if(destination == NULL) {
		perror("\nОшибка выделения памяти для пункта назначения");
	}
	sprintf(format, " %%%ds", DESTINATION_SIZE - 1);
	scanf(format, destination);
	return destination;
}

/*************** const char * get_file_name(); ***********************
 * Ввод имени файла
 **********************************************************************/
const char * get_file_name() {
	char *name = malloc(sizeof(char) * 255);
	scanf(" %254s", name);
	return name;
}

/*************** int check_file_name(const char *name); ***************
 * Проверка расширения .dat у имени файла расписания
 **********************************************************************/
int check_file_name(const char *name) {
	int pos = strlen(name) - 4;
	if(pos < 0 || strcmp(&name[pos], ".dat")) {
		return 0;
	}
	return 1;
}

/*************** const char * make_file_name(const char *name); ********
 * Добавление расширения .dat к имени файла, если нужно
 **********************************************************************/
const char * make_file_name(const char *name) {
	errno = 0;
	if(check_file_name(name)){
		return name;
	}
	char * new_name;
	new_name = (char *) malloc(sizeof(char) * (strlen(name) + 5));
	if(new_name != NULL) {
		return strcat(strcpy(new_name, name), ".dat");
	}
	perror("\nПамять для имени файла не выделена");
	return NULL;
}

/*************** int file_exists(const char *name); *******************
 * Проверка существования файла
 **********************************************************************/
int file_exists(const char *name) {
	glob_t globbuf;
	globbuf.gl_offs = 2;
	glob(name, GLOB_DOOFFS, NULL, &globbuf);
	return globbuf.gl_pathc;
}

/*************** TabPtr read_table(const char *file_name); ************
 * Чтение файла расписания
 **********************************************************************/
TabPtr read_table(const char *name) {
	errno = 0;
	if(!file_exists(name)) {
		perror("\nФайл отсутствует");
		return NULL;
	}
	size_t count;
	FILE *f = fopen(name, "rb");
	if(f == NULL) {
		perror("\nОшибка открытия файла");
		return NULL;
	}
	if(fread(&count, sizeof(count), 1, f) != 1) {
		perror("\nОшибка чтения файла");
		fclose(f);
		return NULL;
	}
	Rows rows = new_rows(count);
	if(rows == NULL){
		fclose(f);
		return NULL;
	}
	size_t n = fread(rows, sizeof(Row), count, f);
	if(n != count) {
		fprintf(stderr, "\ncount: %lu; n: %lu\n", count, n);
		fclose(f);
		return NULL;
	}
	fclose(f);
	TabPtr tab = make_timetable(rows, n);
	free(rows);
	return tab;
}

/****** int write_table(const char *file_name, const TabPtr tab); *****
 * Запись файла расписания
 **********************************************************************/
int write_table(const char *name, const TabPtr tab) {
	errno = 0;
	FILE *f = fopen(name, "wb");
	if(f == NULL) {
		perror("\nОшибка открытия файла");
		return 1;
	}
	if(fwrite(&tab->count, sizeof(tab->count), 1, f) != 1) {
		perror("\nОшибка записи в файл");
		fclose(f);
		return 2;
	}
	size_t n = fwrite(tab->rows, sizeof(Row), tab->count, f);
	if(n != tab->count) {
		fprintf(stderr, "\ncount: %lu; n: %lu\n", tab->count, n);
		fclose(f);
		return 3;
	}
	fclose(f);
	return 0;
}

/*************** TabPtr append_row(TabPtr tab); ***********************
 * Добавление рейса в расписание
 **********************************************************************/
TabPtr append_row(TabPtr tab) {
	Row row;
	printf("\nВведите номер рейса от %d до %d: ", MIN_NUMBER, MAX_NUMBER);
	row.number = get_number();
	printf("Введите час вылета: ");
	row.hours = get_hours();
	printf("Введите минуты вылета: ");
	row.minutes = get_minutes();
	printf("Введите пункт назначения: ");
	strcpy(row.destination, get_destination());
	return add_row(tab, &row);
}

/*************** void show_table(const TabPtr tab); *******************
 * Вывод расписания на экран или в файл
 **********************************************************************/
void show_table(const TabPtr tab, FILE *f) {
	fprintf(f, "\nКоличество рейсов: %lu", tab->count);
	fprintf(f, "\n#=====================================================");
	fprintf(f, "\n# Номер | Время | Пункт назначения");
	fprintf(f, "\n#-----------------------------------------------------");
	for (size_t i = 0; i < tab->count; ++i) {
		fprintf(f, "\n# %5d | %02d:%02d | %s",
			tab->rows[i].number,
			tab->rows[i].hours,
			tab->rows[i].minutes,
			tab->rows[i].destination
		);
	}
	fprintf(f, "\n#=====================================================\n");
}

/*************** void show_menu(); ************************************
 * Вывод меню на экран
 **********************************************************************/
void show_menu() {
	printf("\
		\n1 => загрузить расписание из файла: 'имя файла'.dat\
		\n2 => сохранить расписание в файл: 'имя файла'.dat\
		\n3 => вывести текущее расписание на экран и в файл timetable.txt\
		\n4 => добавить новый рейс в расписание\
		\n5 => фильтровать расписание по пункту назначения и вывести\
		\n     результаты на экран и в файл 'фильтр пункта назначения'.txt\
		\nq => закрыть программу\
		\n\
		\nВведите число для выбора действия или q для выхода и нажмите Enter: ");
}

/*************** char press_next(); ***********************************
 * Ожидание ввода
 **********************************************************************/
char press_next() {
	printf("\n\nВведите любую цифру или букву и нажмите Enter: ");
	char c;
	scanf(" %c", &c);
	return c;
}

/*************** void clear_scr(); ************************************
 * Очистка экрана
 **********************************************************************/
void clear_scr() {
	printf("\
		\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\
		\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\
		\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\
		\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\
		\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\
	");
}
