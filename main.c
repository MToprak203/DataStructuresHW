#include<stdio.h>
#include<stdlib.h>

typedef struct hashNode{
    char str[35];
    struct hashNode* next;
}HNODE;

typedef struct listNode{
    char str[35];
    float score;
    struct listNode* next;
    struct listNode* before;
}LNODE;

typedef struct linkedList{
    LNODE* head;
    LNODE* tail;
}LLIST;

void search(char*);

char* completePath(char*);

int compareStr(char*, char*);

void sort(char*);

void mergeSort(char**, int, int, int);

void merge(char**, int, int, int, int);

int isPrior(char*, char*);

void hash(char*);

void hashInsert(HNODE**, char*);

void hashSearch(HNODE**);

int findHashPoint(char*);

void linkedList(char*);

void listInsert(LLIST*, char*);

float passwordScore(char*);

void menu();


int main(){
    menu();
    return 0;
}

void menu(){
    int choice;

    while(1){
        printf("\n1- Search\n2- Sort\n3- Hash\n4- LinkedList\n5- Quit\n");
        scanf("%d", &choice);
        switch(choice){
            case 1 : printf("\n--Searching--\n\n"); search("100000.txt"); break;
            case 2 : printf("\n--Sorting--\n\n"); sort("10000.txt"); break;
            case 3 : printf("\n--Hash--\n\n"); hash("1000.txt"); break;
            case 4 : printf("\n--LinkedList--\n\n"); linkedList("1000.txt"); break;
            case 5 : printf("\n--Shutting Down--\n"); return;
            default : printf("\n--Invalid Choice--\n");
        }
    }
}

