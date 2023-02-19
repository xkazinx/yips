#pragma once

class Object
{
private:
	using ID = ObjectID;

public:
	Object() : _id(ID::None) {}
	Object(ID id) : _id(id) {
		PrintDebug("Object::Object({})", enum_name(_id));
	}

	// For projectile
	Object(ID id, Vector2f origin) : Object(id) {
		SetPosition(origin.x, origin.y);
	}

protected:
	virtual bool HandleEvents(sf::Event & event) { return false; }
	virtual void Render(sf::RenderTarget & wnd, sf::Time delta_time);
	virtual void Tick(sf::Time delta_time);

public:
	virtual void Move(float x, float y);
	virtual Vector2f GetPosition();
	virtual void SetPosition(float x, float y);

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
	T * AddObject(unique_ptr<T> && object)
	{
		_objects.emplace_back(std::move(object));
		return (T*)_objects.back().get();
	}

	vector<unique_ptr<Object>> _objects;

	ObjectID _id;
	Vector2f _pos, _initial_pos = {-1.f, -1.f};

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

	bool IsSelectable() const 
	{
		return _id == ObjectID::Obstacle;
	}

	bool _selected = false;
};

