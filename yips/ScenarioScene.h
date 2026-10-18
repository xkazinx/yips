#pragma once
#include "Scene.h"
#include "Object.h"

class ObjectShip1;
class Scenario;

class ScenarioScene : public Scene, public Object
{
public:
	ScenarioScene();

	void Initialize();

	bool HandleEvents(sf::Event & event) override;
	void Render(sf::RenderWindow& wnd, sf::Time delta_time) override;
	void Tick(sf::Time delta_time) override;

	unique_ptr<Scenario> _scenario;

	ObjectShip1 * _ship;

	// #section #items
	ObstacleType _last_killed_obstacle_type = ObstacleType::None;
	u8 _last_killed_obstacle_cnt = 0;

	// #section Abilities
	u16 _total_energies = 0;
	bool UseAbility(sf::Keyboard::Key key);
	void ChargeAbilityLevel(sf::Keyboard::Key key);
	void ChargeAbilityAmmo(PowerUpID id, u16 ammo);

	static AbilityID KeyToAbility(sf::Keyboard::Key key);
	static AbilityID PowerUpToAbility(PowerUpID id);
	std::array<u8, enum_count<AbilityID>()> _ability_levels;
	std::array<u16, enum_count<AbilityID>()> _ability_ammo;

};

// When you receive an energy, you set it to a number from 1 to 8 (analog keys)

