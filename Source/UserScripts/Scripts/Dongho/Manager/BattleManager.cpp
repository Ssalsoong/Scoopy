#include "Export.h"
#include "ScriptBehaviour.h"
#include "BattleManager.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../Castle/Castle.h"
#include "../Building/Building.h"
#include "../Battlestats.h"

MMMEngine::ObjPtr<MMMEngine::BattleManager> MMMEngine::BattleManager::instance = nullptr;

void MMMEngine::BattleManager::Start()
{
	instance = GetGameObject()->GetComponent<BattleManager>();
}

void MMMEngine::BattleManager::Update()
{
}

void MMMEngine::BattleManager::Attack(ObjPtr<GameObject> target, int damage)
{
	if (!target) return;

	auto bs = target->GetComponent<Battlestats>();
	if (!bs) return;
	if (bs->HP <= 0) return;
	if (auto playercomp = target->GetComponent<Player>())
	{
		playercomp->GetDamage(damage);
	}
	else {
		bs->HP -= damage;
		if (bs->HP < 0) bs->HP = 0;
	}
}

void MMMEngine::BattleManager::SnowAttack(ObjPtr<GameObject> target, int point)
{
	if (!target) return;
	auto ec = target->GetComponent<Enemy>();
	if (!ec) return;
	auto bs = target->GetComponent<Battlestats>();
	if (!bs) return;
	if (bs->HP <= 0) return;
	if (!ec->ApplySnowDamage()) return;

	bs->HP = std::max(bs->HP - point, 0);
}