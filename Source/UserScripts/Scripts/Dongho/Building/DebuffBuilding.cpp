#include "Export.h"
#include "ScriptBehaviour.h"
#include "DebuffBuilding.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Building.h"
#include "../Enemy/Enemy.h"
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
	GetGameObject()->GetComponent<Battlestats>()->HP = 50;
	GetGameObject()->GetComponent<Building>()->maxHP = 50;
}

void MMMEngine::DebuffBuilding::Update()
{
}

void MMMEngine::DebuffBuilding::GiveDebuff()
{
	std::unordered_set<ObjPtr<Enemy>> nowInside;

	auto pos = GetTransform()->GetWorldPosition();
	float bestD2 = debuffdist * debuffdist;
	auto enemys = GetGameObject()->FindGameObjectsWithTag("Enemy");
	
	for (auto& e : enemys)
	{
		if (!e) continue;

		auto enemy = e->GetComponent<Enemy>();
		if (!enemy) continue;

		auto epos = e->GetTransform()->GetWorldPosition();

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
			enemy->velocity *= debuff;
			enemy->attackDelay /= debuff;
		}
	}
	for (auto& enemy : m_inside)
	{
		if (nowInside.find(enemy) == nowInside.end())
		{
			enemy->velocity /= debuff;
			enemy->attackDelay *= debuff;
		}
	}
	m_inside.swap(nowInside);

}

void MMMEngine::DebuffBuilding::LevelApply(int level)
{
	if (level == 1)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 50;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		debuff = 0.8f;
	}
	if (level == 2)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 50;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		debuff = 0.65f;
	}
	if (level == 3)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 75;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		debuff = 0.65f;
	}
	if (level == 4)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 75;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		debuff = 0.5f;
	}
	if (level == 5)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 100;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		debuff = 0.5f;
	}
}