Kanban Board (C Console App)

This project is a simple console-based Kanban board implemented in C, using linked lists to manage columns (lists) and tasks (items).
Features

    Maintain a board of named lists (e.g. “Nick”, “Tim”, “To Do”, “Doing”, “Done”).

    Add, edit, and delete lists.

    Add, edit, and delete items within a list.

    Display the current board in a readable format.​

    Save the current board to a text file.​

    Load a board from a text file in the same format.​

On exit, the program frees all dynamically allocated memory and prints an ASCII-art Golden State Warriors logo.
Data Model

Two main structs are used:​

    Item: stores an item name and a pointer to the next item in that list.

    List: stores a list name, pointer to its first item, and pointer to the next list on the board.

The board is represented by a singly linked list of List nodes, and each List contains a singly linked list of Item nodes.
File Format

Boards are stored in a plain-text format:​

    Each list name is followed by a colon and newline, e.g. Nick:

    Items belonging to that list appear on subsequent lines, each starting with a tab.

    Lists are separated by a blank line.

Example:

text
Nick:
    3070 RTX

Tim:
    Oculus Quest 2

Menu Options

When you run the program, you interact via a numeric menu:​

    Display board – Prints all lists and their items.​

    Load board from a file – Prompts for a filename and loads that board, replacing any current data.​

    Edit list – Work within a single list: edit items, add items, or delete items.​

    Edit board – Edit list names, add new lists, or delete lists.​

    Save board to a file – Prompts for a filename and writes the current board.​

    Quit – Exits the program after freeing memory and printing ASCII art.

Building and Running

    Ensure you have a C compiler (e.g. gcc).

    Place all three files in the same directory: KanbanMain.c, Functions.c, Functions.h.

    Compile:

bash
gcc KanbanMain.c Functions.c -o kanban

    Run:

bash
./kanban

On first startup, the program initializes a default board with some example lists and items (Nick, Tim, Dante, Abey and their items).​
Key Functions (API Overview)

Defined in Functions.h:

    void menu(void); – Prints the main menu.

    int takeInputMenu(void); – Handles user input and triggers actions, returns 6 only when quitting.

    void DisplayBoard(List *head); – Prints the entire board.

    void loadBoardFromFile(List **head, const char *filename); – Loads a board from file.

    void saveBoardToFile(List *head, const char *filename); – Saves the board to file.

    void addList(List **head, const char *name); – Adds a new list to the board.

    void addItem(List *head, const char *listName, const char *itemName); – Adds an item to a list.

    void editItem(List *head, const char *listName, const char *itemName, const char *newName); – Renames an item.

    void deleteItem(List *head, const char *listName, const char *itemName); – Deletes an item from a list.

    void editListName(List *head, const char *listName, const char *newName); – Renames a list.

    void deleteList(List **head, const char *listName); – Deletes an entire list.

    void freeAllLists(List *head); – Frees all lists and items.

    void edit_list(List *head); – Handles the “Edit list” submenu.

    void edit_board(List **head, int option); – Handles the “Edit board” submenu logic.

Notes

    Input is mostly line-based; the code uses fgets and scanf carefully to avoid newline issues.​

    When loading from a file, any existing board is cleared first to avoid mixing data.
