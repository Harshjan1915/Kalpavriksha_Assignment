#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024

struct freeBlock{
    int index;
    struct freeBlock* next;
    struct freeBlock* prev;
};

struct fileNode{
    char name[50];
    int isFolder;
    struct fileNode* parent;
    struct fileNode* child;
    struct fileNode* next;
    struct fileNode* prev;
    int blockPointers[20];
    int fileSize;
};

void createDirectoryOrFile(struct fileNode* cwd, int isFolder){
    struct fileNode* newNode = (struct fileNode*)malloc(sizeof(struct fileNode));
    char FileorDirName[50];
    scanf("%s", FileorDirName);
    strcpy(newNode->name, FileorDirName);
    struct fileNode* head1 = cwd->child;
    struct fileNode* temp1 = head1;
    if(head1 != NULL){
    do{
        if(strcmp(temp1->name,FileorDirName)==0){
            printf("File or Directory with name '%s' already exists.\n",FileorDirName);
            free(newNode);
            return;
        }
        temp1=temp1->next;
    }while(temp1!=head1);
    }
    newNode->isFolder = isFolder;
    newNode->parent = cwd;
    newNode->child = NULL;
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->fileSize = 0;

    if(cwd->child == NULL){
        cwd->child = newNode;
        newNode->next = newNode;
    } 
    else {
        struct fileNode* head = cwd->child;
        struct fileNode* temp = head;
        while(temp->next != head){
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
        newNode->next = head;
    }
    if(isFolder){
        printf("Directory '%s' created successfully.\n", newNode->name);
    }
    else{
        printf("File '%s' created successfully.\n", newNode->name);
    }
}

void writeFile(struct fileNode* cwd, char virtualDisk[NUM_BLOCKS][BLOCK_SIZE], struct freeBlock** freeListHead){
    char fileName[50];
    scanf(" %s", fileName);
    struct fileNode* head = cwd->child;
    struct fileNode* temp = head;
    int blockCount = 0;
    struct freeBlock* block = *freeListHead;

    char data[BLOCK_SIZE];
    while(getchar()!= '"');
    scanf("%[^\"]", data);
    getchar();

    if(head==NULL){
        printf("No files to write. Create a file first.\n");
        return;
    }
    do{
        if(strcmp(temp->name, fileName) == 0 && temp->isFolder == 0){

            blockCount = strlen(data)/BLOCK_SIZE + 1;

            for(int i = 0; i < blockCount; i++){
                temp->blockPointers[i] = block->index;
                *freeListHead = (*freeListHead)->next;
                block = *freeListHead;
            }

            strncpy(virtualDisk[temp->blockPointers[0]], data, BLOCK_SIZE);
            temp->fileSize = strlen(data);
            printf("Data written Successfully (Size: %d bytes)\n", temp->fileSize);
            return;
        }
        temp = temp->next;
    }
    while(temp != head);
    printf("File '%s' not found.\n", fileName);
}

void readFile(struct fileNode* cwd, char virtualDisk[NUM_BLOCKS][BLOCK_SIZE]){
    char fileName[50];
    scanf(" %s", fileName);
    struct fileNode* head = cwd->child;
    struct fileNode* temp = head;
    if(head==NULL){
        printf("No files to read.\n");
        return;
    }
    do{
        if(strcmp(temp->name, fileName) == 0 && temp->isFolder == 0){
            int size = temp->fileSize;
            int blocksToRead = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
            for(int i = 0; i < blocksToRead; i++){
                printf("%s", virtualDisk[temp->blockPointers[i]]);
            }
            printf("\n");
            return;
        }
        temp = temp->next;
    }
    while(temp != head);
    printf("File '%s' not found.\n", fileName);
}

void deleteFile(struct fileNode* cwd, char virtualDisk[NUM_BLOCKS][BLOCK_SIZE], struct freeBlock** freeListTail){
    char fileName[50];
    scanf(" %s", fileName);
    struct fileNode* head = cwd->child;
    struct fileNode* temp = head;
    do{
        if(strcmp(temp->name, fileName) == 0 && temp->isFolder == 0){
            int blocksToFree = (temp->fileSize + BLOCK_SIZE - 1) / BLOCK_SIZE;
            for(int i = 0; i < blocksToFree; i++){
                struct freeBlock* newBlock = (struct freeBlock*)malloc(sizeof(struct freeBlock));
                newBlock->index = temp->blockPointers[i];
                newBlock->next = NULL;
                newBlock->prev = *freeListTail;
                (*freeListTail)->next = newBlock;
                
                *freeListTail = newBlock;
            }
            if(temp->prev != NULL){
                temp->prev->next = temp->next;
            }
            if(temp->next != NULL){
                temp->next->prev = temp->prev;
            }
            if(cwd->child == temp){
                if(temp->next != temp){
                    cwd->child = temp->next;
                    temp->next->parent=cwd;
                }
                else{
                    cwd->child = NULL;
                }
            }
            free(temp);
            printf("File '%s' deleted successfully.\n", fileName);
            return;
        }
        if(strcmp(temp->name, fileName) == 0 && temp->isFolder == 1){
            printf("'%s' is a directory. Use rmdir to delete directories.\n", fileName);
            return;
        }
        temp = temp->next;
    }
    while(temp != head);
    printf("File '%s' not found.\n", fileName);
}

void removeDirectory(struct fileNode* cwd){
    char dirName[50];
    scanf(" %s", dirName);
    struct fileNode* head = cwd->child;
    struct fileNode* temp = head;
    if(head==NULL){
        printf("No directories to remove.\n");
        return;
    }
    do{
        if(strcmp(temp->name, dirName) == 0 && temp->isFolder == 1){
            if(temp->child != NULL){
                printf("Directory '%s' is not empty.\n", dirName);
                return;
            }
            if(temp->prev != NULL){
                temp->prev->next = temp->next;
            }
            if(temp->next != NULL){
                temp->next->prev = temp->prev;
            }
            if(cwd->child == temp){
                if(temp->next != temp){
                    cwd->child = temp->next;
                    temp->next->parent=cwd;
                }
                else{
                    cwd->child = NULL;
                }
            }
            free(temp);
            printf("Directory '%s' deleted successfully.\n", dirName);
            return;
        }
        if(strcmp(temp->name, dirName) == 0 && temp->isFolder == 0){
            printf("'%s' is a file. Use delete to remove files.\n", dirName);
            return;
        }
        temp = temp->next;
    }
    while(temp != head);

    printf("Directory '%s' not found.\n", dirName);
}

void changeDirectory(struct fileNode** cwd){
    char dirName[50];
    scanf(" %s", dirName);
    if(strcmp(dirName, "..") == 0){
        if((*cwd)->parent != NULL){
            *cwd = (*cwd)->parent;
        }
        return;
    }
    struct fileNode* head = (*cwd)->child;
    struct fileNode* temp = head;
    do{
        if(strcmp(temp->name, dirName) == 0 && temp->isFolder == 1){
            *cwd = temp;
            printf("Moved to /%s\n", dirName);
            return;
        }
        temp = temp->next;
    }
    while(temp != head);
    printf("Directory '%s' not found.\n", dirName);
}

void printWorkingDirectory(struct fileNode* cwd){
    if(cwd->parent == NULL){
        printf("/\n");
        return;
    }
    char path[500] = "";
    struct fileNode* temp = cwd;
    while(temp != NULL){
        char tempName[50];
        strcpy(tempName, temp->name);
        strcat(tempName, "/");
        strcat(tempName, path);
        strcpy(path, tempName);
        temp = temp->parent;
    }
    printf("%s\n", path);
}

void showDiskUsage(struct freeBlock* head){
    int freeBlocks = 0;
    struct freeBlock* temp = head;
    while(temp != NULL){
        freeBlocks++;
        temp = temp->next;
    }
    int usedBlocks = NUM_BLOCKS - freeBlocks;
    float usagePercent = (usedBlocks * 100.0) / NUM_BLOCKS;
    printf("Total Blocks: %d\nUsed Blocks: %d\nFree Blocks: %d\nDisk Usage: %.2f%%\n", NUM_BLOCKS, usedBlocks, freeBlocks, usagePercent);
}
void listDirectory(struct fileNode* cwd){
    if(cwd->child == NULL){
        printf("(empty)\n");
        return;
    }
    struct fileNode* head = cwd->child;
    struct fileNode* temp = head;
    do {
        printf("%s/ \n", temp->name);
        temp = temp->next;
    } while (temp != head);
    printf("\n");
}

void freeDiskAndList(struct freeBlock* head, struct fileNode* root){
    struct freeBlock* tempBlock = head;
    while(tempBlock != NULL){
        struct freeBlock* nextBlock = tempBlock->next;
        free(tempBlock);
        tempBlock = nextBlock;
    }

    void freeFileNodes(struct fileNode* node){
        if(node == NULL) {
            return;
        }
        struct fileNode* head = node->child;
        if(head != NULL){
            struct fileNode* temp = head;
            do{
                struct fileNode* nextNode = temp->next;
                freeFileNodes(temp);
                temp = nextNode;
            }while(temp != head);
        }
        free(node);
    }
    freeFileNodes(root);
}

int main(){
    char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];

    struct fileNode* root = (struct fileNode*)malloc(sizeof(struct fileNode));
    strcpy(root->name, "/");
    root->isFolder = 1;
    root->parent = NULL;
    root->child = NULL;
    struct fileNode* cwd = root;

    struct freeBlock* head = NULL;
    for(int index = NUM_BLOCKS - 1; index >= 0; index--){
        struct freeBlock* newBlock = (struct freeBlock*)malloc(sizeof(struct freeBlock));
        newBlock->index = index;
        newBlock->next = head;
        newBlock->prev = NULL;
        if(head != NULL){
            head->prev = newBlock;
        }
        head = newBlock;
    }

    struct freeBlock* tail = head;
    while(tail->next != NULL){
        tail = tail->next;
    }
 
    while(1){
        printf("%s> ", cwd->name);
        char command[100];
        scanf(" %s", command);
        if(strcmp(command, "mkdir") == 0){
            createDirectoryOrFile(cwd, 1);
        }
        else if(strcmp(command, "create") == 0){
            createDirectoryOrFile(cwd, 0);
        }
        else if(strcmp(command, "write") == 0){
            writeFile(cwd, virtualDisk, &head);
        }
        else if(strcmp(command, "read") == 0){
            readFile(cwd, virtualDisk);
        }
        else if(strcmp(command, "delete") == 0){
            deleteFile(cwd, virtualDisk, &tail);
        }
        else if(strcmp(command, "rmdir") == 0){
            removeDirectory(cwd);
        }
        else if(strcmp(command, "cd") == 0){
            changeDirectory(&cwd);
        }
        else if(strcmp(command, "pwd") == 0){
            printWorkingDirectory(cwd);
        }
        else if(strcmp(command, "df") == 0){
            showDiskUsage(head);
        }
        else if(strcmp(command, "ls") == 0){
            listDirectory(cwd);
        }
        else if(strcmp(command, "exit") == 0){
            free(cwd);
            freeDiskAndList(head, root);
            printf("Memory released. Exiting program...\n");
            break;
        }
        else{
            printf("Invalid command.\n");
            printf("Available commands: mkdir, create, write, read, delete, rmdir, cd, pwd, df, ls, exit\n");
        }       
    }

    return 0;

}
