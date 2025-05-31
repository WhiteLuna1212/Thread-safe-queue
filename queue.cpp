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

Reply enqueue(Queue* queue, Item item) {
    //[1]
    std::lock_guard<std::mutex> lock(queue->mtx);  // 락으로 thread-safe 보장

    //[2]
    Reply reply;
    reply.item = item;

    //[3]
    Node* new_node = nalloc(item);
    if (!new_node) {
        reply.success = false;
        return reply;
    }

    //[4]
    if (!queue->head) {
        queue->head = queue->tail = new_node;
        reply.success = true;
        return reply;
    }

    //[5]
    if (item.key < queue->head->item.key) {
        new_node->next = queue->head;
        queue->head = new_node;
        reply.success = true;
        return reply;
    }

    //[6]
    Node* prev = queue->head;
    Node* curr = queue->head->next;

    while (curr && curr->item.key <= item.key) {
        prev = curr;
        curr = curr->next;
    }

    //[7]
    prev->next = new_node;
    new_node->next = curr;

    //[8]
    if (!curr) queue->tail = new_node;

    //[9]
    reply.success = true;
    return reply;
}

Reply dequeue(Queue* queue) {
    //[1]
    std::lock_guard<std::mutex> lock(queue->mtx);

    //[2]
    Reply reply;

    //[3]
    if (!queue->head) {
        reply.success = false;
        return reply;
    }

    //[4]
    Node* node = queue->head;
    reply.item = node->item;
    reply.success = true;

    //[5]
    queue->head = node->next;

    //[6]
    if (!queue->head) {
        queue->tail = nullptr;
    }

    //[7]
    nfree(node);
    return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
    return NULL;
}
