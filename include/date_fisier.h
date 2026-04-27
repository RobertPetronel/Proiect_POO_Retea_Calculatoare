#pragma once
#include "utilizator.h"

class DateFisier
{
private:
	char* nume;
	char* path;
	size_t data_crearii;
	Utilizator* creator;
public:
	DateFisier() : nume(NULL), path(NULL), data_crearii(0), creator(NULL) {}
	DateFisier(const char*, const char*, const size_t, const Utilizator*&);
	DateFisier(const DateFisier&);
	DateFisier& operator=(const DateFisier&);
	void rename(const char*);
	const char* get_path() const;
	const char* get_nume() const;
	const size_t get_data_crearii() const;
	Utilizator* get_creator() const;
	virtual size_t get_size() const = 0;
	virtual ~DateFisier();
};