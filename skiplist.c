#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#define MAX_LEVEL 5

typedef struct Node {
    int key;
    int value;
    struct Node **forward;
} Node;

typedef struct SkipList {
    int levels;
    struct Node *head;
} SkipList;

int search(SkipList*, int );
void insert(SkipList*, int, int);
void delete(SkipList*, int);
Node* makeNode(int, int, int);
SkipList* initList();
int genreateLevel();
void displayList(SkipList*);


int search(SkipList *list, int search_key) {
    if(!list) {
        perror("List doesn't exist\n");
        return -1;
    }

    Node *ptr = list->head;
    for(int i = list->levels; i >= 0; --i) {
        while(ptr->forward[i] && ptr->forward[i]->key < search_key)
            ptr = ptr->forward[i];
    }
    ptr = ptr->forward[0];

    if(ptr && ptr->key == search_key)
        return ptr->value;
    else
        return -1;
}

void insert(SkipList *list, int search_key, int value) {
    Node *update[MAX_LEVEL];
    
    Node *ptr = list->head;
    for(int i = list->levels; i >= 0; --i) {
        while(ptr->forward[i] && ptr->forward[i]->key < search_key)
            ptr = ptr->forward[i];
        update[i] = ptr;
    }

    ptr = ptr->forward[0];
    if(ptr && ptr->key == search_key) {
        ptr->value = value;
        printf("Value updated\n");
        return;
    }
    
    int level = genreateLevel();
    printf("List levels: %d\n", list->levels);
    if (level > list->levels+1) {
        for(int i = (list->levels)+1; i < level; ++i)
            update[i] = list->head;
        list->levels = level-1;
    }

    ptr = makeNode(level, search_key, value);

    for(int i = 0; i < level; ++i) {
        ptr->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = ptr;
        printf("Inserted value: %d\n", update[i]->forward[i]->key);
    }
      
    printf("Value Inserted\n");
}

void delete(SkipList *list, int search_key) {

    Node *update[MAX_LEVEL];
    
    Node *ptr = list->head;
    for(int i = list->levels; i >= 0; --i) {
        while(ptr->forward[i] && ptr->forward[i]->key < search_key)
            ptr = ptr->forward[i];
        update[i] = ptr;
    }

    ptr = ptr->forward[0];

    if(ptr && ptr->key == search_key) {
        for(int i = 0; i < list->levels; i++) {
            //making sure it's the node before the node to be deleted.
            if(update[i]->forward[i] != ptr)
                break;
            update[i]->forward[i] = ptr->forward[i];
        }
        free(ptr);
    }
    while(list->levels > 0 && list->head->forward[list->levels] == NULL) 
        list->levels -= 1;
    
    printf("Deleted Successfully\n");
}

Node* makeNode(int level, int key, int value) {
    Node *new_node = (Node*) malloc(sizeof(Node));
    
    new_node->key = key;
    new_node->value = value;
    
    new_node->forward = (Node**) malloc(sizeof(Node*) * level);
    for(int i = 0; i < level; ++i)
        new_node->forward[i] = NULL;
    
    return new_node;
}

SkipList* initList() {
    SkipList *new_list = (SkipList*) malloc(sizeof(SkipList) * 1);
    new_list->levels = 0;
    new_list->head   = makeNode(MAX_LEVEL, INT_MIN, INT_MIN);
    return new_list;
}

int genreateLevel() {
    int level = 1;
    int flip = 0;
    do {
       flip = rand() % 2;
       if(flip)
           level+=1;
    } while(flip && level <= MAX_LEVEL);
    
    return level; 
}

void displayList(SkipList *list) {
    Node *ptr = list->head;
    for(int i = list->levels; i >= 0; --i) {
        printf("Level %d: ", i);
        ptr = list->head;
        while(ptr) {
            printf("%d ", ptr->key);
            ptr = ptr->forward[i];
        }
        printf("\n");
    }
}



int main() {
    srand(1);

    SkipList *list = initList();
    
    int key = 0, value = 0;

    while(1) {
        scanf("%d %d", &key, &value);
        if(key == -1)
            break;
        insert(list, key, value);
        displayList(list);
    }
        displayList(list);
    return 0;
}

