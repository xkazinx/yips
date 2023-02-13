#pragma once

class Scene
{
public:
	Scene(SceneID id) {}

	virtual bool HandleEvents(sf::Event & event) = 0;
	virtual void Render(sf::RenderWindow & wnd, sf::Time delta_time) = 0;
	virtual void Tick(sf::Time delta_time) = 0;

	bool Is(SceneID id) const 
	{
		return _id == id;
	}

private:
	SceneID _id;
};