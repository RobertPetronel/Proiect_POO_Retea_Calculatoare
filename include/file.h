#pragma once
#include "date_fisier.h"

class File : public DateFisier
{
public:
	enum class tip {TEXT, IMAGE, VIDEO, AUDIO, ZIP};
	File(const DateFisier&, int size, const tip);
	File(const char*, const char*, size_t , const Utilizator*, size_t, tip);
	size_t get_size() const override;
	void open() const;
private:
	size_t size;
	tip tip_fisier;
};