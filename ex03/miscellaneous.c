#include "miscellaneous.h"

void initList(List *list) {
    list->head = list->tail = NULL;
    list->size = 0;
}

Node* addNode(List *list, BLOCK_T block) {
    Node* node = (Node*)calloc(1, sizeof(Node));
    node->block = block;

    if (list->size == 0) {
        list->head = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
    }

    list->tail = node;
    list->size++;
    return node;
}

int getCurrentTimeStamp() {
    char* timestamp = (char*)malloc(16);
	time_t ltime;
    ltime = time(NULL);
    struct tm *tm;
    tm = localtime(&ltime);

    sprintf(timestamp,"%04d%02d%02d%02d%02d%02d", tm->tm_year+1900, tm->tm_mon, 
        tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

	int result = atoi(timestamp);
	free(timestamp);
    return result;
}

char *itoa(int number) {
    char *result = (char*)calloc(1, MAX_SUPPORTED_NUM_OF_DIGITS + 1 * sizeof(char));
    sprintf(result, "%d", number);

    return result;
}