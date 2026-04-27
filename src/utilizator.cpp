#include <cstring.h>
#include "grupa.h"

Utilizator::Utilizator() : nume(NULL), g() {}
Utilizator::Utilizator(char* nume) : g()
{
	if (!nume) {this->nume = NULL; return;}
	this->nume = new char[1+strlen(nume)];
	strcpy(this->nume, nume);
}
Utilizator::Utilizator(const char* nume, const char* nume_grup) : g(nume_grup)
{
	if (!nume) {this->nume = NULL; return;}
	this->nume = new char[1+strlen(nume)];
	strcpy(this->nume, nume);
}
Utilizator::Utilizator(char* nume, const Grupa& grupa) : g(grupa)
{
	this->nume = new char[1+strlen(nume)];
	strcpy(this->nume, nume);
}
Utilizator::Utilizator(const Utilizator& u) : g(u.g)
{
	nume = new char[1+strlen(u.nume)];
	strcpy(nume, u.nume);
}
Utilizator& Utilizator::operator=(const Utilizator& u)
{
	if (this != &u) return *this;
	if (nume != NULL) delete[] nume;
	nume = new char[1+strlen(u.nume)];
	strcpy(nume, u.nume);
	g = u.g;
	return *this;
}
const char* Utilizator::get_nume() const
{
	return nume;
}
void Utilizator::change_nume(char* nume)
{
	if (this->nume != NULL) delete[] this->nume;
	this->nume = new char[1+strlen(nume)];
	strcpy(this->nume, nume);
}
void Utilizator::change_grupa(const Grupa& g)
{
	this->g = g;
}
bool Utilizator::operator==(const Utilizator& u) const
{
	return strcmp(nume, u.nume) == 0 && g == u.g;
}
bool Utilizator::operator!=(const Utilizator& u) const
{
	return !(*this == u);
}
Utilizator::~Utilizator()
{
	if (nume != NULL) delete[] nume;
}