#include <cstring.h>
#include "grupa.h"

DateFisier::DateFisier(const char* nume,
                       const char* path,
                       const size_t data_crearii,
                       const Utilizator*& creator)
{
    this->nume = new char[strlen(nume) + 1];
    strcpy(this->nume, nume);

    this->path = new char[strlen(path) + 1];
    strcpy(this->path, path);

    this->data_crearii = data_crearii;

    this->creator = const_cast<Utilizator*>(creator);
}
DateFisier::DateFisier(const DateFisier& df)
{
    nume = new char[strlen(df.nume) + 1];
    strcpy(nume, df.nume);

    path = new char[strlen(df.path) + 1];
    strcpy(path, df.path);

    data_crearii = df.data_crearii;

    creator = df.creator;
}

DateFisier& DateFisier::operator=(const DateFisier& df)
{
    if (this == &df)
        return *this;

    delete[] nume;
    delete[] path;

    nume = new char[strlen(df.nume) + 1];
    strcpy(nume, df.nume);

    path = new char[strlen(df.path) + 1];
    strcpy(path, df.path);

    data_crearii = df.data_crearii;
    creator = df.creator;

    return *this;
}

void DateFisier::rename(const char* new_name)
{
    delete[] nume;

    nume = new char[strlen(new_name) + 1];
    strcpy(nume, new_name);
}

const char* DateFisier::get_path() const
{
    return path;
}

const char* DateFisier::get_nume() const
{
    return nume;
}

const size_t DateFisier::get_data_crearii() const
{
    return data_crearii;
}

Utilizator* DateFisier::get_creator() const
{
    return creator;
}

DateFisier::~DateFisier()
{
    delete[] nume;
    delete[] path;
}