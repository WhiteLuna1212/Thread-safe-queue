//큐 구현 (제출)

#include <iostream>
#include "queue.h"

Queue* init(void) {
    Queue* q = new Queue;
    q->head = q->tail = nullptr;
    return q;
}

void release(Queue* queue) {
    Node* current = queue->head;
    while (current) {
        Node* next = current->next;
        nfree(current);
        current = next;
    }
    delete queue;
}

Node* nalloc(Item item) {
    Node* node = new Node;
    node->item = item;
    node->next = nullptr;
    return node;
}

void nfree(Node* node) {
    delete node;
}

Node* nclone(Node* node) {
	return NULL;
}
