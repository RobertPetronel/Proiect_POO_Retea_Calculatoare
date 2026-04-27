#include "director.h"
#include <cstring.h>
#include <string>

int Director::find(const DateFisier* f)
{
    for (int i = 0; i < copii.size(); i++)
        if (copii[i] == f)
            return i;

    return -1;
}

void Director::add(const DateFisier* f)
{
    copii.push_back(const_cast<DateFisier*>(f));
}

void Director::remove(const DateFisier* f)
{
    int pos = find(f);

    if (pos == -1)
        return;

    delete copii[pos];
    copii.erase(copii.begin() + pos);
}

std::vector<DateFisier*>::iterator
Director::find(const DateFisier* f) const
{
    for (auto it = copii.begin(); it != copii.end(); ++it)
        if (*it == f)
            return it;

    return copii.end();
}

Director* Director::find_subdirectory(const std::string nume) const
{
    for (audo e : copii)
        if (Director* dir = dynamic_cast<Director*>(e))
            if (dir->get_nume() == nume)
                return dir;
    return NULL
}

std::vector<DateFisier*>::iterator Director::begin() const
{
    return copii.begin();
}

std::vector<DateFisier*>::iterator Director::end() const
{
    return copii.end();
}

size_t Director::get_size() const override
{
    return 0;
}

Director::~Director()
{
    for (auto& f : copii)
        delete f;

    copii.clear();
}