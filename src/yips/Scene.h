#pragma once

class Scene
{
public:
	explicit Scene(const SceneID id) : _id(id) {}
	virtual ~Scene() = default;

	virtual bool HandleEvents(const sf::Event & event) = 0;
	virtual void Render(sf::RenderWindow & wnd, const sf::Time delta_time) = 0;
	virtual void Tick(const sf::Time delta_time) = 0;

	bool Is(const SceneID id) const
	{
		return _id == id;
	}

	SceneID Id() const
	{
		return _id;
	}

private:
	SceneID _id;
};
