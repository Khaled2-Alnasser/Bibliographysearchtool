#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 100

typedef struct {
    char type[30];
    char id[100];
    char author[300];
    char title[300];
    char journal[200];
    char year[10];
} BibEntry;

int loadEntries(BibEntry entries[]) {
    FILE *file = fopen("resitbib.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open resitbib.txt\n");
        return 0;
    }

    char line[500];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "@") != NULL) {
            sscanf(line, "@%[^'{']{%[^,]", entries[count].type, entries[count].id);
        } else if (strstr(line, "author") != NULL) {
            sscanf(line, "   author = {%[^}]", entries[count].author);
        } else if (strstr(line, "title") != NULL) {
            sscanf(line, "   title = {%[^}]", entries[count].title);
        } else if (strstr(line, "journal") != NULL) {
            sscanf(line, "   journal = {%[^}]", entries[count].journal);
        } else if (strstr(line, "year") != NULL) {
            sscanf(line, "   year = {%[^}]", entries[count].year);
            count++; // finish entry
        }
    }

    fclose(file);
    return count;
}

void searchByAuthor(BibEntry entries[], int count) {
    char search[100];
    printf("Enter author name to search: ");
    scanf(" %[^\n]", search);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].author, search)) {
            printf("\n--- Entry Found ---\n");
            printf("Author: %s\n", entries[i].author);
            printf("Title: %s\n", entries[i].title);
            printf("Journal: %s\n", entries[i].journal);
            printf("Year: %s\n", entries[i].year);
            found = 1;
        }
    }
    if (!found) {
        printf("No entry found for author '%s'\n", search);
    }
}

void printHarvardStyle(BibEntry entries[], int count) {
    char search[100];
    printf("Enter entry ID (e.g., Henri2017): ");
    scanf(" %[^\n]", search);
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].id, search) == 0) {
            printf("\n--- UWE Harvard Style ---\n");
            printf("%s (%s) %s. *%s*.\n",
                entries[i].author,
                entries[i].year,
                entries[i].title,
                entries[i].journal);
            return;
        }
    }
    printf("Entry ID not found.\n");
}

int main() {
    BibEntry entries[MAX_ENTRIES];
    int count = loadEntries(entries);
    if (count == 0) return 1;

    int choice;
    do {
        printf("\n==== Bibliography Tool ====\n");
        printf("1. Search by Author\n");
        printf("2. Display Entry (Harvard Style)\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                searchByAuthor(entries, count);
                break;
            case 2:
                printHarvardStyle(entries, count);
                break;
            case 3:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid option.\n");
        }
    } while (choice != 3);

    return 0;
}
