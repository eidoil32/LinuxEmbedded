#include "Miscellaneous.h"
#include "Block.h"

// data of Node must be none-static memory (allocated in runtime)
void freeList(List list) {
	Node current = list->head;
	while (current) {
		Node temp = current->next;
		free(current);
		current = temp;
	}
}

// creating and adding new node to tail
Node addNode(List list, void* data) {
	Node node = (Node)malloc(NODE_SIZE);
	if (node) {
		node->data = data;
		node->next = NULL;
		if (!list->head) { // empty list
			list->size = 0;
			list->head = list->tail = node;
		} else {
			node->prev = list->tail;
			list->tail->next = node;
			list->tail = node;
		}
		list->size++;
	} else { // allocate memory for new node failed
		fprintf(stderr, ERROR_CREATING_NEW_NODE);
	}

	return node;
}

BLOCK_T copyCtor(BLOCK_T block) {
	BLOCK_T newBlock = initBlock(block->relayed_by);
	newBlock->height = block->height;
	newBlock->hash = block->hash;
	newBlock->prev_hash = block->prev_hash;
	newBlock->nonce = block->nonce;

	free(block);
	return newBlock;
}

// get current timestamp
int getCurrentTimeStamp() {
    String timestamp = (String)malloc(16);
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