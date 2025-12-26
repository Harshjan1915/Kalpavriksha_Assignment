#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define MAX_ARRAY_SIZE 100

struct Message {
  long messageType;
  int arraySize;
  int numbers[MAX_ARRAY_SIZE];
};

int compareIntegers(const void *firstElement, const void *secondElement) {
  return (*(int *)firstElement - *(int *)secondElement);
}

void printMessageArray(struct Message *message) {
  for (int i = 0; i < message->arraySize; i++) {
    printf("%d\t", message->numbers[i]);
  }
  printf("\n");
}

void sendMessage(int messageQueueId, struct Message *message) {
  if (msgsnd(messageQueueId, message, sizeof(*message) - sizeof(long), 0) ==
      -1) {
    perror("Error sending message");
    exit(1);
  }
}

void receiveMessage(int messageQueueId, struct Message *message, long type) {
  if (msgrcv(messageQueueId, message, sizeof(*message) - sizeof(long), type,
             0) == -1) {
    perror("Error receiving message");
    exit(1);
  }
}

void executeChildProcess(int messageQueueId) {
  struct Message message;

  receiveMessage(messageQueueId, &message, 1);

  qsort(message.numbers, message.arraySize, sizeof(int), compareIntegers);

  message.messageType = 2;
  sendMessage(messageQueueId, &message);

  exit(0);
}

void executeParentProcess(int messageQueueId, struct Message *message) {
  message->messageType = 1;
  sendMessage(messageQueueId, message);

  wait(NULL);
  receiveMessage(messageQueueId, message, 2);

  printf("After sorting:\n");
  printMessageArray(message);

  msgctl(messageQueueId, IPC_RMID, NULL);
}

int main() {
  key_t key;
  int messageQueueId;
  struct Message message;

  if (scanf("%d", &message.arraySize) != 1) {
    fprintf(stderr, "Error reading size\n");
    return 1;
  }

  key = ftok("data", 77);
  if (key == -1) {
    perror("ftok failed");
  }

  messageQueueId = msgget(key, 0666 | IPC_CREAT);
  if (messageQueueId == -1) {
    perror("msgget failed");
    return 1;
  }

  for (int i = 0; i < message.arraySize; i++) {
    scanf("%d", &message.numbers[i]);
  }

  printf("Before sorting:\n");
  printMessageArray(&message);

  int processId = fork();

  if (processId < 0) {
    perror("Fork failed");
    return 1;
  } else if (processId == 0) {
    executeChildProcess(messageQueueId);
  } else {
    executeParentProcess(messageQueueId, &message);
  }
  return 0;
}
