#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "timetable.h"

/**********************************************************************
 * Выделяет память для массива структур записей о рейсах
 * и возвращает указатель на начало массива
 **********************************************************************/
Rows new_rows(size_t count) {
	errno = 0;
	Rows rows = (Rows)malloc(sizeof(Row) * count);
	if(rows != NULL) {
		return rows;
	}
	perror("Память для Rows не выделена");
	return NULL;
}

/**********************************************************************
 * Создает структуру расписания нужного размера
 * и возвращает указатель на нее
 **********************************************************************/
TabPtr new_timetable(size_t count) {
	errno = 0;
	TabPtr tab = (TabPtr)malloc(sizeof(Timetable));
	if(tab != NULL) {
		if(count == 0) {
			tab->count = 0;
			tab->rows = NULL;
			return tab;
		}
		tab->rows = new_rows(count);
		if(tab->rows != NULL) {
			tab->count = count;
			return tab;
		}
	}
	perror("Память для Timetable не выделена");
	return NULL;
}

/**********************************************************************
 * Освобождает память занятую расписанием
 **********************************************************************/
void free_timetable(TabPtr tab) {
	free(tab->rows);
	free(tab);
}

/**********************************************************************
 * Создает структуру расписания нужного размера, копирует в нее массив
 * структур записей о рейсах и возвращает указатель на нее
 **********************************************************************/
TabPtr make_timetable(const Rows rows, size_t count){
	TabPtr tab = new_timetable(count);
	if(tab != NULL) {
		tab->count = count;
		memcpy(tab->rows, rows, sizeof(Row) * count);
		return tab;
	}
	return NULL;
}

/**********************************************************************
 * Копирует расписание и добавляет в него запись о рейсе
 **********************************************************************/
TabPtr add_row(const TabPtr tab, const RowPtr row) {
	TabPtr new_tab = new_timetable(tab->count + 1);
	if( new_tab != NULL) {
		memcpy(new_tab->rows, tab->rows, sizeof(Row) * tab->count);
		memcpy(new_tab->rows + tab->count, row, sizeof(Row));
		return new_tab;
	}
	return NULL;
}

/**********************************************************************
 * Фильтрует расписание по пункту назначения
 **********************************************************************/
TabPtr filter_table(const TabPtr tab, const char *destination) {
	TabPtr t = new_timetable(0);
	if(t != NULL) {
		for(size_t i = 0; i < tab->count; i++) {
			if(strcmp(tab->rows[i].destination, destination) == 0) {
				t->rows = realloc(t->rows, sizeof(Row) * (t->count + 1));
				memcpy(t->rows + t->count, &tab->rows[i], sizeof(Row));
				++t->count;
			}
		}
		return t;
	}
	return NULL;
}

