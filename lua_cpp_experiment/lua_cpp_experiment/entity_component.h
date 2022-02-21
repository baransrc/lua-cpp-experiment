#pragma once

#include <iostream>

#include "sol.hpp"

// For simplicity we have three component types:
enum class ComponentType
{
	NONE,
	X,
	Y
};

void component_type_to_lua (sol::table& table)
{
	table.new_enum
	(
		"ComponentType",
		"NONE", ComponentType::NONE,
		"X", ComponentType::X,
		"Y", ComponentType::Y
	);
}

class Component;
class Entity;

// Here we have a simple version of Component
class Component
{
private:
	float position_x;
	float position_y;
	float position_z;
	Entity* owner;
public:
	Component() 
		: position_x(0.0f)
		, position_y(0.0f)
		, position_z(0.0f)
		, owner(nullptr)
	{};

	virtual ~Component() = default;

	virtual ComponentType GetType()
	{
		return ComponentType::NONE;
	}

	virtual void Initialize(Entity* new_owner)
	{
		owner = new_owner;
	}

	virtual void DoStuff()
	{
		std::cout << "Component::DoStuff" << std::endl;
	}

	void SetStuff(float new_position_x, float new_position_y, float new_position_z)
	{
		std::cout << "Component::SetStuff" << std::endl;

		position_x = new_position_x;
		position_y = new_position_y;
		position_z = new_position_z;
		
		std::cout << "x: " << position_x << " y: " << position_y << " z: " << position_z << std::endl;
	}
};

std::string component_lua_to_string(Component* component)
{
	return "Deneme";
}

void component_to_lua(sol::table& table)
{
	std::cout << "Component::ToLua" << std::endl;

	table.new_usertype<Component>
		(
			"Component",
			"Initialize", &Component::Initialize,
			"SetStuff", &Component::SetStuff,
			"DoStuff", &Component::DoStuff,
			"GetType", &Component::GetType,
			sol::meta_method::to_string, &component_lua_to_string
		);
}

class ComponentX : public Component
{
private:
	std::string x_value;

public:
	ComponentX() 
		: Component()
		, x_value("default x_value")
	{
	}

	~ComponentX() override
	{
	}

	void Initialize(Entity* new_owner) override
	{
		std::cout << "ComponentX::Initialize" << std::endl;

		Component::Initialize(new_owner);
	}

	virtual ComponentType GetType()
	{
		std::cout << "ComponentX::GetType" << std::endl;

		return ComponentType::X;
	}

	void SetXValue(std::string new_x_value)
	{
		std::cout << "ComponentX::SetXValue" << std::endl;

		x_value = new_x_value;
	}

	const std::string& GetXValue()
	{
		std::cout << "ComponentX::GetXValue" << std::endl;

		return x_value;
	}

	void DoStuff() override
	{
		std::cout << "ComponentX::DoStuff" << std::endl;
	}

	
};

void component_x_to_lua(sol::table& table)
{
	table.new_usertype<ComponentX>
		(
			"ComponentX",
			"Initialize", &ComponentX::Initialize,
			"SetStuff", &Component::SetStuff,
			"DoStuff", &ComponentX::DoStuff,
			"GetType", &ComponentX::GetType,
			"GetXValue", &ComponentX::GetXValue,
			"SetXValue", &ComponentX::SetXValue,
			sol::meta_method::to_string, [] {return "this should be visible in ide"; }
		);
}

class ComponentY : public Component
{
private:
	double y_value;

public:
	ComponentY()
		: Component()
		, y_value(0.0)
	{
	}

	~ComponentY() override
	{
	}

	void Initialize(Entity* new_owner) override
	{
		std::cout << "ComponentY::Initialize" << std::endl;

		Component::Initialize(new_owner);
	}		

	virtual ComponentType GetType()
	{
		std::cout << "ComponentY::GetType" << std::endl;

		return ComponentType::Y;
	}

	void SetYValue(double new_y_value)
	{
		std::cout << "ComponentX::SetYValue" << std::endl;

		y_value = new_y_value;
	}

	double GetYValue()
	{
		std::cout << "ComponentX::GetYValue" << std::endl;

		return y_value;
	}

	void DoStuff() override
	{
		std::cout << "ComponentX::DoStuff" << std::endl;
	}
};

void component_y_to_lua(sol::table& table)
{
	table.new_usertype<ComponentY>
		(
			"ComponentY",
			"Initialize", &ComponentY::Initialize,
			"SetStuff", &Component::SetStuff,
			"DoStuff", &ComponentY::DoStuff,
			"GetType", &ComponentY::GetType,
			"GetYValue", &ComponentY::GetYValue,
			"SetYValue", &ComponentY::SetYValue
		);
}

#define TYPE_IF_DERIVED_CLASS(BASE, DERIVED, TYPE) std::enable_if_t<std::is_base_of<BASE, DERIVED>::value, TYPE>
#define COMPONENT_VOID template <class COMPONENT_TYPE> TYPE_IF_DERIVED_CLASS(Component, COMPONENT_TYPE, void)
#define COMPONENT_PTR_CONST template <class COMPONENT_TYPE> TYPE_IF_DERIVED_CLASS(Component, COMPONENT_TYPE, COMPONENT_TYPE* const)
#define COMPONENT_SOL_OBJECT template <class COMPONENT_TYPE> TYPE_IF_DERIVED_CLASS(Component, COMPONENT_TYPE, sol::object)

// And here we have a simplified version of Entity
class Entity
{
private:
	std::vector<Component*> components;
public:
	int id;
	std::string name;

	Entity()
		: name("default_name")
		, id(0)
		
	{
	}

	COMPONENT_VOID AddComponent();

	COMPONENT_PTR_CONST GetComponent() const;

	Component* const GetComponent(ComponentType type)
	{
		for (Component* component : components)
		{
			if (component->GetType() == type)
			{
				std::cout << "Found the component" << std::endl;

				return component;
			}
		}

		std::cout << "Couldnt find the component" << std::endl;

		return nullptr;
	}

	std::vector<Component*>& GetComponents()
	{
		return components;
	}

	bool AddComponent(Component* component)
	{
		components.push_back(component);

		return true;
	}
};

void entity_to_lua(sol::table& table)
{
	// TODO: This seems so tedious. Sol does not allow template functions
	// to be sent to lua since template functions are not created until they
	// are instantiated. Find a better way or, use GetComponent by supplying
	// ComponentType as argument.

	table.new_usertype<Entity>
		(
			"Entity",
			"ID", &Entity::id,
			"AddComponentX", &Entity::AddComponent<ComponentX>,
			"GetComponentX", &Entity::GetComponent<ComponentX>,
			"AddComponentY", &Entity::AddComponent<ComponentY>,
			"GetComponentY", &Entity::GetComponent<ComponentY>,
			"GetComponents", &Entity::GetComponents
		);
}

COMPONENT_VOID Entity::AddComponent()
{
	COMPONENT_TYPE* component = new COMPONENT_TYPE();

	if (component == nullptr)
	{
		return;
	}

	bool added_successfully = AddComponent(component);

	if (!added_successfully)
	{
		delete component;
	}
	else
	{
		std::cout << "Added component successfully" << std::endl;
		
		component->Initialize(this);
	}
}

COMPONENT_PTR_CONST Entity::GetComponent() const
{
	ComponentType type = COMPONENT_TYPE().GetType();

	for (Component* component : components)
	{
		if (component->GetType() == type)
		{
			std::cout << "Found the Component" << std::endl;
			return (COMPONENT_TYPE*)component;
		}
	}

	return nullptr;
}