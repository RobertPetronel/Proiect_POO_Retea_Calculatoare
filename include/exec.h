#pragma once
#include <vector>
#include <string>

class Executabil
{
public:
	virtual std::string run(const std::vector<std::string>& args) = 0;
	virtual ~Executabil() = default;
};