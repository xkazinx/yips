#pragma once
#include "Scene.h"

class Scenes : public Scene
{
public:
	Scenes();

	bool HandleEvents(sf::Event & event) override;
	void Render(sf::RenderWindow & wnd, sf::Time delta_time) override;
	void Tick(sf::Time delta_time) override;

	void Set(SceneID id);

	template<typename T> 
	T * Get()
	{
		return (T*)_ptr.get();
	}

private:
	unique_ptr<Scene> _ptr;
};

