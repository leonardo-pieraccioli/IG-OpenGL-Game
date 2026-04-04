#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class LeaderboardEntry
{
public:
	LeaderboardEntry(std::string name, int score, int round) : name(name), score(score), round(round) {}
	std::string name;
	int score;
	int round;
};

class Leaderboard
{
public:
	Leaderboard() {
		isLoaded = false;
		LoadLeaderboard();
	};
	~Leaderboard() {
		if (leaderboardFile.is_open())
		{
			leaderboardFile.close();
		}
	};

	void AddScore(std::string name, int score, int round)
	{
		if (!isLoaded)
		{
			LoadLeaderboard();
		}
		if (find_if(leaderboard.begin(), leaderboard.end(), [name, score, round](LeaderboardEntry a) {
			return a.name == name && a.round == round && a.score == score;
			}) == leaderboard.end())
		{
			leaderboard.push_back(LeaderboardEntry(name, score, round));
			std::cout << "Score added: " << name << " " << score << " " << round << std::endl;
			sort(leaderboard.begin(), leaderboard.end(), [](LeaderboardEntry a, LeaderboardEntry b) { 
				if (a.score == b.score)
					return a.round > b.round;
				else
					return a.score > b.score;
			});
			SaveLeaderboard();
		}
		else 
		{
			std::cout << "Score already exists: " << name << " " << score << " " << round << std::endl;
		}
	}
	std::vector<LeaderboardEntry> GetLeaderboard()
	{
		std::vector<LeaderboardEntry> bestTen;
		if (leaderboard.empty())
		{
			LoadLeaderboard();
		}
		if (leaderboard.size() > 10)
		{
			for (int i = 0; i < 10; i++)
			{
				bestTen.push_back(leaderboard[i]);
			}
		}
		else
		{
			bestTen = leaderboard;
		}
		return bestTen;
	}
	void SaveLeaderboard()
	{
		if (!leaderboardFile.is_open())
		{
			leaderboardFile.open("leaderboard.txt", std::ios::out);
		}

		if (leaderboardFile.is_open())
		{
			for (auto entry : leaderboard)
			{
				leaderboardFile << entry.name << " " << entry.score << " " << entry.round << std::endl;
			}
			leaderboardFile.close();
			std::cout << "Leaderboard saved" << std::endl;
		}
		else
		{
			std::cout << "Unable to open file" << std::endl;
		}
	}
	void LoadLeaderboard()
	{
		if (!leaderboardFile.is_open())
		{
			leaderboardFile.open("leaderboard.txt", std::ios::in);
			std::cout << "Leaderboard file opened" << std::endl;
		}

		if (leaderboardFile.is_open())
		{
			std::string name;
			int score;
			int round;
			leaderboard.clear();
			while (leaderboardFile >> name >> score >> round)
			{
				if (find_if(leaderboard.begin(), leaderboard.end(), [name, score, round](LeaderboardEntry a) { 
						return a.name == name && a.round == round && a.score == score; 
					}) != leaderboard.end())
				{
					std::cout << "Duplicate entry found: " << name << " " << score << " " << round << std::endl;
					continue;

				}
				leaderboard.push_back(LeaderboardEntry(name, score, round));
				std::cout << "Leaderboard entry loaded: " << name << " " << score << " " << round << std::endl;
			}
			sort(leaderboard.begin(), leaderboard.end(), [](LeaderboardEntry a, LeaderboardEntry b) { return a.score > b.score; });
			isLoaded = true;
			leaderboardFile.close();
			std::cout << "Leaderboard loaded" << std::endl;
		}
		else
		{
			std::cout << "Unable to open file" << std::endl;
		}
	}

private:
	bool isLoaded;
	std::fstream leaderboardFile;
	std::vector<LeaderboardEntry> leaderboard;
};
