#pragma once

#include <GameEngine/Components/ComponentsIncludeList.h>
#include <memory>

class ComponentFactory
{
	public:
		BaseGameObject* createObject(int id);

		ComponentFactory(ComponentFactory const&) = delete;
		void operator=(ComponentFactory const&) = delete;

		static ComponentFactory& GetInstance() {
			static ComponentFactory instance;
			return instance;
		}

	private:
		int globalObjectId;
		ComponentFactory() : globalObjectId(0) {};
};