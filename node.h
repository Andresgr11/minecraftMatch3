#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

template <typename T>

class node
{
private:
	T data;
	node* next;
public:
	node(int val) : data(val), next(nullptr) {}
	T getData() const { return data; }
	void setData(int val) { data = val; }
	node* getNext() const { return next; }
	void setNext(node* nxt) { next = nxt; }
};