#include "Functions.h"
List* head_ptr = NULL;


// Input related functions //

// Prints out the options for a user.
void menu (void){
    printf("Menu:\n");
    printf("1. Display board\n2. Load board from a file\n");
    printf("3. Edit list\n4. Edit Board\n5. Save board to a file\n6. Quit\n");
    printf("Enter your choice (1 - 6): ");
    return;
}

// This function deals with taking the inputs.
// It returns 6, the stopping case only when quit is pressed, otherwise it just returns 0, which does nothing, keeping the while
// loop in main continuing
int takeInputMenu(void){
    int input = 0;
    int input2 = 0;
    scanf("%d", &input);
    char filename[25];
    char list_name[20];

    while (input < 1 || input > 6){
        printf("Invalid Input, it must be an integer (1 - 6)\n");
        printf("Enter your choice (1 - 6): ");
        scanf("%d", &input);
    }

    switch (input){
        case 1:
            DisplayBoard(head_ptr);
            break;
        case 2:
            printf("Enter file name: ");
            while ((getchar()) != '\n'); // to clear any \ns that may break the fgets
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;  // Remove trailing newline
            loadBoardFromFile(&head_ptr, filename);
            break;
        case 3:
            edit_list(head_ptr);
            break;
        case 4:
            printf("Options:\n");
            printf("1. Edit the name of a list\n2. Add a new list\n3. Delete a list\n4. Return to main menu\n");
            printf("Enter your option: ");
            scanf("%d", &input2);
            while (input2 < 1 || input2 > 4){
                printf("Invalid Input, it must be an integer (1 - 4)\n");
                printf("Enter your option (1 - 4): ");
                scanf("%d", &input2);
            }
            if(input2 == 4){
                break;
            }
            edit_board(&head_ptr, input2);
            break;
        case 5:
            printf("Enter filename: ");
            scanf("%24s", filename);
            saveBoardToFile(head_ptr, filename);
            break;
        case 6:
            return 6;
            break;
        default:
            printf("Error taking input\n");
            break;
    }
    
    return 0;
}




// This function takes in a double pointer head so that we can edit it.
// It also takes in the name of the new list.
// It appends the new list to the end of the list of names
void addList(List **head, const char *name) {
    List *newList = malloc(sizeof(List));
    strcpy(newList->name, name);
    newList->item = NULL; // initialising the values.
    newList->nextList = NULL;

    if (*head == NULL) { // If the list is empty, make the new list node the head of the board
        *head = newList;
    } else { // Else add the new list at the end.
        List *current = *head;
        while (current->nextList != NULL){
            current = current->nextList;
        }
        current->nextList = newList;
    }
}

// This function is similar to addList but it doesn't need the double pointer
// it uses strcmp to find the list then it appends the item to the end of the list of items.
void addItem(List *head, const char *listName, const char *itemName) {
    while (head != NULL) {
        if (strcmp(head->name, listName) == 0) {
            Item *newItem = malloc(sizeof(Item));
            strcpy(newItem->name, itemName);
            newItem->nextItem = NULL;

            if (head->item == NULL) {// If it is the first item added to the list
                head->item = newItem;
            } else { // If it isn't, shuffle until the curr->nextItem is free
                Item *current = head->item;
                while (current->nextItem != NULL){
                    current = current->nextItem;
                }
                current->nextItem = newItem; // Then store it there.
            }
            return;
        } // if the head isn't the list name needed then go to the next name.
        head = head->nextList;
    }
    printf("List name does not exist.\n");
    return;
}

void freeAllLists(List *head) { //This is used to delete all the lists
    while (head != NULL) {
        List *tempList = head;
        head = head->nextList;

        // Free all items in the list
        Item *item = tempList->item;
        while (item != NULL) {
            Item *tempItem = item;
            item = item->nextItem;
            free(tempItem);
        }

        // Free the list node itself
        free(tempList);
    }
}


// This function is what we use to save the board to the file. It takes in the top of the list,
// which is the first name in the board
// It also takes in the name of the file as a string
void saveBoardToFile(List *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file\n");
        return;
    }
    // The items in the linked list continue to be printed until there is no more list names.
    while (head != NULL) {
        fprintf(file, "%s:\n", head->name);
        Item *item = head->item;
        while (item != NULL) {
            fprintf(file, "\t%s\n", item->name);
            item = item->nextItem;
        }
        fprintf(file, "\n");
        head = head->nextList;
    }

    fclose(file);
    printf("Board has been saved.\n");
    return;
}

