#pragma once
#include "DateFisier.h"

class File : public DateFisier
{
private:
	size_t size;
	enum class tip {TEXT, IMAGE, VIDEO, AUDIO, ZIP};
	tip tip_fisier;
public:
	File(const DateFisier&, int size, const tip);
	File(const char*, const char*, size_t , Utilizator*, size_t, tip);
	void open() const;
};