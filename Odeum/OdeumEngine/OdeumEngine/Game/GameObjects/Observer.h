#ifndef OBSERVER_H
#define OBSERVER_H

#include "../../pch.h"
#include "../../Engine/Core/GameObject.h"
#include "../../Engine/Core/OdeumEngine.h"

enum class GameEventType : uint8_t
{
	None = 0,
	GameMove, GamePurchase, GameTriggerTile,
	GameAttack, GameDefend
};

enum GameEventCategory : uint8_t
{
	GameMap, GameCombat
};

// macro so we don't have to retype this in every derived event, thanks cherno
#define GAME_EVENT_TYPE(type) static GameEventType GetStaticType() { return GameEventType::type; }\
														virtual GameEventType GetEventType() const override { return GetStaticType(); } \
														virtual const char* GetName() const override { return #type; }

// same here
#define GAME_EVENT_CATEGORY(category) virtual uint32_t GetCategoryFlags() const override { return category; }

class GameEvent
{
public:

	virtual ~GameEvent() = default;
	bool handled = false;

	virtual GameEventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual uint32_t GetCategoryFlags() const = 0;
	virtual std::string ToString() const { return GetName(); }

	bool isInCategory(GameEventCategory category_)
	{
		return GetCategoryFlags() & category_;
	}
};

class GameMapEvent : public GameEvent
{
public:
	GAME_EVENT_CATEGORY(GameMap)
protected:
	GameMapEvent() {}
};

class GameMoveEvent : public GameMapEvent
{
public:
	
	GameMoveEvent(GameObject* Target, Vector2 Move) : GameMapEvent() { target = Target; move = Move; }

	Vector2 Distance() { return move; }

	GAME_EVENT_TYPE(GameMove)
protected:
	Vector2 move;
	GameObject* target;
};

class Observer
{
public:
	virtual ~Observer() {}
	virtual void OnNotify(GameObject& GameObject, GameEvent& Event) = 0;
};

class Subject
{
public:
	void AddObserver(Observer* Obs)
	{
		m_observers.push_back(Obs);
	}

	void RemoveObserver(Observer* Obs)
	{
		auto it = std::find(m_observers.begin(), m_observers.end(), Obs);
		if (it != m_observers.end())
			m_observers.erase(it);
	}

protected:
	virtual void Notify(GameObject& GameObject, GameEvent& Event)
	{
		for (auto observer : m_observers)
			observer->OnNotify(GameObject, Event);
	}

private:
	std::vector<Observer*> m_observers;
};

#endif