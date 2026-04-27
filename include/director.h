#pragma once
#include <vector>
#include "date_fisier.h"
#include <string>

class Director : public DateFisier
{
private:
	std::vector<DateFisier *> copii;
	int find(const DateFisier*);
public:
	Director();
	Director(const char* nume, const char* path, const size_t data_crearii, const Utilizator* creator) :
		DateFisier(nume, path, data_crearii, creator) {}
	Director(const DateFisier& t) : DateFisier(t) {}
	size_t get_size() const override;
	void add(const DateFisier*);
	void remove(const DateFisier*);
	Director* find_subdirectory(const std::string&) const;
	std::vector<DateFisier *>::iterator find(const DateFisier*) const;
	std::vector<DateFisier *>::iterator begin() const;
	std::vector<DateFisier *>::iterator end() const;
	~Director();
};