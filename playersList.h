#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "node.h"
#include "player.h"

using namespace std;

class playersList
{
private:
	node<player>* head;
public:
	playersList() : head(nullptr) {}

	~playersList()
	{
		node<player>* current = head;
		while (current != nullptr)
		{
			node<player>* nextNode = current->getNext();
			delete current;
			current = nextNode;
		}
	}

	node<player>* findPlayerNode(const string& name)
	{
		node<player>* current = head;
		while (current != nullptr) {
			if (current->getData().getName() == name)
			{
				return current;
			}
			current = current->getNext();
		}
		return nullptr;
	}

	node<player>* findOrCreatePlayer(const string& name)
	{
		node<player>* existingPlayer = findPlayerNode(name);
		if (existingPlayer != nullptr)
		{
			cout << "Jugador '" << name << "' encontrado." << endl;
			return existingPlayer;
		}
		cout << "Creando nuevo jugador: " << name << endl;
		player newPlayerData(name, 0, 0);
		node<player>* newNode = new node<player>(newPlayerData);
		newNode->setNext(head);
		head = newNode;
		return newNode;
	}

	void saveToFile(const string& filename) {
		ofstream save(filename);
		if (!save.is_open())
		{
			cerr << "Error: No se pudo abrir el archivo para guardar: " << filename << endl;
			return;
		}

		node<player>* current = head;
		while (current != nullptr)
		{
			save << current->getData().getName() << "," << current->getData().getMaxScore() << "," << current->getData().getLevelsCompleted() << "\n";
			current = current->getNext();
		}
		save.close();
		cout << "Datos de jugadores guardados en: " << filename << endl;
	}

	void loadFromFile(const string& filename)
	{
		ifstream loadPLayers(filename);
		if (!loadPLayers.is_open())
		{
			cerr << "No se encontro el archivo de jugadores: " << filename << endl;
			return;
		}

		string line;
		while (getline(loadPLayers, line))
		{
			stringstream ss(line);
			string segment;
			string name;
			int maxScore = 0;
			int levelsCompleted = 0;

			if (getline(ss, name, ',') && getline(ss, segment, ',') && (maxScore = stoi(segment), true) && getline(ss, segment, ','))
			{
				levelsCompleted = stoi(segment);
				player loadedPlayer(name, maxScore, levelsCompleted);
				node<player>* newNode = new node<player>(loadedPlayer);
				newNode->setNext(head);
				head = newNode;
			}
		}
		loadPLayers.close();
		cout << "Datos de jugadores cargados en " << filename << endl;
	}

	void sortPlayersByScore()
	{
		if (head == nullptr || head->getNext() == nullptr) {
			return;
		}

		bool swapped;
		do {
			swapped = false;
			node<player>* current = head;

			while (current->getNext() != nullptr)
			{
				if (current->getData().getMaxScore() < current->getNext()->getData().getMaxScore())
				{
					player temp = current->getData();
					current->setData(current->getNext()->getData());
					current->getNext()->setData(temp);
					swapped = true;
				}
				current = current->getNext();
			}
		} while (swapped);
	}

	void topPlayers(int n, string names[], int scores[])
	{
		node<player>* current = head;

		for (int i = 0; i < n; i++)
		{
			if (current != nullptr)
			{
				names[i] = current->getData().getName();
				scores[i] = current->getData().getMaxScore();
				current = current->getNext();
			}
			else
			{
				names[i] = "";
				scores[i] = 0;
			}
		}
	}
};