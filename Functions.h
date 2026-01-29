#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H


typedef struct Item{
    char name[20];
    struct item *nextItem;
}Item;

typedef struct List {
    char name[20];
    struct List *nextList;
    Item *item;
}List;

extern List* head_ptr;


void menu(void);
int takeInputMenu(void);

void freeAllLists(List *head);

void printBoard(List *head);
void DisplayBoard(List *head);

void loadBoardFromFile(List **head, const char *filename);
void saveBoardToFile(List *head, const char *filename);

void addList(List **head, const char *name);
void addItem(List *head, const char *listName, const char *itemName);


void editItem(List *head, const char *listName, const char *itemName, const char *newName);
void deleteItem(List *head, const char *listName, const char *itemName);
void editListName(List *head, const char *listName, const char *newName);
void deleteList(List **head, const char *listName);
void edit_list(List *head);
void edit_board(List **head, int option);

#endif
