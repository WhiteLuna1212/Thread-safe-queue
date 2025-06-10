//테스트 코드 (제출x)
//대부분은 채찍이(Chat gpt)한테 대충 만들어달라고해서 만든 테스트용 코드입니다.

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>     // strcpy, strlen
#include <cstdlib>     // malloc, free
#include "queue.h"

void print_item(const Item& item) {
    std::cout << "key: " << item.key
        << ", value: " << static_cast<char*>(item.value)
        << ", size: " << item.value_size << "\n";
}

int main() {
    Queue* q = init();

    // 문자열 예제 데이터 준비 (heap에 할당해서 넘김)
    const char* words[] = { "apple", "banana", "cherry", "banana", "date" };
    const unsigned int keys[] = { 30, 10, 20, 10, 25 };

    const int N = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < N; ++i) {
        int len = strlen(words[i]) + 1;
        char* val = (char*)malloc(len);
        strcpy(val, words[i]);

        Item item;
        item.key = keys[i];
        item.value = val;
        item.value_size = len;

        Reply r = enqueue(q, item);
        if (r.success) {
            std::cout << "[enqueue 성공] ";
            print_item(r.item);
        }
        else {
            std::cout << "[enqueue 실패] key = " << item.key << "\n";
        }

        // enqueue에서 깊은 복사했으므로 원본 해제해도 안전
        free(val);
    }

    std::cout << "\n[dequeue 테스트]\n";
    for (int i = 0; i < N + 1; ++i) { // intentionally 1번 더 호출
        Reply r = dequeue(q);
        if (r.success) {
            std::cout << "[dequeue 성공] ";
            print_item(r.item);
            free(r.item.value); // 반드시 free() 필요
        }
        else {
            std::cout << "[dequeue 실패] 더 이상 꺼낼 항목 없음\n";
        }
    }

    std::cout << "\n[range 테스트: key 15 ~ 30]\n";

    // 다시 enqueue
    for (int i = 0; i < N; ++i) {
        int len = strlen(words[i]) + 1;
        char* val = (char*)malloc(len);
        strcpy(val, words[i]);

        Item item;
        item.key = keys[i];
        item.value = val;
        item.value_size = len;

        enqueue(q, item);
        free(val);
    }

    Queue* filtered = range(q, 15, 30);
    Node* cur = filtered->head;
    while (cur) {
        print_item(cur->item);
        cur = cur->next;
    }

    release(filtered);
    release(q);
    return 0;
}
