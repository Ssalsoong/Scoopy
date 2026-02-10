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
	if (auto player = GetComponent<Player>()) { player->Dead(); return; }
	if (auto enemy = GetComponent<Enemy>()) { enemy->ChangeState(Enemy::EnemyState::Dead); return; }
	if (auto castle = GetComponent<Castle>()) { castle->Dead(); return; }
	if (auto bld = GetComponent<Building>()) { bld->Dead(); return; }
}

void MMMEngine::Battlestats::SetHP(int value)
{
	HP = value;
	if (HP > 0) bDead = false;
}