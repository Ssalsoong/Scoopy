#include "Export.h"
#include "ScriptBehaviour.h"
#include "Battlestats.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Castle/Castle.h"
#include "Building/Building.h"

void MMMEngine::Battlestats::Start()
{
}

void MMMEngine::Battlestats::Update()
{
	if (HP <= 0)
		Dead();
}

void MMMEngine::Battlestats::Dead()
{
	if (GetComponent<Player>())
	{
		GetComponent<Player>()->Dead();
	}
	if (GetComponent<Enemy>())
	{
		GetComponent<Enemy>()->ChangeState(Enemy::EnemyState::Dead);
	}
	if (GetComponent<Castle>())
	{
		GetComponent<Castle>()->Dead();
	}
	if (GetComponent<Building>())
	{
		GetComponent<Building>()->Dead();
	}
}