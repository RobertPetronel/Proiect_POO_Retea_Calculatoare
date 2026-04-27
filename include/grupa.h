#pragma once
#include <iostream>

class Grupa
{
	char* nume;
public:
	// constructori
	Grupa();
	Grupa(const char*);
	// constructorul de copiere
	Grupa(const Grupa&);
	// operatorul de asignare
	Grupa& operator=(const Grupa&);
	// operatorii de citire si afisare
	friend std::ostream& operator<< (std::ostream&, const Grupa& g);
	friend std::istream& operator>> (std::istream&, Grupa& g);
	bool operator==(const Grupa& g) const;
	bool operator!=(const Grupa& g) const;
	// getter
	const char* get_nume() const;
	// setter
	void change_nume(char*);
	// destructor
	~Grupa();
};