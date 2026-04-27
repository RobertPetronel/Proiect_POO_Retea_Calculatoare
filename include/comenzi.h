#pragma once
#include <vector>
#include <string>
#include "exec.h"

class mkdir : public Executabil
{
public:
	virtual std::string run(const std::vector<std::string>& args);
	virtual ~mkdir();
};

class ls : public Executabil
{
public:
	virtual std::string run(const std::vector<std::string>& args);
	virtual ~ls();
};

class touch : public Executabil
{
public:
	virtual std::string run(const std::vector<std::string>& args);
	virtual ~touch();
};

class rm : public Executabil
{
public:
	virtual std::string run(const std::vector<std::string>& args);
	virtual ~rm();
};

class mv : public Executabil
{
public:
	virtual std::string run(const std::vector<std::string>& args);
	virtual ~mv();
};

class cd : public Executabil
{
public:
	virtual std::string run(const std::vector<std::string>& args);
	virtual ~cd();
};