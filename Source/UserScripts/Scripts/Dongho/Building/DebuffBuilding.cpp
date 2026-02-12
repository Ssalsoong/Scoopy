#include "Export.h"
#include "ScriptBehaviour.h"
#include "DebuffBuilding.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Building.h"
#include "../../test/EnemyMove.h"
#include "../../test/EnemyController.h"
#include "Transform.h"
#include "../Battlestats.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<DebuffBuilding>("DebuffBuilding")
		(rttr::metadata("wrapper_type_name", "ObjPtr<DebuffBuilding>"))
		.property("speedDebuff", &DebuffBuilding::speedDebuff);

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
	GiveDebuff();
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
				enemyMove->AddSpeedDebuffSource(this, speedDebuff);
			if (auto enemyController = enemy->GetComponent<EnemyController>(); enemyController.IsValid())
				enemyController->AddAttackDebuffSource(this, attackDebuff);
		}
	}
	for (auto& enemy : m_inside)
	{
		if (nowInside.find(enemy) == nowInside.end())
		{
			if (auto enemyMove = enemy->GetComponent<EnemyMove>(); enemyMove.IsValid())
				enemyMove->RemoveSpeedDebuffSource(this);
			if (auto enemyController = enemy->GetComponent<EnemyController>(); enemyController.IsValid())
				enemyController->RemoveAttackDebuffSource(this);
		}
	}
	m_inside.swap(nowInside);

}

void MMMEngine::DebuffBuilding::LevelApply(int level)
{
	if (level == 1)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 70;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(70);
		speedDebuff = 0.8f;
		attackDebuff = 1.5f;
		debuffdist = 2.0f;
	}
	if (level == 2)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 90;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(90);
		speedDebuff = 0.65f;
		attackDebuff = 1.75f;
		debuffdist = 2.5f;
	}
	if (level == 3)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 110;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(110);
		speedDebuff = 0.65f;
		attackDebuff = 2.f;
		debuffdist = 3.0f;
	}
	if (level == 4)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 130;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(130);
		speedDebuff = 0.5f;
		attackDebuff = 2.25f;
		debuffdist = 3.5f;
	}
	if (level == 5)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 150;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(150);
		speedDebuff = 0.5f;
		attackDebuff = 2.5f;
		debuffdist = 4.0f;
	}
}