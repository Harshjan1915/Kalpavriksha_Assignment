#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_MAX 20
#define MIN_MARK 0.0f
#define MAX_MARK 100.0f
#define GRADE_A 85.0f
#define GRADE_B 70.0f
#define GRADE_C 50.0f
#define GRADE_D 35.0f

struct Student {
    int rollNumber;
    char name[NAME_MAX];
    float mark1;
    float mark2;
    float mark3;
    char grade;
    float total;
    float average;
};

int readStudents(struct Student *students, int studentcount) {
    for (int i = 0; i < studentcount; i++) {
        if (scanf("%d", &students[i].rollNumber) != 1){
             return -1;
        }
        if (scanf("%19s", students[i].name) != 1) {
            return -1;
        }
        if (scanf("%f %f %f", &students[i].mark1, &students[i].mark2, &students[i].mark3) != 3) {
            return -1;
        }

        if (students[i].mark1 < MIN_MARK || students[i].mark1 > MAX_MARK ||
            students[i].mark2 < MIN_MARK || students[i].mark2 > MAX_MARK ||
            students[i].mark3 < MIN_MARK || students[i].mark3 > MAX_MARK) {
            fprintf(stderr, "Error: Marks must be between %.0f and %.0f for roll %d\n", MIN_MARK, MAX_MARK, students[i].rollNumber);
            return -2;
        }
    }
    return 0;
}

void computeResults(struct Student *students, int studentcount) {
    for (int i = 0; i < studentcount; i++) {
        students[i].total = students[i].mark1 + students[i].mark2 + students[i].mark3;
        students[i].average = students[i].total / 3.0f;

        if (students[i].average >= GRADE_A){ 
            students[i].grade = 'A';
        }
        else if (students[i].average >= GRADE_B) {
            students[i].grade = 'B';
        }
        else if (students[i].average >= GRADE_C) {
            students[i].grade = 'C';
        }
        else if (students[i].average >= GRADE_D) {
            students[i].grade = 'D';
        }
        else {
            students[i].grade = 'F';
        }
    }
}

void printPerformanceStars(char grade) {
    int stars = 0;
    switch (grade) {
        case 'A': stars = 5; break;
        case 'B': stars = 4; break;
        case 'C': stars = 3; break;
        case 'D': stars = 2; break;
        default: stars = 0;
    }
    for (int i = 0; i < stars; i++){
        printf("*");
    }
    printf("\n");
}

void printReports(struct Student *students, int studentcount) {
    for (int i = 0; i < studentcount; i++) {
        printf("\nRoll: %d\n", students[i].rollNumber);
        printf("Name: %s\n", students[i].name);
        printf("Total: %.0f\n", students[i].total);
        printf("Average: %.2f\n", students[i].average);
        printf("Grade: %c\n", students[i].grade);

        if (students[i].grade == 'F') {
            continue;
        }

        printf("Performance: ");
        printPerformanceStars(students[i].grade);
    }
}

void printRollNumbers(struct Student *students, int index, int studentcount) {
    if (index >= studentcount){
     return;
    }
    printf("%d ", students[index].rollNumber);
    printRollNumbers(students, index + 1, studentcount);
}

int main() {
    int studentcount;
    if (scanf("%d", &studentcount) != 1) {
        fprintf(stderr, "Error: invalid number of students\n");
        return 1;
    }
    if (studentcount <= 0) {
        fprintf(stderr, "Error: number of students must be positive\n");
        return 1;
    }

    struct Student *students = malloc(sizeof(struct Student) * studentcount);
    if (students == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    int readStatus = readStudents(students, studentcount);
    if (readStatus != 0) {
        free(students);
        return 1;
    }

    computeResults(students, studentcount);

    
    printReports(students, studentcount);
    printf("\n");
    printf("List of Roll Numbers (via recursion): ");
    printRollNumbers(students, 0, studentcount);
    printf("\n");

    free(students);
    return 0;
}
