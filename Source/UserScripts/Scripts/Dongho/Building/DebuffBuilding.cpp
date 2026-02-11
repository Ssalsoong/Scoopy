#include "Export.h"
#include "ScriptBehaviour.h"
#include "DebuffBuilding.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Building.h"
#include "../../test/EnemyMove.h"
#include "Transform.h"
#include "../Battlestats.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<DebuffBuilding>("DebuffBuilding")
        (rttr::metadata("wrapper_type_name", "ObjPtr<DebuffBuilding>"));

	registration::class_<ObjPtr<DebuffBuilding>>("ObjPtr<DebuffBuilding>")
		.constructor(
			[]() {
				return Object::NewObject<DebuffBuilding>();
			})
        .method("Inject", &ObjPtr<DebuffBuilding>::Inject);
}

void MMMEngine::DebuffBuilding::Start()
{

}

void MMMEngine::DebuffBuilding::Update()
{
}

void MMMEngine::DebuffBuilding::GiveDebuff()
{
	std::unordered_set<ObjPtr<GameObject>> nowInside;

	auto pos = GetTransform()->GetWorldPosition();
	float bestD2 = debuffdist * debuffdist;
	auto enemys = GetGameObject()->FindGameObjectsWithTag("Enemy");
	
	for (auto& enemy : enemys)
	{

		if (!enemy) continue;

		auto epos = enemy->GetTransform()->GetWorldPosition();

		float dx = pos.x - epos.x;
		float dz = pos.z - epos.z;
		float d2 = dx * dx + dz * dz;

		if (d2 < bestD2)
			nowInside.insert(enemy);
	}
	for (auto& enemy : nowInside)
	{
		if (m_inside.find(enemy) == m_inside.end())
		{
			if (auto enemyMove = enemy->GetComponent<EnemyMove>(); enemyMove.IsValid())
				enemyMove->SetDebuffSpeed(debuff);
			//enemy->attackDelay /= debuff;
		}
	}
	for (auto& enemy : m_inside)
	{
		if (nowInside.find(enemy) == nowInside.end())
		{
			if (auto enemyMove = enemy->GetComponent<EnemyMove>(); enemyMove.IsValid())
				enemyMove->SetDebuffSpeed(1.0f);
			//enemy->attackDelay *= debuff;
		}
	}
	m_inside.swap(nowInside);

}

void MMMEngine::DebuffBuilding::LevelApply(int level)
{
	if (level == 1)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 50;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(50);
		debuff = 0.8f;
		debuffdist = 2.0f;
	}
	if (level == 2)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 50;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(50);
		debuff = 0.65f;
		debuffdist = 2.0f;
	}
	if (level == 3)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 75;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(75);
		debuff = 0.65f;
		debuffdist = 3.0f;
	}
	if (level == 4)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 75;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(75);
		debuff = 0.5f;
		debuffdist = 3.0f;
	}
	if (level == 5)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 100;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(100);
		debuff = 0.5f;
		debuffdist = 4.0f;
	}
}