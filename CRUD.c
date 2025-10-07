#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USER_FILE "users.txt"
#define TEMP_FILE "temp_users.txt"

struct User {
    int id;
    char name[50];
    int age;
};

int userExists(int userId) {
    FILE *userFile = fopen(USER_FILE, "r");
    if (userFile == NULL) return 0;

    struct User user;
    while (fscanf(userFile, "%d %49s %d", &user.id, user.name, &user.age) == 3) {
        if (user.id == userId) {
            fclose(userFile);
            return 1;
        }
    }
    fclose(userFile);
    return 0;
}

void addUser() {
    FILE *userFile = fopen(USER_FILE, "a");
    if (userFile == NULL) {
        printf("Error opening user file.\n");
        return;
    }

    struct User newUser;
    printf("Enter ID: ");
    scanf("%d", &newUser.id);

    if (userExists(newUser.id)) {
        printf("Error: A user with this ID already exists.\n");
        fclose(userFile);
        return;
    }

    printf("Enter Name: ");
    scanf("%49s", newUser.name);
    printf("Enter Age: ");
    scanf("%d", &newUser.age);

    fwrite(&newUser, sizeof(struct User), 1, userFile);
    fclose(userFile);
    printf("User added successfully.\n");
}

void displayUsers() {
    FILE *userFile = fopen(USER_FILE, "r");
    if (userFile == NULL) {
        printf("No users found.\n");
        return;
    }

    struct User user;
    printf("\n--- User List ---\n");
    while (fread(&user, sizeof(struct User), 1, userFile)) {
        printf("ID: %d | Name: %s | Age: %d\n", user.id, user.name, user.age);
    }
    fclose(userFile);
}

void updateUser(int userId) {
    FILE *userFile = fopen(USER_FILE, "r");
    FILE *tempFile = fopen(TEMP_FILE, "w");
    if (userFile == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct User user;
    int found = 0;
    while (fread(&user, sizeof(struct User), 1, userFile)) {
        if (user.id == userId) {
            found = 1;
            printf("Enter new Name: ");
            scanf("%49s", user.name);
            printf("Enter new Age: ");
            scanf("%d", &user.age);
        }
        fwrite(&user, sizeof(struct User), 1, tempFile);
    }

    fclose(userFile);
    fclose(tempFile);
    remove(USER_FILE);
    rename(TEMP_FILE, USER_FILE);

    if (found)
        printf("User updated successfully.\n");
    else
        printf("User with ID %d not found.\n", userId);
}

void deleteUser(int userId) {
    FILE *userFile = fopen(USER_FILE, "r");
    FILE *tempFile = fopen(TEMP_FILE, "w");
    if (userFile == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct User user;
    int found = 0;

    while (fread(&user, sizeof(struct User), 1, userFile)) {
        if (user.id != userId) {
            fwrite(&user, sizeof(struct User), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(userFile);
    fclose(tempFile);
    remove(USER_FILE);
    rename(TEMP_FILE, USER_FILE);

    if (found)
        printf("User deleted successfully.\n");
    else
        printf("User with ID %d not found.\n", userId);
}

int main() {
    int choice, id;
    while (1) {
        printf("\n--- User Management System ---\n");
        printf("1. Add User\n2. Display Users\n3. Update User\n4. Delete User\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addUser();
                break;
            case 2:
                displayUsers();
                break;
            case 3:
                printf("Enter User ID to update: ");
                scanf("%d", &id);
                updateUser(id);
                break;
            case 4:
                printf("Enter User ID to delete: ");
                scanf("%d", &id);
                deleteUser(id);
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}
