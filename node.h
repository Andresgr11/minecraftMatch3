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
	node<T>* next;
public:
	node(T val) : data(val), next(nullptr) {}
	~node() {}
	T getData() const { return data; }
	void setData(T val) { data = val; }
	node<T>* getNext() const { return next; }
	void setNext(node<T>* nxt) { next = nxt; }
};