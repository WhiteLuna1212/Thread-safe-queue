//테스트 코드 (제출x)

//gpt한테 대충 시켜서 만든 테스트코드
/*
기대 출력이 이렇게 된다고함.
  큐 내용:
key: 10, value: 100
key: 20, value: 200
*/

int main() {
    // 큐 초기화
    Queue* q = init();
    if (!q) {
        printf("큐 초기화 실패\n");
        return 1;
    }

    // 노드 수동으로 생성 및 연결
    Item item1 = { .key = 10, .value = 100 };
    Item item2 = { .key = 20, .value = 200 };
    Node* n1 = nalloc(item1);
    Node* n2 = nalloc(item2);

    if (!n1 || !n2) {
        printf("노드 생성 실패\n");
        return 1;
    }

    // 큐에 직접 연결해보기
    q->head = n1;
    n1->next = n2;
    q->tail = n2;

    // 큐 출력
    printf("큐 내용:\n");
    Node* current = q->head;
    while (current) {
        printf("key: %d, value: %d\n", current->item.key, current->item.value);
        current = current->next;
    }

    // 메모리 해제
    release(q);

    return 0;
}
