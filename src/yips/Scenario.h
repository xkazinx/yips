#pragma once
#include "Object.h"

class Projectile;

class Scenario : public Object
{
public:
	static constexpr auto kCount = 32u;
	// A projectile this far from the view's middle has left the screen for good
	static constexpr float kProjectileReach = kViewSizeY * 1.5f;

	explicit Scenario(const ChunkIdx chunk_cnt);
	explicit Scenario(const Json & json);
	// In the .cpp: _projectiles' Projectile is only forward-declared here
	~Scenario() override;

	void Initialize(const ChunkIdx chunk_cnt);
	void Load(const Json & json);
	// The scenario as its file holds it: name, chunks, the placed obstacles
	Json ToJson() const;

	void Render(sf::RenderTarget & wnd, const sf::Time delta_time) override;
	void Tick(const sf::Time delta_time) override;
	void Move(const float x, const float y) override;

	// Added after this frame's collisions (an obstacle's power up)
	void Spawn(unique_ptr<Object> && object);

	// Objects other than the background (what a test counts)
	szt CountObjects(const ObjectID id) const;

public:
	sf::View _view;
	bool _debug_overlay = true;	// F3: the view and mouse numbers

	ChunkIdx _chunk_cnt = 0;
	string _name;

	vector<unique_ptr<Projectile>> _projectiles;
	vector<unique_ptr<Object>> _spawned;
};
