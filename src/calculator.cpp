#include "date_fisier.h"
#include "comenzi.h"
#include "calculator.h"
#include "director.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <filesystem>


calculator::calculator()
    : root(), root_path(std::filesystem::current_path().string() + "/root"), pwd("/"), curent_user_id(-1)
{
    Utilizator root_user("root", "admin");
    users[users.size()] = root_user;
    user_activ.push_back(true);

    root = Director("root", "root", static_cast<size_t>(std::chrono::duration_cast<std::chrono::seconds>
        (std::chrono::system_clock::now().time_since_epoch()).count()), &users[0]);
    std::filesystem::create_directory(root_path);

    comenzi["mkdir"] = new mkdir();
    comenzi["ls"] = new ls();
    comenzi["cd"] = new cd();
    comenzi["rm"] = new rm();
    comenzi["touch"] = new touch();
}

Director* calculator::get_directory(const std::string& path)
{
    if (path == "/")
        return &root;

    Director* current_dir = &root;
    size_t pos = 0;
    size_t next_pos = path.find("/");

    while (next_pos != std::string::npos)
    {
        std::string dir_name = path.substr(pos, next_pos - pos);
        current_dir = current_dir->find_subdirectory(dir_name);
        
        if (current_dir == nullptr)
        {
            std::cout << "Directorul " << dir_name << " nu a fost găsit!" << "\n";
            return nullptr;
        }

        pos = next_pos + 1;
        next_pos = path.find("/", pos);
    }

    std::string final_dir = path.substr(pos);
    current_dir = current_dir->find_subdirectory(final_dir);

    if (current_dir == nullptr)
    {
        std::cout << "Directorul final " << final_dir << " nu a fost găsit!" << "\n";
        return nullptr;
    }

    return current_dir;
}

void calculator::login(const char* nume)
{
    for (int i = 0; i < users.size(); i++)
        if (strcmp(users[i].get_nume(), nume) == 0)
        {
            curent_user_id = i;
            std::cout << "Logare cu success\n";
            return;
        }
    throw std::invalid_argument("Logare esuata, nu exista utilizator cu acel nume");
}
void calculator::create_user(const char* nume, const char* grupa)
{
    users[users.size()] = Utilizator(nume, grupa);
    user_activ.push_back(true);
}

void calculator::run()
{
    std::string command_line;
    std::cout << "Bine ai venit! Introdu comenzi. Tipul 'exit' pentru a incheia.\n";
    while (true)
    {
        std::cout << this->pwd << "> ";
        std::getline(std::cin, command_line);

        std::transform(command_line.begin(), command_line.end(), command_line.begin(), ::tolower);

        if (command_line == "exit")
        {
            std::cout << "Iesire din aplicatie...\n";
            break;
        }

        std::istringstream iss(command_line);
        std::string command;
        std::vector<std::string> args;
        args.push_back(root_path + pwd);
        
        iss >> command;
        args.push_back(command);
        std::string arg;
        while (iss >> arg)
            args.push_back(arg);
        if (comenzi.find(command) != comenzi.end())
            pwd = comenzi[command]->run(args);
        else 
            std::cout << "Comanda nu este recunoscuta: " << command << "\n";
    }
}

calculator::~calculator()
{
    for (auto& cmd : comenzi) {
        delete cmd.second;
    }
}