//테스트 코드 (제출x)
//대부분은 채찍이(Chat gpt)한테 대충 만들어달라고해서 만든 테스트용 코드입니다.

#include <iostream>
#include "queue.h"

int main() {
    Queue* q = init();

    // 테스트용 데이터: 무작위 key
    Item items[] = {
        {30, 300},
        {10, 100},
        {20, 200},
        {40, 400},
        {25, 250}
    };

    const int N = sizeof(items) / sizeof(Item);
    for (int i = 0; i < N; ++i) {
        Reply r = enqueue(q, items[i]);
        if (r.success) {
            std::cout << "삽입 성공: key = " << r.item.key << ", value = " << r.item.value << "\n";
        } else {
            std::cout << "삽입 실패: key = " << items[i].key << "\n";
        }
    }

    // 큐 내부 출력 (정렬 상태 확인)
    std::cout << "\n[큐 상태: key 오름차순 출력]\n";
    Node* cur = q->head;
    while (cur) {
        std::cout << "key: " << cur->item.key << ", value: " << cur->item.value << "\n";
        cur = cur->next;
    }

    release(q);
    return 0;
}

/*
예상 출력이라고 함.

삽입 성공: key = 30, value = 300
삽입 성공: key = 10, value = 100
삽입 성공: key = 20, value = 200
삽입 성공: key = 40, value = 400
삽입 성공: key = 25, value = 250

[큐 상태: key 오름차순 출력]
key: 10, value: 100
key: 20, value: 200
key: 25, value: 250
key: 30, value: 300
key: 40, value: 400
*/