// This function loads the board from our given file.
// It takes in a double pointer head so that we can modify this if necessary and the name of the file
void loadBoardFromFile(List **head, const char *filename) {
    // opening file
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return;
    }
    
    if (*head != NULL) {
        freeAllLists(*head); // This ensures we're not just appending the list onto the already existing lists
        *head = NULL;
    }

    // this line array is used to read each line, as we are seperating list names with 'name':
    // and items with a tab
    /*eg.
    Vishal:
        Iphone
        Charger

    Carl:
        Airpods
        Powerbank
    
    */

    char line[100];
    List *currentList = NULL;

    // fgets reads a line at a time from the file into line
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n') continue;  // Uesd to skip empty lines

        if (line[0] != '\t') { // If the line doesn't begin with a tab or an empty line then it should be a list name.
            line[strcspn(line, ":\n")] = 0; // remove ':' and newline when storing the line
            addList(head, line); // calls the function to create a node with the new value.
            // move currentList pointer to the newly added list
            currentList = *head;
            while (currentList->nextList != NULL) // moves to the next available position in the list where currentList->nextList is empty.
                currentList = currentList->nextList;
        } else {
            // if it's an item
            char *itemName = line + 1; // pointer that skips tab
            itemName[strcspn(itemName, "\n")] = 0; // strip newline
            addItem(currentList, currentList->name, itemName); //adds the task to the current column.
        }
    }

    fclose(file);
    printf("\nBoard loaded succesfully.\n");
    DisplayBoard(*head);
    return;
    
}


// Function to display the board
void DisplayBoard(List *head) {
    if (head == NULL) {
        printf("Board is empty or not initialized\n");
        return;
    }
    
    printf("\n--- Board ---\n");
    
    List *currentList = head;
    while (currentList != NULL) {
        printf("%s:", currentList->name);
        
        // Display all items in the current list
        Item *currentItem = currentList->item;
        while (currentItem != NULL) {
            printf("\n\t%s", currentItem->name);
            currentItem = currentItem->nextItem;
        }
        printf("\n");
        
        currentList = currentList->nextList;
    }
    printf("\n");
}

// Function to edit an item within a list
void editItem(List *head, const char *listName, const char *itemName, const char *newName) {
    List *currentList = head;
    // Find the correct list
    while (currentList != NULL) {
        if (strcmp(currentList->name, listName) == 0) {
            Item *currentItem = currentList->item;
            // Find the item to edit
            while (currentItem != NULL) {
                if (strcmp(currentItem->name, itemName) == 0) {
                    // Update the item name
                    strcpy(currentItem->name, newName);
                    printf("Item '%s' updated to '%s'.\n\n", itemName, newName);
                    return;
                }
                currentItem = currentItem->nextItem;
            }
            printf("Item '%s' not found in list '%s'.\n\n", itemName, listName);
            return;
        }
        currentList = currentList->nextList;
    }
    printf("List '%s' not found.\n\n", listName);
}

// Function to delete an item from a list
void deleteItem(List *head, const char *listName, const char *itemName) {
    List *currentList = head;
    // Find the correct list
    while (currentList != NULL) {
        if (strcmp(currentList->name, listName) == 0) {
            Item *currentItem = currentList->item;
            Item *prevItem = NULL;
            
            // Handle case where the item to be deleted is the first item
            if (currentItem != NULL && strcmp(currentItem->name, itemName) == 0) {
                currentList->item = currentItem->nextItem;
                free(currentItem);
                printf("Item '%s' deleted.\n\n", itemName);
                return;
            }
            
            // Navigate through list to find the item
            while (currentItem != NULL && strcmp(currentItem->name, itemName) != 0) {
                prevItem = currentItem;
                currentItem = currentItem->nextItem;
            }
            
            // Check if item was found
            if (currentItem == NULL) {
                printf("Item '%s' not found in list '%s'.\n\n", itemName, listName);
                return;
            }
            
            // Remove the item from the list
            prevItem->nextItem = currentItem->nextItem;
            free(currentItem);
            printf("Item '%s' deleted.\n", itemName);
            return;
        }
        currentList = currentList->nextList;
    }
    printf("List '%s' not found.\n\n", listName);
}

// Function to edit the name of a list
void editListName(List *head, const char *listName, const char *newName) {
    List *currentList = head;
    // Find the list to edit
    while (currentList != NULL) {
        if (strcmp(currentList->name, listName) == 0) {
            // Update the list name
            strcpy(currentList->name, newName);
            printf("List '%s' updated to '%s'.\n\n", listName, newName);
            return;
        }
        currentList = currentList->nextList;
    }
    printf("List '%s' not found.\n", listName);
}

