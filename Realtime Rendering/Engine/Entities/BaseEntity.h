#pragma once
#include "../Components/TransformComponent.h"

#include <glm/glm.hpp>

#include <CEGUI/CEGUI.h>

#include <vector>

//Base Entity, basis for all Entities
class BaseEntity {
public:
	BaseEntity(std::string name);
	~BaseEntity();

	void Update();
	void addComponent(BaseComponent* comp);
	void Init();

	TransformComponent * Transform;

	std::string Name;
	int ID;
	std::vector<BaseComponent*> components;
	CEGUI::ListboxTextItem * hierarchyItem;

	BaseComponent * getComponent(ComponentTypeEnum _componentType);	

	template <typename Derived>
	Derived* getElementOfType()
	{
		for (auto* component : components) {
			auto* p = dynamic_cast<Derived*>(component);
			if (p != nullptr) {
				return p;
			}
		}
		return nullptr;
	}

	template <typename Derived>
	Derived* getElementAtIndex()
	{
		for (auto* component : components) {
			auto* p = dynamic_cast<Derived*>(component);
			if (p != nullptr) {
				return p;
			}
		}
		return nullptr;
	}

private:
};

