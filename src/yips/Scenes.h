#pragma once
#include "Scene.h"

class Scenes : public Scene
{
public:
	Scenes();

	bool HandleEvents(const sf::Event & event) override;
	void Render(sf::RenderWindow & wnd, const sf::Time delta_time) override;
	void Tick(const sf::Time delta_time) override;

	// Changes the scene at the start of the next frame: a scene that asks for it
	// (a key, an editor button) keeps running to the end of its own call
	void Set(const SceneID id);
	// The Scenario scene on this scenario (the editor's Play)
	void Play(Json scenario);
	void ApplyPending();

	SceneID Current() const { return _ptr ? _ptr->Id() : SceneID::None; }

	template<typename T>
	T * Get()
	{
		return dynamic_cast<T *>(_ptr.get());
	}

private:
	unique_ptr<Scene> _ptr;
	SceneID _pending = SceneID::None;
	optional<Json> _pending_scenario;
};
