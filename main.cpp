//테스트 코드 (제출x)
//대부분은 채찍이(Chat gpt)한테 대충 만들어달라고해서 만든 테스트용 코드입니다.

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <atomic>
#include "queue.h"

const int THREAD_COUNT = 4;
const int OPS_PER_THREAD = 10000;
std::atomic<int> success_enq{ 0 }, success_deq{ 0 };

// 각 스레드가 큐에 문자열을 삽입
void threaded_enqueue(Queue* q, int thread_id) {
    for (int i = 0; i < OPS_PER_THREAD; ++i) {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "T%d-%d", thread_id, i);

        int len = static_cast<int>(strlen(buffer)) + 1;
        char* val = (char*)malloc(len);
        strcpy(val, buffer);

        Item item;
        item.key = thread_id * OPS_PER_THREAD + i;
        item.value = val;
        item.value_size = len;

        Reply r = enqueue(q, item);
        if (r.success) success_enq++;

        free(val);
    }
}

// 각 스레드가 큐에서 데이터를 꺼냄
void threaded_dequeue(Queue* q) {
    for (int i = 0; i < OPS_PER_THREAD; ++i) {
        Reply r = dequeue(q);
        if (r.success) {
            success_deq++;
            free(r.item.value);
        }
    }
}

int main() {
    Queue* q = init();

    std::cout << "[enqueue 병렬 테스트 시작]\n";
    std::thread producers[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; ++i)
        producers[i] = std::thread(threaded_enqueue, q, i);
    for (int i = 0; i < THREAD_COUNT; ++i)
        producers[i].join();

    std::cout << "enqueue 성공 횟수: " << success_enq.load() << "\n";

    std::cout << "\n[dequeue 병렬 테스트 시작]\n";
    std::thread consumers[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; ++i)
        consumers[i] = std::thread(threaded_dequeue, q);
    for (int i = 0; i < THREAD_COUNT; ++i)
        consumers[i].join();

    std::cout << "dequeue 성공 횟수: " << success_deq.load() << "\n";

    release(q);
    return 0;
}


