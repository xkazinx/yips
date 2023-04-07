#pragma once
#include "Object.h"

class Chunk;
class Background;
class Projectile;

class Scenario : public Object
{
public:
	static Scenario * Current;
	static constexpr auto kCount = 32u;

	Scenario(Scenario && scenario);
	Scenario(ChunkIdx chunk_cnt);
	void Initialize(ChunkIdx chunk_cnt);

	Scenario(const Json & json) { Load(json); }
	void Load(const Json & json);

	void Render(sf::RenderTarget & wnd, sf::Time delta_time) override;
	void Tick(sf::Time delta_time) override;
	void Move(float x, float y) override;

public:
	sf::View _view;

	bool IsOnSight(float x, float y) const {
		auto view_viewport	= _view.getViewport();
		//auto view_center		= _view.getCenter();
		//auto view_rotation	= _view.getRotation();
		auto view_size			= _view.getSize();

		// #test

		return x >= view_viewport.left && x <= view_viewport.left + view_size.x
			&& y >= view_viewport.top && y <= view_viewport.top + view_size.y;
	}
	
public:
	ChunkIdx _chunk_cnt;

	string _name;

public:
	sf::VertexArray _line;

public:
	vector<unique_ptr<Projectile>> _projectiles;

public:
	vector<Background*> _background_layers;
};

