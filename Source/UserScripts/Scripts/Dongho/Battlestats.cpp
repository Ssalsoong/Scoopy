#include "Export.h"
#include "ScriptBehaviour.h"
#include "Battlestats.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Castle/Castle.h"
#include "Building/Building.h"

void MMMEngine::Battlestats::Start()
{
	if (GetComponent<Player>())
		type = Type::Player;
	else if (GetComponent<Enemy>())
		type = Type::Enemy;
	else if (GetComponent<Castle>())
		type = Type::Castle;
	else if (GetComponent<Building>())
		type = Type::Building;
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
		if (auto e = GetComponent<Enemy>()) e->ChangeState(Enemy::EnemyState::Dead);
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