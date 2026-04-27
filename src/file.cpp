#include "file.h"

File::File(const DateFisier& df, int size, const tip t)
    : DateFisier(df)
{
    this->size = size;
}

File::File(const char* nume,
           const char* path,
           size_t data_crearii,
           Utilizator* creator,
           size_t size,
           tip t)
    : DateFisier(nume, path, data_crearii, creator)
{
    this->size = size;
}

void File::open() const
{
    switch (tip_fisier)
    {
    case tip::TEXT:
        std::cout << "Fisierul este gol";
        break;
    case tip::IMAGE:
        throw "Nu exista o metoda de a deschide imaginea";
        break;
    case tip::VIDEO:
        throw "Nu exista o metoda de a deschide video-ul";
        break;
    case tip::AUDIO:
        throw "Nu exista o metoda de a reda audio-ul";
        break;
    case tip::ZIP:
        throw "Nu exista o metoda de a dezarhiva";
        break;
    }
}

size_t File::get_size() const
{
    return size;
}