void search(char* fileName){
    FILE* fp;
    char* fPath = completePath(fileName);
    if ((fp = fopen(fPath, "r")) == NULL){
        printf("\n--Opening %s is unsuccessful--\n", fileName);
        return;
    }
    char searchPassword[35];
    printf("\nPassword : ");
    scanf("%s", searchPassword);
    char filePassword[35];
    while(fgets(filePassword, 35, fp) != NULL){
        if(compareStr(filePassword, searchPassword)){
            printf("\n%s is in %s\n", searchPassword, fileName);
            fclose(fp);
            return;
        }
    }
    printf("\n%s is not in %s\n", searchPassword, fileName);
    fclose(fp);
}
int strLen(char* string){
    int len = 0;
    for(int i = 0; string[i] != '\0'; ++i)
        ++len;
    return len;
}
char* completePath(char* fileName){
    const char* path = "10-million-password-list-top/";
    const int lenPath = 29;
    const int lenFileName = strLen(fileName);
    char* fullPath = (char*)malloc(lenPath+lenFileName+1);
    for(int i = 0; i < lenPath + lenFileName; ++i){
        if(i < lenPath)
            fullPath[i] = path[i];
        else
            fullPath[i] = fileName[i - lenPath];
    }
    fullPath[lenPath+lenFileName] = '\0';
    return fullPath;
}
int compareStr(char* filePassword, char* searchPassword){
    int i;
    for(i = 0; searchPassword[i] != '\0'; ++i){
        if(filePassword[i] == '\n' || filePassword[i] != searchPassword[i])
            return 0;
    }
    if(filePassword[i] != '\n')
        return 0;
    return 1;
}
void sort(char* fileName){
    FILE* fp;
    char* fPath = completePath(fileName);
    if ((fp = fopen(fPath, "r")) == NULL){
        printf("\n--Opening %s is unsuccessful--\n", fileName);
        return;
    }
    printf("\nYou are working on %s.\n",fileName);
    char** passwords = (char**)malloc(sizeof(char*));
    char password[35];
    int len = 0;
    while(fgets(password, 35, fp) != NULL){
        passwords = (char**)realloc(passwords, (sizeof(char*)*(len+1)));
        passwords[len] = (char*)malloc(35);
        for(int i = 0; i < 35; i++){
            passwords[len][i] = password[i];
        }
        ++len;
    }
    fclose(fp);

    int isAscending = -1;
    while(!(isAscending == 0 || isAscending == 1)){
        printf("\nDescending Order: 0\nAscending Order:  1\n(Choice must be 0 or 1.)\nChoice: ");
        scanf("%d",&isAscending);
    }

    mergeSort(passwords, 0, len-1, isAscending);

    printf("\nTop 10 passwords :\n\n");
    for(int i = 0; i < 100; i++){
        printf("%s\n", passwords[i]);
    }

    for(--len; len >= 0; --len){
        free(passwords[len]);
    }
    free(passwords);
}
void mergeSort(char** passwords, int first, int last, int isAscending){
    if(first < last){
        int mid = first + (last - first) / 2;

        mergeSort(passwords, first, mid, isAscending);
        mergeSort(passwords, mid + 1, last, isAscending);

        merge(passwords, first, mid, last, isAscending);
    }
}
void merge(char** passwords, int first, int mid, int last, int isAscending){
    int i, j, k;
    int n1 = mid - first + 1;
    int n2 = last - mid;
    char** L = (char**)malloc(sizeof(char*)*n1);
    char** R = (char**)malloc(sizeof(char*)*n2);

    for (i = 0; i < n1; i++)
        L[i] = passwords[first + i];
    for (j = 0; j < n2; j++)
        R[j] = passwords[mid + 1 + j];

    i = 0;
    j = 0;
    k = first;
    while (i < n1 && j < n2) {
        if (!(isPrior(L[i], R[j]) ^ isAscending)){
            passwords[k] = L[i];
            i++;
        }
        else {
            passwords[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        passwords[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        passwords[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}
int isPrior(char* left, char* right){
    /** If left is prior returns 1 else 0 **/
    int i = 0;
    while(left[i] == right[i]){
        ++i;
        if(left[i] == '\n')
            return 1;
        if(right[i] == '\n')
            return 0;
    }
    if(left[i] < right[i])
        return 1;
    else
        return 0;
}
void hash(char* fileName){
    FILE* fp;
    char* fPath = completePath(fileName);
    if ((fp = fopen(fPath, "r")) == NULL){
        printf("\n--Opening %s is unsuccessful--\n"), fileName;
        return;
    }
    printf("\nYou are working on %s.\n",fileName);
    HNODE** hash = (HNODE**)malloc(sizeof(HNODE*)*37);
    for(int i = 0; i < 37; ++i)
        hash[i] = NULL;
    char password[35];
    while(fgets(password, 35, fp) != NULL){
        hashInsert(hash, password);
    }
    fclose(fp);

    printf("\nHash created successfully!\n");
    hashSearch(hash);
    for(int i = 0; i < 37; ++i){
        HNODE* iter = hash[i];
        HNODE* temp;
        while(iter != NULL){
            temp = iter;
            iter = iter->next;
            free(temp);
        }
    }
    free(hash);
}
void hashInsert(HNODE** hash, char* password){
    int point = findHashPoint(password);

    if(!hash[point]){
        hash[point] = (HNODE*)malloc(sizeof(HNODE));
        for(int i = 0; i < 35; ++i){
            hash[point]->str[i] = password[i];
        }
        hash[point]->next = NULL;
        return;
    }

    HNODE* temp = hash[point];
    while(temp->next)
        temp = temp->next;
    temp->next = (HNODE*)malloc(sizeof(HNODE));
    for(int i = 0; i < 35; ++i){
        temp->next->str[i] = password[i];
    }
    temp->next->next = NULL;
}
int findHashPoint(char* password){
    if(password[0] >= 'a' && password[0] <='z')
        return password[0] - 'a';

    if(password[0] >= 'A' && password[0] <='Z')
        return password[0] - 'A';

    if(password[0] >= '0' && password[0] <= '9')
        return password[0] - '0' + 26 ;

    return 36;
}
void hashSearch(HNODE** hash){
    int collisionCounter;
    int choice = 1;
    char password[35];
    while(1){
        printf("\n\nWould you like to search a password?\n(Yes:1, No:0)\nChoice:");
        scanf("%d",&choice);
        if(choice == 0)
            break;
        if(choice != 1)
            continue;
        printf("\nPassword: hunter\n");
        strcpy(password, "hunter");
        //scanf("%s", password); //If you want to search your password, delete 'hunter' , strcpy and //.
        HNODE* temp = hash[findHashPoint(password)];
        collisionCounter = 0;
        while(!compareStr(temp->str, password)){
            ++collisionCounter;
            temp = temp->next;
            if(!temp){
                printf("\n%s is not in the hash.\n",password);
                break;
            }
        }
        if(temp)
            printf("\n%s is found. %d times collided.\n\n", password, collisionCounter);
    }
}
void linkedList(char* fileName){
    FILE* fp;
    char* fPath = completePath(fileName);
    if ((fp = fopen(fPath, "r")) == NULL){
        printf("\n--Opening %s is unsuccessful--\n", fileName);
        return;
    }
    printf("\nYou are working on %s.\n",fileName);
    LLIST* lis = (LLIST*)malloc(sizeof(LLIST));
    lis->head = NULL;
    lis->tail = NULL;
    char password[35];
    while(fgets(password, 35, fp) != NULL){
        listInsert(lis, password);
    }

    printf("\nList created successfully!\n");

    LNODE* temp = lis->head;
    printf("\nTop 10 weakest passwords: \n");
    for(int i = 0; i < 10 ; i++){
        printf("\nPassword: %s, Score: %.2f\n", temp->str, temp->score);
        temp = temp->next;
    }

    printf("\n----------------------------------\n\nTop 10 strongest passwords:\n");
    temp = lis->tail;
    for(int i = 0; i < 10 ; i++){
        printf("\nPassword: %s, Score: %.2f\n", temp->str, temp->score);
        temp = temp->before;
    }

    LNODE* iter = lis->head;
    while(iter){
        temp = iter;
        iter = iter->next;
        free(temp);
    }
    free(lis);
}
void listInsert(LLIST* lis, char* password){
    if(!lis->head){
        lis->head = (LNODE*)malloc(sizeof(LNODE));
        lis->head->next   = NULL;
        lis->head->before = NULL;
        lis->head->score = passwordScore(password);
        for(int i = 0; i < 35; ++i)
            lis->head->str[i] = password[i];
        lis->tail = lis->head;
        return;
    }

    LNODE* temp = (LNODE*)malloc(sizeof(LNODE));
    for(int i = 0; i < 35; ++i)
        temp->str[i] = password[i];
    temp->score = passwordScore(password);
    LNODE* iter = lis->head;

    while(temp->score > iter->score){
        iter = iter->next;
        if(!iter)
            break;
    }

    if(!iter){
        lis->tail->next = temp;
        temp->before = lis->tail;
        temp->next = NULL;
        lis->tail = temp;
        return;
    }

    if(iter == lis->head){
        temp->next = lis->head;
        temp->before = NULL;
        lis->head->before = temp;
        lis->head = temp;
        return;
    }

    temp->next = iter;
    temp->before = iter->before;
    temp->next->before = temp;
    temp->before->next = temp;
}
float passwordScore(char* password){
    float score = 0;
    for(int i = 0; password[i] != '\n' && password[i] != '\0'; ++i){
        if(password[i] >= 'a' && password[i] <='z')
            score += 1;
        else if(password[i] >= 'A' && password[i] <='Z')
            score += 1.5;
        else if(password[0] >= '0' && password[0] <= '9')
            score += 2;
        else
            score += 3.5;
        if(password[i-1] && (password[i-1] == password[i] || password[i-1] + 1 == password[i]))
            score -= 2;
    }
    return score;
}






