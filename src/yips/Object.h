#pragma once

class Object
{
private:
	using ID = ObjectID;

public:
	Object() : _id(ID::None) {}
	explicit Object(const ID id) : _id(id) {}

	// For projectile
	Object(const ID id, const Vector2f origin) : Object(id)
	{
		_pos = origin;
	}

	// Deleted through unique_ptr<Object>: the derived parts (shapes, textures) go too
	virtual ~Object() = default;

	virtual bool HandleEvents(const sf::Event & event) { return false; }
	virtual void Render(sf::RenderTarget & wnd, const sf::Time delta_time);
	virtual void Tick(const sf::Time delta_time);

	virtual void Move(const float x, const float y);
	virtual Vector2f GetPosition();
	virtual void SetPosition(const float x, const float y);

	bool IsCollision(const Object * object) const
	{
		return GetCollisionRect().findIntersection(object->GetCollisionRect()).has_value();
	}

	virtual sf::FloatRect GetCollisionRect() const
	{
		return {};
	}

	virtual void HandleCollision(Object * obj) {}

	template<typename T = Object>
	T * AddObject(unique_ptr<T> && object)
	{
		T * const p = object.get();
		_objects.emplace_back(std::move(object));
		return p;
	}

	vector<unique_ptr<Object>> _objects;

	ObjectID _id;
	Vector2f _pos, _initial_pos = { -1.f, -1.f };

	template<typename T>
	T * Get()
	{
		return static_cast<T *>(this);
	}

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
