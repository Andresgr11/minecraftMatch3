#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class player
{
private:
	string name;
	int maxScore;
	int levelsCompleted;
public:
	player(string playerName, int score, int levels) : name(playerName), maxScore(score), levelsCompleted(levels) {}
	~player() {}
	string getName() const { return name; }
	int getMaxScore() const { return maxScore; }
	int getLevelsCompleted() const { return levelsCompleted; }
	void setName(string playerName) { name = playerName; }
	void setMaxScore(int score) { maxScore = score; }
	void setLevelsCompleted(int levels) { levelsCompleted = levels; }
};

