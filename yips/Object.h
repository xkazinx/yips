#pragma once

class Object
{
public: 
	Object() : _id(ObjectID::None) {}
	Object(ObjectID id) : _id(id) {
		PrintDebug("Object::Object({})", enum_name(_id));
	}

	virtual bool HandleEvents(sf::Event & event) { return false; }
	virtual void Render(sf::RenderTarget & wnd, sf::Time delta_time);
	virtual void Tick(sf::Time delta_time);

	virtual void Move(float x, float y);
	virtual Vector2f GetPosition();
	virtual void SetPosition(float x, float y);
	virtual void SetInitialPosition(float x, float y)
	{
		_initial_pos = { x, y };
		PrintDebug("SetInitialPosition({}, {})", x, y);
	}

	bool IsCollision(const Object * object) const
	{
		return GetCollisionRect().intersects(object->GetCollisionRect());
	}

	virtual sf::FloatRect GetCollisionRect() const {
		return {};
	};

	virtual void HandleCollision() {
		
	}

	template<typename T = Object>
	T * AddObject(unique_ptr<Object> && object)
	{
		_objects.emplace_back(std::move(object));
		return (T*)_objects.back().get();
	}

	vector<unique_ptr<Object>> _objects;

	ObjectID _id;
	Vector2f _pos, _initial_pos;

private:
	bool _expired = false;

public:
	bool HasExpired() const
	{
		return _expired;
	}

	virtual void Expire() 
	{
		_expired = true;
	}
};