// Function to delete a list from the board
void deleteList(List **head, const char *listName) {
    if (*head == NULL) {
        printf("Board is empty.\n\n");
        return;
    }
    
    List *currentList = *head;
    List *prevList = NULL;
    
    // Handle case where the list to be deleted is the first list
    if (strcmp(currentList->name, listName) == 0) {
        *head = currentList->nextList;
        
        // Free all items in the list
        Item *currentItem = currentList->item;
        while (currentItem != NULL) {
            Item *tempItem = currentItem;
            currentItem = currentItem->nextItem;
            free(tempItem);
        }
        
        free(currentList);
        printf("List '%s' deleted.\n\n", listName);
        return;
    }
    
    // Navigate through board to find the list
    while (currentList != NULL && strcmp(currentList->name, listName) != 0) {
        prevList = currentList;
        currentList = currentList->nextList;
    }
    
    // Check if list was found
    if (currentList == NULL) {
        printf("List '%s' not found.\n\n", listName);
        return;
    }
    
    // Remove the list from the board
    prevList->nextList = currentList->nextList;
    
    // Free all items in the list
    Item *currentItem = currentList->item;
    while (currentItem != NULL) {
        Item *tempItem = currentItem;
        currentItem = currentItem->nextItem;
        free(tempItem);
    }
    
    free(currentList);
    printf("List '%s' deleted.\n\n", listName);
}

// Function to handle the edit list menu
void edit_list(List *head) {
    char listName[100];
    char itemName[100];
    char newName[100];
    int option;
    
    printf("Enter the name of the list to edit: ");
    scanf(" %99[^\n]", listName);
    
    // Check if the list exists
    List *currentList = head;
    bool listFound = false;
    
    while (currentList != NULL) {
        if (strcmp(currentList->name, listName) == 0) {
            listFound = true;
            break;
        }
        currentList = currentList->nextList;
    }
    
    if (!listFound) {
        printf("List '%s not found\n\n", listName);
        return;
    }
    
    do {
        printf("Options:\n");
        printf("1. Edit an item\n");
        printf("2. Add a new item\n");
        printf("3. Delete an item\n");
        printf("4. Return to main menu\n");
        printf("Enter your option: ");
        scanf("%d", &option);
        
        switch (option) {
            case 1: // Edit an item
                printf("Enter the name of the item to edit: ");
                scanf(" %[^\n]", itemName);
                printf("Enter new name for item '%s': ", itemName);
                scanf(" %[^\n]", newName);
                editItem(head, listName, itemName, newName);
                break;
                
            case 2: // Add a new item
                printf("Enter the name of the new item: ");
                scanf(" %[^\n]", newName);
                addItem(head, listName, newName);
                break;
                
            case 3: // Delete an item
                printf("Enter the name of the item to delete: ");
                scanf(" %[^\n]", itemName);
                deleteItem(head, listName, itemName);
                break;
                
            case 4: // Return to main menu
                printf("Returning to main menu.\n");
                break;
                
            default:
                printf("Invalid option. Please try again.\n");
        }
    } while (option != 4);
}

// Function to handle the edit board menu
void edit_board(List **head, int option) {
    char listName[100];
    char newName[100];
    
    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    switch (option) {
        case 1: // Edit the name of a list
            printf("Enter the name of the list to edit: ");
            fgets(listName, sizeof(listName), stdin);
            listName[strcspn(listName, "\n")] = 0; // Remove newline
            
            //check if list exists before asking for new name
            List *currentList = *head;
            bool listFound= false;


            while (currentList != NULL) {
                if (strcmp(currentList->name, listName) == 0){
                    listFound = true;
                    break;
                }
                currentList = currentList->nextList;
            }

            if(!listFound){
                printf("List %s not found.\n\n", listName);
                break;
            }

            printf("Enter new name for list '%s': ", listName);
            fgets(newName, sizeof(newName), stdin);
            newName[strcspn(newName, "\n")] = 0;
            
            editListName(*head, listName, newName);
            break;
            
        case 2: // Add a new list
            printf("Enter the name of the new list: ");
            fgets(newName, sizeof(newName), stdin);
            newName[strcspn(newName, "\n")] = 0;
            
            addList(head, newName);
            printf("List '%s' added.\n\n", newName);
            break;
            
        case 3: // Delete a list
            printf("Enter the name of the list to delete: ");
            fgets(listName, sizeof(listName), stdin);
            listName[strcspn(listName, "\n")] = 0;
            
            deleteList(head, listName);
            break;
            
        default:
            printf("Invalid option. Please try again.\n\n");
    }
    }
