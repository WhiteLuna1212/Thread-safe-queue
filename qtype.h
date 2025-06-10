//수정 가능(제출O)
#pragma once

#include <stdbool.h> 
#include <mutex>

typedef unsigned int Key;  // 값이 클수록 높은 우선순위
typedef void* Value;

typedef struct {
	Key key;
	Value value;
	int value_size; // byte
} Item;

typedef struct {
	bool success;   // true: 성공, false: 실패
	Item item;
	// 필드추가가능
} Reply;

typedef struct node_t {
	Item item;
	struct node_t* next;
	// 필드추가가능
} Node;

typedef struct {
	Node* head;
	Node* tail;
	std::mutex mtx;
	// 필드추가가능
} Queue;
