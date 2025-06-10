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
    std::lock_guard<std::mutex> lock(queue->mtx);

    Reply reply;

    // [1] 동일 key가 존재하면 value만 덮어쓰기
    Node* cur = queue->head;
    while (cur) {
        if (cur->item.key == item.key) {
            // 기존 value 메모리 해제 후 새로운 값으로 덮어쓰기
            free(cur->item.value);
            cur->item.value = malloc(item.value_size);
            memcpy(cur->item.value, item.value, item.value_size);
            cur->item.value_size = item.value_size;

            reply.item = cur->item;
            reply.success = true;
            return reply;
        }
        cur = cur->next;
    }

    // [2] 새 노드용 item 깊은 복사
    Item copied_item;
    copied_item.key = item.key;
    copied_item.value_size = item.value_size;
    copied_item.value = malloc(item.value_size);
    memcpy(copied_item.value, item.value, item.value_size);

    // [3] 노드 생성
    Node* new_node = nalloc(copied_item);
    if (!new_node) {
        reply.success = false;
        return reply;
    }

    // [4] 정렬 삽입
    // 빈큐 or 맨 앞 삽입
    if (!queue->head || copied_item.key < queue->head->item.key) {
        new_node->next = queue->head;
        queue->head = new_node;
        if (!queue->tail) queue->tail = new_node;
        reply.item = copied_item;
        reply.success = true;
        return reply;
    }

    // 중간 or 뒤 삽입
    Node* prev = queue->head;
    Node* curr = queue->head->next;

    while (curr && curr->item.key <= copied_item.key) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = new_node;
    new_node->next = curr;
    if (!curr) queue->tail = new_node;

    //[5] 복사된 아이템을 담아 리턴
    reply.item = copied_item;
    reply.success = true;
    return reply;
}

Reply dequeue(Queue* queue) {
    std::lock_guard<std::mutex> lock(queue->mtx);

    Reply reply;
    reply.success = false;

    if (!queue->head) {
        return reply;
    }

    //[1] head 제거, 그리고 다음 노드로 head를 이동, tail도 비워야할 경우 갱신
    Node* node = queue->head;
    queue->head = node->next;
    if (!queue->head) queue->tail = nullptr;

    //[2] 깊은 복사 수행
    reply.item.key = node->item.key;
    reply.item.value_size = node->item.value_size;
    reply.item.value = malloc(node->item.value_size);
    memcpy(reply.item.value, node->item.value, node->item.value_size);

    //[3] 메모리 정리 후 리턴
    reply.success = true;

    // 기존 노드 해제
    free(node->item.value);
    delete node;

    return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
    std::lock_guard<std::mutex> lock(queue->mtx);

    Queue* new_queue = init();
    if (!new_queue) return nullptr;

    //[1] 순회하며 범위 체크 + 복사 수행
    Node* curr = queue->head;

    while (curr) {
        if (curr->item.key >= start && curr->item.key <= end) {
            // item 깊은 복사
            Item copied_item;
            copied_item.key = curr->item.key;
            copied_item.value_size = curr->item.value_size;
            copied_item.value = malloc(copied_item.value_size);
            memcpy(copied_item.value, curr->item.value, copied_item.value_size);

            enqueue(new_queue, copied_item);
        }
        curr = curr->next;
    }

    //[2] 결과 넣기
    return new_queue;
}

