#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "timetable.h"
#include "timetable_io.h"

int main(int argc, char **argv)
{
	TabPtr tab, /* указатель на расписание */
		tmp_tab; /* временный указатель на расписание */
	/* Пробуем загрузить файл переданый аргументом коммандной строки
	 * или файл по умолчанию */
	if(argc > 1) tab = read_table(make_file_name(argv[1]));
	else tab = read_table("timetable.dat");
	/* если неудалось, то создаем пустое расписание*/
	if(tab == NULL) {
		printf("\nОшибка при загрузке файла.");
		tab = new_timetable(0);
	}
	char choise; /* переменная выбора меню */
	const char *file_name; /* имя файла */
	/* основной цыкл программы */
	do {
		show_menu();
		scanf(" %c", &choise);
		switch(choise) {
			/* загрузка расписания из файла */
			case LOAD:
				clear_scr();
				printf("\nВведите имя загружаемого файла: ");
				file_name = make_file_name(get_file_name());
				tmp_tab = read_table(file_name);
				if(tmp_tab) {
					printf("\nФайл %s загружен.\n", file_name);
					free_timetable(tab);
					tab = tmp_tab;
					show_table(tab, stdout);
				} else {
					printf("\nОшибка при загрузке файла: %s.\n", file_name);
				}
				press_next();
				clear_scr();
				break;
			/* сохранение расписания в файл */
			case SAVE:
				clear_scr();
				if((tab == NULL) || (tab->rows == NULL) || (tab->count == 0)){
					printf("\nРасписание отсутствует или пустое");
				}
				printf("\nВведите имя файла для сохранения: ");
				file_name = make_file_name(get_file_name());
				if(write_table(file_name, tab)) {
					printf("\nОшибка при сохранении файла: %s.\n", file_name);
				} else {
					printf("\nФайл %s сохранен.\n", file_name);
					show_table(tab, stdout);
				}
				press_next();
				clear_scr();
				break;
			/* вывод расписания на экран и в файл */
			case SHOW:
				clear_scr();
				if(
					tab == NULL
					|| tab->rows == NULL
					|| tab->count == 0)
				{
					printf("\nРасписание отсутствует или пустое\n");
				} else {
					printf("\nВывод расписания: ");
					show_table(tab, stdout);
					FILE *f = fopen("timetable.txt", "w");
					if(f == NULL) {
						perror("\nОшибка открытия файла");
						return 1;
					} else {
						show_table(tab, f);
						fclose(f);
					}
				}
				press_next();
				clear_scr();
				break;
			/* добавление записи в расписание */
			case APPEND:
				clear_scr();
				if(tab == NULL) {
					tab = new_timetable(0);
				}
				tmp_tab = append_row(tab);
				if(tmp_tab) {
					printf("\n.");
					free_timetable(tab);
					tab = tmp_tab;
					printf("\nЗапись добавлена в расписание.\n");
					show_table(tab, stdout);
				} else {
					printf("\nОшибка добавления записи.\n");
				}
				press_next();
				clear_scr();
				break;
			/* фильтрация расписания по пункту назначения
			 * и вывод на экран и в файл */
			case FILTER:
				clear_scr();
				if(tab != NULL) {
					printf("\nВведите пункт назначения: ");
					char *dst = get_destination();
					tmp_tab = filter_table(tab, dst);
					if(tmp_tab && tmp_tab->count) {
						show_table(tmp_tab, stdout);
						char name[strlen(dst) + 5];
						strcpy(name, dst);
						strcat(name, ".txt");
						errno = 0;
						FILE *f = fopen(name, "w");
						if(f == NULL) {
							perror("\nОшибка открытия файла");
							return 1;
						} else {
							show_table(tmp_tab, f);
							fclose(f);
						}
						free_timetable(tmp_tab);
					} else {
						printf("\nСовпадения отсутствуют.\n");
					}
				} else {
					printf("\nРасписание отсутствует.\n");
				}
				press_next();
				clear_scr();
				break;
			/* выход из программы */
			case EXIT:
				printf("\nРабота завершена.\n");
				break;
			/* обработка неправильного выбора */
			default :
				printf("\
					\nДействие не определено: key: %c; char: %i\
					\nПпопробуйте еше раз.\n", choise, choise);
		}
	} while(choise != EXIT);
	return 0;
}
