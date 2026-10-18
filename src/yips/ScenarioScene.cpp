#include "pch.h"
#include "ScenarioScene.h"
#include "ObjectShip1.h"
#include "Scenario.h"
#include "BallProjectile.h"
#include "BallObstacle.h"
#include "Random.h"
#include "App.h"
#include "Input.h"

ScenarioScene::ScenarioScene() : Scene(SceneID::Scenario)
{
	_scenario = make_unique<Scenario>(kChunkCount);
	Initialize();
}

ScenarioScene::ScenarioScene(const Json & scenario) : Scene(SceneID::Scenario)
{
	_scenario = make_unique<Scenario>(scenario);
	Initialize();
}

ScenarioScene::~ScenarioScene() = default;

void ScenarioScene::Initialize()
{
	_ship = _scenario->AddObject(make_unique<ObjectShip1>());

	_ability_levels.fill(0);
	_ability_ammo.fill(0);

	_total_energies = 1;
	ChargeAbilityLevel(sf::Keyboard::Key::A);

	ChargeAbilityAmmo(PowerUpID::Ammo_Ability5, 99u);
}

void ScenarioScene::AddObstacle(const MoveType move)
{
	auto obs = _scenario->AddObject(make_unique<BallObstacle>(Vector2f{ (float)g_rand->Generate<u16>(0u, kViewSizeX), _scenario->_view.getCenter().y + kViewSizeY / 2.f }));
	obs->_velocity_x = 10.f;
	obs->_velocity_y = 0.f;
	obs->_move_type = move;
}

bool ScenarioScene::HandleEvents(const sf::Event & event)
{
	const auto key = event.getIf<sf::Event::KeyPressed>();
	if (!key)
		return false;

	using Key = sf::Keyboard::Key;

	switch (key->code)
	{
	case Key::O:
		AddObstacle(MoveType::Wave);
		return true;

	case Key::L:
		AddObstacle(MoveType::Linear);
		return true;

	case Key::F3:
		_scenario->_debug_overlay = !_scenario->_debug_overlay;
		return true;

	case Key::Q:
	case Key::W:
	case Key::E:
	case Key::R:
	case Key::A:
	case Key::S:
	case Key::D:
	case Key::F:
		if (key->control || input::IsCtrl())
			ChargeAbilityLevel(key->code);
		else
			UseAbility(key->code);
		return true;

	default:
		break;
	}

	return false;
}

bool ScenarioScene::UseAbility(const sf::Keyboard::Key key)
{
	const auto ability = KeyToAbility(key);
	if (ability == AbilityID::None || _ability_ammo[Under(ability)] == 0)
		return false;

	--_ability_ammo[Under(ability)];

	switch (ability)
	{
	case AbilityID::Ability5:
		_scenario->_projectiles += make_unique<BallProjectile>(_ship->Muzzle(BallProjectile::kRadius));
		++_shots;
		return true;

	// #todo the other abilities: their ammo is spent, nothing happens yet
	default:
		return true;
	}
}

void ScenarioScene::ChargeAbilityLevel(const sf::Keyboard::Key key)
{
	const auto ability = KeyToAbility(key);
	if (_total_energies == 0 || ability == AbilityID::None)
		return;

	++_ability_levels[Under(ability)];
	--_total_energies;
}

void ScenarioScene::ChargeAbilityAmmo(const PowerUpID power_up, const u16 ammo)
{
	const auto ability_id = PowerUpToAbility(power_up);
	if (ability_id == AbilityID::None)
		return;

	_ability_ammo[Under(ability_id)] += ammo;
}

AbilityID ScenarioScene::KeyToAbility(const sf::Keyboard::Key key)
{
	using Key = sf::Keyboard::Key;
	switch (key)
	{
	case Key::Q: return AbilityID::Ability1;
	case Key::W: return AbilityID::Ability2;
	case Key::E: return AbilityID::Ability3;
	case Key::R: return AbilityID::Ability4;
	case Key::A: return AbilityID::Ability5;
	case Key::S: return AbilityID::Ability6;
	case Key::D: return AbilityID::Ability7;
	case Key::F: return AbilityID::Ability8;
	default: return AbilityID::None;
	}
}

AbilityID ScenarioScene::PowerUpToAbility(const PowerUpID id)
{
	// Ammo_Ability1..8 follow each other, as Ability1..8 do
	if (id < PowerUpID::Ammo_Ability1 || id > PowerUpID::Ammo_Ability8)
		return AbilityID::None;

	return AbilityID(Under(id) - Under(PowerUpID::Ammo_Ability1));
}

void ScenarioScene::Render(sf::RenderWindow & wnd, const sf::Time delta_time)
{
	_scenario->Render(wnd, delta_time);

	wnd.setView(wnd.getDefaultView());
	g_app->RenderText(format("E: {}   Shots: {}   Destroyed: {}   Power ups: {}", _total_energies, _shots, _destroyed_obstacles, _power_ups_taken), { 0.f, g_res_y - 30.f }, 25);

	constexpr float kShapeSize = 35.f;
	constexpr const char * kKeys[kAbilityCount] = { "Q", "W", "E", "R", "A", "S", "D", "F" };
	constexpr float kTotalWidth = kAbilityCount * kShapeSize;

	sf::RectangleShape shape({ kShapeSize, kShapeSize });
	shape.setFillColor({ 255 / 2, 0, 0, 255 / 2 });
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);

	for (szt i = 0u; i < kAbilityCount; ++i)
	{
		shape.setPosition({ (g_res_x / 2.f - kTotalWidth / 2.f) + (i * kShapeSize), g_res_y - (kShapeSize * 2.f) });
		wnd.draw(shape);

		const auto pos = shape.getPosition();
		g_app->RenderText(kKeys[i], pos, 30);
		g_app->RenderText(to_string(_ability_ammo[i]), { pos.x + 15.f, pos.y }, 15);
		g_app->RenderText(to_string(_ability_levels[i]), { pos.x + 15.f, pos.y + 15.f }, 15);
	}
}

void ScenarioScene::Tick(const sf::Time delta_time)
{
	_scenario->Tick(delta_time);
	_scenario->Move(0, ObjectShip1::kScrollVelocity * delta_time.asSeconds());
}
