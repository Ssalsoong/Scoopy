#include "Export.h"
#include "ScriptBehaviour.h"
#include "Battlestats.h"
#include "Player/Player.h"
#include "Castle/Castle.h"
#include "Building/Building.h"
#include "../test/EnemyController.h"

void MMMEngine::Battlestats::Start()
{

	if (auto player = GetComponent<Player>()) {
		HP = player->GetmaxHP();
		type = Type::Player;
	}
	else if (GetComponent<EnemyController>()) {
		type = Type::Enemy;
	}
	else if (auto castle = GetComponent<Castle>()) {
		HP = castle->GetmaxHP();
		type = Type::Castle;
	}
		
	else if (auto building = GetComponent<Building>()) {
		HP = building->maxHP;
		type = Type::Building;
	}
		
}

void MMMEngine::Battlestats::Update()
{
	if (!bDead && HP <= 0)
	{
		bDead = true;
		Dead();
	}
}

void MMMEngine::Battlestats::ApplyDamage(int amount)
{
	if (bDead || amount <= 0) return;

	HP = std::max(HP - amount, 0);
}

void MMMEngine::Battlestats::Dead()
{
	switch (type)
	{
	case Type::Player:
		if (auto p = GetComponent<Player>()) p->Dead();
		return;
	case Type::Enemy:
		if (auto e = GetComponent<EnemyController>()) e->OnStateEnter(EnemyController::EnemyState::Dead);
		return;
	case Type::Castle:
		if (auto c = GetComponent<Castle>()) c->Dead();
		return;
	case Type::Building:
		if (auto b = GetComponent<Building>()) b->Dead();
		return;
	default:
		return;
	}
}

void MMMEngine::Battlestats::SetHP(int value)
{
	HP = value;
	if (HP > 0) bDead = false;
}