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
	instance = GetGameObject()->GetComponent<BattleManager>();
}

void MMMEngine::BattleManager::Start()
{
}

void MMMEngine::BattleManager::Update()
{
}

void MMMEngine::BattleManager::Attack(ObjPtr<GameObject> attacker, ObjPtr<GameObject> target, int damage)
{
	if (!attacker || !target) return;
	if (attacker->GetComponent<Snowball>())
	{
		auto ec = target->GetComponent<Enemy>();
		if (!ec) return;
		auto bs = target->GetComponent<Battlestats>();
		if (!bs) return;
		if (bs->HP <= 0) return;
		if (!ec->ApplySnowDamage()) return;

		bs->ApplyDamage(damage);
	}
	else
	{
		auto bs = target->GetComponent<Battlestats>();
		if (!bs) return;
		if (bs->HP <= 0) return;
		if (auto player = target->GetComponent<Player>())
		{
			player->GetDamage(attacker, damage);
			return;
		}
		if (auto castle = target->GetComponent<Castle>())
		{
			castle->GetDamage(attacker, damage);
			return;
		}
		bs->ApplyDamage(damage);
	}
}