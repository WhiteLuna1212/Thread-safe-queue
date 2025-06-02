//테스트 코드 (제출x)
//대부분은 채찍이(Chat gpt)한테 대충 만들어달라고해서 만든 테스트용 코드입니다.

//병렬성 테스트 코드 제작
#include <iostream>
#include <thread>
#include <chrono>
#include "queue.h"

// 각 스레드가 큐에 넣을 아이템들을 생성하고 삽입
void enqueue_range(Queue* q, int start_key, int count, int value_base) {
    for (int i = 0; i < count; ++i) {
        Item item = { start_key + i, value_base + i };
        enqueue(q, item);
    }
}

int main() {
    Queue* q = init();

    const int thread_count = 4;
    const int items_per_thread = 25000; // 총 10만 개 삽입
    std::thread threads[thread_count];  // ✅ vector 대신 정적 배열

    auto start = std::chrono::high_resolution_clock::now();

    // 1. 각 스레드가 서로 다른 key 범위의 아이템 삽입
    for (int i = 0; i < thread_count; ++i) {
        int key_start = i * items_per_thread;
        int value_base = i * 100000;
        threads[i] = std::thread(enqueue_range, q, key_start, items_per_thread, value_base);
    }

    // 2. 모든 스레드가 작업을 마칠 때까지 기다림
    for (int i = 0; i < thread_count; ++i) {
        threads[i].join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "[병렬 enqueue 테스트 완료] 삽입 소요 시간: " << duration << "ms\n";

    // 3. 큐 상태 점검 (총 개수 및 정렬 여부)
    std::cout << "삽입된 총 항목 수 확인 중...\n";

    Node* cur = q->head;
    int count = 0;
    bool sorted = true;
    int last_key = -1;

    while (cur) {
        if (cur->item.key < last_key) {
            sorted = false;
        }
        last_key = cur->item.key;
        ++count;
        cur = cur->next;
    }

    std::cout << "총 삽입된 항목 수: " << count << "\n";
    std::cout << "정렬 상태: " << (sorted ? "유지됨" : "정렬 깨짐") << "\n";

    release(q);
    return 0;
}

//기초 기능 테스트를 위한 최종 테스트 코드를 깔끔하게 정리.
/*
#include <iostream>
#include "queue.h"

int main() {
    Queue* q = init();

    // 1. 테스트용 데이터 삽입 (무작위 key)
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
        }
        else {
            std::cout << "삽입 실패: key = " << items[i].key << "\n";
        }
    }

    // 2. 큐 상태 출력
    std::cout << "\n[큐 상태: key 오름차순 출력]\n";
    Node* cur = q->head;
    while (cur) {
        std::cout << "key: " << cur->item.key << ", value: " << cur->item.value << "\n";
        cur = cur->next;
    }

    // 3. dequeue 테스트
    std::cout << "\n[dequeue 테스트: 큐에서 하나씩 꺼내기]\n";
    for (int i = 0; i < N + 1; ++i) { // intentionally 하나 더 꺼냄
        Reply r = dequeue(q);
        if (r.success) {
            std::cout << "꺼냄: key = " << r.item.key << ", value = " << r.item.value << "\n";
        }
        else {
            std::cout << "더 이상 꺼낼 항목이 없습니다.\n";
        }
    }

    // 4. range 테스트 (enqueue로 다시 채우고 range)
    for (int i = 0; i < N; ++i) {
        enqueue(q, items[i]);  // 큐 다시 채우기
    }

    std::cout << "\n[range 테스트: key 15~30 범위의 항목만 복사해서 새 큐 만들기]\n";

    Queue* ranged = range(q, 15, 30);  // 새 큐 생성
    if (ranged) {
        Node* cur2 = ranged->head;
        while (cur2) {
            int key = cur2->item.key;
            int value = cur2->item.value;
            std::cout << "key: " << key << ", value: " << value << "\n";

            // 검증: 범위 안에 있는지 체크
            if (key < 15 || key > 30) {
                std::cout << "❌ 범위 오류: key = " << key << "\n";
            }

            cur2 = cur2->next;
        }

        release(ranged);  // 새 큐 해제
    }
    else {
        std::cout << "range() 호출 실패: 새 큐 생성에 실패했습니다.\n";
    }

    // 5. 원래 큐 해제
    release(q);

    return 0;
}
*/

/*
예상 출력
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

[dequeue 테스트: 큐에서 하나씩 꺼내기]
꺼냄: key = 10, value = 100
꺼냄: key = 20, value = 200
꺼냄: key = 25, value = 250
꺼냄: key = 30, value = 300
꺼냄: key = 40, value = 400
더 이상 꺼낼 항목이 없습니다.

[range 테스트: key 15~30 범위의 항목만 복사해서 새 큐 만들기]
key: 20, value: 200
key: 25, value: 250
key: 30, value: 300
*/
