#include "Export.h"
#include "ScriptBehaviour.h"
#include "BattleManager.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../Castle/Castle.h"
#include "../Building/Building.h"

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

	if (auto p = target->GetComponent<Player>())
		p->GetDamage(damage);
	else if (auto e = target->GetComponent<Enemy>())
		e->GetDamage(damage);
	else if (auto c = target->GetComponent<Castle>())
		c->GetDamage(damage);
	else if (auto b = target->GetComponent<Building>())
		b->GetDamage(damage);
}