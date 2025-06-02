//수정 가능(제출O)
#pragma once

#include <stdbool.h> 
#include <mutex>

typedef int Key;
typedef int Value;

typedef struct {
	Key key;
	Value value;
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
