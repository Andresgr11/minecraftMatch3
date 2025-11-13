#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "node.h"

using namespace std;

template <typename T>

class playersList
{
private:
	node<T>* head;
public:
	playersList() : head(nullptr) {}

	~playersList()
	{
		node<T>* current = head;
		while (current != nullptr)
		{
			node<T>* nextNode = current->getNext();
			delete current;
			current = nextNode;
		}
	}
};