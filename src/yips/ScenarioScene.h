#pragma once
#include "Scene.h"
#include "Obstacle.h"

class ObjectShip1;
class Scenario;

class ScenarioScene : public Scene
{
public:
	static constexpr ChunkIdx kChunkCount = 16;

	ScenarioScene();
	explicit ScenarioScene(const Json & scenario);	// the editor's Play
	~ScenarioScene() override;

	void Initialize();

	bool HandleEvents(const sf::Event & event) override;
	void Render(sf::RenderWindow & wnd, const sf::Time delta_time) override;
	void Tick(const sf::Time delta_time) override;

	// O / L: an obstacle at a random x on the top of the screen, moving in waves / in a line
	void AddObstacle(const MoveType move);

	unique_ptr<Scenario> _scenario;

	ObjectShip1 * _ship = nullptr;

	// #section #items
	ObstacleType _last_destroyed_obstacle_type = ObstacleType::None;
	u8 _last_destroyed_obstacle_cnt = 0;

	// Counters (the HUD's and a test's)
	u32 _shots = 0;
	u32 _destroyed_obstacles = 0;
	u32 _power_ups_taken = 0;

	// #section Abilities
	u16 _total_energies = 0;
	bool UseAbility(const sf::Keyboard::Key key);
	void ChargeAbilityLevel(const sf::Keyboard::Key key);
	void ChargeAbilityAmmo(const PowerUpID id, const u16 ammo);

	static AbilityID KeyToAbility(const sf::Keyboard::Key key);
	static AbilityID PowerUpToAbility(const PowerUpID id);
	array<u8, kAbilityCount> _ability_levels{};
	array<u16, kAbilityCount> _ability_ammo{};
};

// When you receive an energy, you set it to a number from 1 to 8 (analog keys)
