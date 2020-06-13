#pragma once

class Generator;

class GeneratorManager
{
public:
	GeneratorManager(GeneratorManager const&) = delete;
	void operator=(GeneratorManager const&) = delete;

	static GeneratorManager& GetInstance()
	{
		static GeneratorManager instance;
		return instance;
	}

	void SetGenerator(Generator* generator);
	Generator* GetGenerator();

private:
	Generator* generator;
	GeneratorManager() {};

};
