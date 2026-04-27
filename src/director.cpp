#include "director.h"
#include <cstring>
#include <string>

Director::Director() : DateFisier(), copii() {}

int Director::find_poz(const DateFisier* f)
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
    int pos = find_poz(f);

    if (pos == -1)
        return;

    delete copii[pos];
    copii.erase(copii.begin() + pos);
}

Director* Director::find_subdirectory(const std::string& nume) const
{
    for (auto e : copii)
        if (Director* dir = dynamic_cast<Director*>(e))
            if (dir->get_nume() == nume)
                return dir;
    return NULL;
}

std::vector<DateFisier*>::iterator
Director::find(const DateFisier* f)
{
    for (auto it = copii.begin(); it != copii.end(); ++it)
        if (*it == f)
            return it;

    return copii.end();
}

std::vector<DateFisier*>::iterator Director::begin()
{
    return copii.begin();
}

std::vector<DateFisier*>::iterator Director::end()
{
    return copii.end();
}

size_t Director::get_size() const
{
    return 0;
}

Director::~Director()
{
    for (auto& f : copii)
        delete f;

    copii.clear();
}