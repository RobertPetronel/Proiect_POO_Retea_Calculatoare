#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "director.h"

class calculator
{
	static const int root_user, root_grup;
	std::unordered_map<int, Utilizator> users;
	std::vector<bool> user_activ;
	std::unordered_map<std::string, Executabil*> comenzi;
	Director root;
	std::string pwd;
	int curent_user_id;
public:
	calculator();
	void login(const char*);
	void create_user(const char*, const char*);
	void run();
    Director* get_directory(const std::string& path);
	~calculator();
};