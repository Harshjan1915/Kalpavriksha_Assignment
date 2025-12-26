#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int compareIntegers(const void *firstElement, const void *secondElement) {
  return (*(int *)firstElement - *(int *)secondElement);
}

void printArray(int *numbers, int arraySize) {
  for (int i = 0; i < arraySize; i++) {
    printf("%d\t", numbers[i]);
  }
  printf("\n");
}

void executeChildProcess(int *pipeFileDescriptors, int *numbers,int arraySize) {
  if (read(pipeFileDescriptors[0], numbers, sizeof(int) * arraySize) == -1) {
    perror("Child read failed");
    exit(1);
  }
  close(pipeFileDescriptors[0]);

  qsort(numbers, arraySize, sizeof(int), compareIntegers);

  if (write(pipeFileDescriptors[1], numbers, sizeof(int) * arraySize) == -1) {
    perror("Child write failed");
    exit(1);
  }
  close(pipeFileDescriptors[1]);
  exit(0);
}

void executeParentProcess(int *pipeFileDescriptors, int *numbers,int arraySize) {
  if (write(pipeFileDescriptors[1], numbers, sizeof(int) * arraySize) == -1) {
    perror("Parent write failed");
    exit(1);
  }
  close(pipeFileDescriptors[1]);

  wait(NULL);

  if (read(pipeFileDescriptors[0], numbers, sizeof(int) * arraySize) == -1) {
    perror("Parent read failed");
    exit(1);
  }
  close(pipeFileDescriptors[0]);

  printf("After sorting:\n");
  printArray(numbers, arraySize);
}

int main() {
  int arraySize;
  if (scanf("%d", &arraySize) != 1)
    return 1;

  int numbers[arraySize];

  int pipeFileDescriptors[2];
  if (pipe(pipeFileDescriptors) != 0) {
    printf("Creating pipe failed\n");
    return 1;
  }

  for (int i = 0; i < arraySize; i++) {
    scanf("%d", &numbers[i]);
  }

  printf("Before sorting:\n");
  printArray(numbers, arraySize);

  int processId = fork();

  if (processId < 0) {
    perror("Fork failed");
    return 1;
  } else if (processId == 0) {
    executeChildProcess(pipeFileDescriptors, numbers, arraySize);
  } else {
    executeParentProcess(pipeFileDescriptors, numbers, arraySize);
  }
  return 0;
}
