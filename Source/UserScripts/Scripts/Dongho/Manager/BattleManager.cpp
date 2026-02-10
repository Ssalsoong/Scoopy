#include "Export.h"
#include "ScriptBehaviour.h"
#include "BattleManager.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../Castle/Castle.h"
#include "../Building/Building.h"
#include "../Battlestats.h"
#include "../Snow/Snowball.h"

MMMEngine::ObjPtr<MMMEngine::BattleManager> MMMEngine::BattleManager::instance = nullptr;

void MMMEngine::BattleManager::Awake()
{
	instance = SelfPtr(this);
}

void MMMEngine::BattleManager::Start()
{
}

void MMMEngine::BattleManager::Update()
{
}

void MMMEngine::BattleManager::OnDestry()
{
	if (instance == SelfPtr(this))
		instance = nullptr;
}

void MMMEngine::BattleManager::Attack(ObjPtr<GameObject> attacker, ObjPtr<GameObject> target, int damage)
{
	if (!attacker || !target) return;

	auto targetbs = target->GetComponent<Battlestats>();
	if (!targetbs || targetbs->HP <= 0) return;

	switch (targetbs->type)
	{
	case Battlestats::Type::Player:
	{
		if (auto player = target->GetComponent<Player>())
			player->GetDamage(attacker, damage);
		return;
	}

	case Battlestats::Type::Enemy:
	{
		auto enemy = target->GetComponent<Enemy>();
		if (!enemy) return;

		// Snowball -> Enemy (특수)
		if (attacker->GetComponent<Snowball>())
		{
			if (!enemy->ApplySnowDamage()) return;
			targetbs->ApplyDamage(damage);
			return;
		}

		// Player -> Enemy
		if (attacker->GetComponent<Player>())
		{
			targetbs->ApplyDamage(damage);
			enemy->PlayerHitMe();
			return;
		}
		targetbs->ApplyDamage(damage);

		return;
	}

	case Battlestats::Type::Castle:
	{
		if (auto castle = target->GetComponent<Castle>())
			castle->GetDamage(attacker, damage);
		return;
	}

	case Battlestats::Type::Building:
	{
		targetbs->ApplyDamage(damage);
		return;
	}

	default:
		return;
	}
}