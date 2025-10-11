#include<stdio.h>

struct student
{
    int rollNo;
    char name[20];
    float mark1, mark2, mark3;
    char grade;
};
int studentCount;
void readData(struct student *s)
{ 
    for (int i = 0; i < studentCount; i++) {
        scanf("%d", &s[i].rollNo);
        scanf("%s", s[i].name);
        scanf("%f %f %f", &s[i].mark1, &s[i].mark2, &s[i].mark3);
    }
};

void printRollNoRecursively(struct student *s, int index)
{
    if (index == studentCount)
        return;
    printf("%d ", s[index].rollNo);
    printRollNoRecursively(s, index + 1);
}

int main()
{   float totalMarks, averageMarks;
    char *performanceStars=NULL;
    scanf("%d", &studentCount); // Input number of students
    struct student s[studentCount];
    readData(s);
    for (int i = 0; i < studentCount; i++) {
        totalMarks = s[i].mark1 + s[i].mark2 + s[i].mark3;
        averageMarks = totalMarks / 3;
        if (averageMarks >= 85){
            s[i].grade = 'A';
            performanceStars="*****";
        }
        else if (averageMarks >= 70){
            s[i].grade = 'B';
            performanceStars="****";
        }
        else if (averageMarks >= 50){
            s[i].grade = 'C';
            performanceStars="***";
        }
        else if (averageMarks >= 35){
            s[i].grade = 'D';
            performanceStars="**";
        }
        else
        {
            s[i].grade = 'F';
        }

        printf("Roll No: %d\nName: %s\nTotal Marks: %.2f\nAverage Marks: %.2f\nGrade: %c\n", s[i].rollNo, s[i].name, totalMarks, averageMarks, s[i].grade);
        if(s[i].grade!='F'){
            printf("Performance: %s\n\n", performanceStars);
        }
    }

    printf("List of Roll Numbers(Via Recursion): ");
    printRollNoRecursively(s, 0);
    printf("\n");

    return 0;
}

