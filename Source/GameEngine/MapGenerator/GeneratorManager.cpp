#include "GeneratorManager.h"
#include "Generator.h"

void GeneratorManager::SetGenerator(Generator* generator) {
	this->generator = generator;
}
Generator* GeneratorManager::GetGenerator() {
	return this->generator;
}