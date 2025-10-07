#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User {
    int id;
    char name[50];
    int age;
};

void addUser() {
    FILE *f = fopen("users.txt", "a");
    if (f == NULL) {
        printf("Error opening file!\n");
        return;
    }

    struct User u;
    printf("Enter ID: ");
    scanf("%d", &u.id);
    printf("Enter Name: ");
    scanf("%s", u.name);
    printf("Enter Age: ");
    scanf("%d", &u.age);

    fprintf(f, "%d %s %d\n", u.id, u.name, u.age);
    fclose(f);
    printf("User added successfully.\n");
}

void showUsers() {
    FILE *f = fopen("users.txt", "r");
    if (f == NULL) {
        printf("No users found.\n");
        return;
    }

    struct User u;
    printf("\n User Records \n");
    printf("ID\tName\tAge\n");
    while (fscanf(f, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        printf("%d\t%s\t%d\n", u.id, u.name, u.age);
    }
    fclose(f);
}

void updateUser(int id) {
    FILE *f = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (f == NULL || temp == NULL) {
        printf("File error!\n");
        return;
    }

    struct User u;
    int found = 0;

    while (fscanf(f, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        if (u.id == id) {
            found = 1;
            printf("Enter new Name: ");
            scanf("%s", u.name);
            printf("Enter new Age: ");
            scanf("%d", &u.age);
        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }

    fclose(f);
    fclose(temp);
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("User updated successfully.\n");
    else
        printf("User with ID %d not found.\n", id);
}

void deleteUser(int id) {
    FILE *f = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (f == NULL || temp == NULL) {
        printf("File error!\n");
        return;
    }

    struct User u;
    int found = 0;

    while (fscanf(f, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        if (u.id == id) {
            found = 1; // skip this user (deleting)
            continue;
        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }

    fclose(f);
    fclose(temp);
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("User deleted successfully.\n");
    else
        printf("User with ID %d not found.\n", id);
}

int main() {
    int choice, id;

    while (1) {
        printf("\n1. Add User\n");
        printf("2. Show Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: 
                addUser();
                break;
            case 2: 
                showUsers();
                break;
            case 3:
                printf("Enter ID to update: ");
                scanf("%d", &id);
                updateUser(id);
                break;
            case 4:
                printf("Enter ID to delete: ");
                scanf("%d", &id);
                deleteUser(id);
                break;
            case 5: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
