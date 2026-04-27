#include <cstring.h>
#include "grupa.h"

Grupa::Grupa() : nume(NULL) {}
Grupa::Grupa(char* nume)
{
	if (!nume) {this->nume = NULL; return;}
	this->nume = new char[1+strlen(nume)];
	strcpy(this->nume, nume); 
}
Grupa::Grupa(const Grupa& g)
{
	nume = new char[1+strlen(g.nume)];
	strcpy(nume, g.nume);
}
const char* Grupa::get_nume() const
{
	return nume;
}
void Grupa::change_nume(char* nume)
{
	if (this->nume != NULL) delete[] this->nume;
	this->nume = new char[1+strlen(nume)];
	strcpy(this->nume, nume);
}
Grupa& Grupa::operator=(const Grupa& g)
{
	if (this == &g) return *this;
	delete[] nume;
	nume = new char[1+strlen(g.nume)];
	strcpy(nume, g.nume);
	return *this;
}
bool Grupa::operator== (const Grupa& g) const
{
	return strcmp(nume, g.nume) == 0;
}
bool Grupa::operator!= (const Grupa& g) const
{
	return strcmp(nume, g.nume) != 0;
}
Grupa::~Grupa()
{
	if (nume != NULL) delete[] nume;
}