#pragma once
#include "grupa.h"
#include <iostream>

class Utilizator
{
	char* nume;
	Grupa g;
public:
	Utilizator();
	Utilizator(const char*);
	Utilizator(const char*, const char*);
	Utilizator(const char*, const Grupa &);
	Utilizator(const Utilizator& u);
	Utilizator& operator=(const Utilizator&);
	const char* get_nume() const;
	void change_nume(char*);
	void change_grupa(const Grupa &);
	friend std::istream& operator>>(std::istream&, Utilizator&);
	friend std::ostream& operator<<(std::ostream&, const Utilizator&);
	bool operator==(const Utilizator&) const;
	bool operator!=(const Utilizator&) const;
	~Utilizator();
};