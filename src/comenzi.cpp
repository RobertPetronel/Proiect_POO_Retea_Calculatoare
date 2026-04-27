#include "calculator.h"
#include "comenzi.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class mkdir : public Executabil
{
public:
    virtual std::string run(const std::vector<std::string>& args) override
    {
        if (args.size() < 3)
            throw std::invalid_argument("Argumente insuficiente: trebuie sa specifici numele directorului.");

        const std::string dir_name = args[2];

        if (fs::exists(dir_name))
            throw std::runtime_error("Directorul deja exista.");

        try
        {
            fs::create_directory(dir_name);
            std::cout << "Directorul \"" << dir_name << "\" a fost creat cu succes." << "\n";
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Eroare la crearea directorului: " + std::string(e.what()));
        }
        return args[0];
    }

    virtual ~mkdir() = default;
};

class ls : public Executabil {
public:
    virtual std::string run(const std::vector<std::string>& args) override
    {
        fs::path dir = (args.size() > 2) ? args[2] : args[0];

        if (fs::exists(dir) && fs::is_directory(dir))
        {
            std::cout << "Listarea continutului directorului: " << dir << "\n";
            for (const auto& entry : fs::directory_iterator(dir))
                std::cout << entry.path().filename() << "\n";
        }
        else
        {
            std::cout << "Directorul nu exista: " << dir << "\n";
        }
        return args[0];
    }
    virtual ~ls() {}
};

class touch : public Executabil {
public:
    virtual std::string run(const std::vector<std::string>& args) override
    {
        if (args.size() < 3)
        {
            std::cout << "Nu a fost specificat niciun fisier.\n";
            return args[0];
        }

        fs::path file_path = args[2];

        if (fs::exists(file_path))
        {
            std::cout << "Fisierul deja exista, actualizare data.\n";
            fs::last_write_time(file_path, std::chrono::system_clock::now());
        }
        else
        {
            std::cout << "Creare fisier: " << file_path << "\n";
            std::ofstream(file_path);
        }
        return args[0];
    }
    virtual ~touch() {}
};

class rm : public Executabil {
public:
    virtual std::string run(const std::vector<std::string>& args) override
    {
        if (args.size() < 3)
        {
            std::cout << "Nu a fost specificat niciun fisier sau director.\n";
            return args[0];
        }

        fs::path target = args[2];

        if (fs::exists(target))
        {
            if (fs::is_directory(target))
            {
                std::cout << "Stergere director: " << target << "\n";
                fs::remove_all(target);
            }
            else
            {
                std::cout << "Stergere fisier: " << target << "\n";
                fs::remove(target);
            }
        }
        else
            std::cout << "Fisierul sau directorul nu exista: " << target << "\n";
        return args[0];
    }
    virtual ~rm() {}
};

class mv : public Executabil {
public:
    virtual std::string run(const std::vector<std::string>& args) override
    {
        if (args.size() < 3)
        {
            std::cout << "Trebuie să specifici atat sursa cat și destinatia.\n";
            return args[0];
        }

        fs::path source = args[2];
        fs::path destination = args[3];

        if (fs::exists(source))
        {
            std::cout << "Mutare/Redenumire fisier: " << source << " -> " << destination << "\n";
            fs::rename(source, destination);
        }
        else
            std::cout << "Fisierul sursa nu exista: " << source << "\n";

        return args[0];
    }
    virtual ~mv() {}
};

class cd : public Executabil {
public:
    virtual std::string run(const std::vector<std::string>& args) override
    {
        if (args.size() < 3)
        {
            std::cout << "Trebuie să specifici un director!\n";
            return args[0];
        }

        std::string new_dir = args[2];
        if (fs::exists(new_dir))
        {
            std::cout << "Ai schimbat directorul curent in: " << new_dir << "\n";
            return args[2];
        }
        else
            std::cout << "Nu am putut schimba directorul!\n";

        return args[0];
    }
    virtual ~cd() {}
};