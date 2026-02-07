#include "Export.h"
#include "ScriptBehaviour.h"
#include "../Building/BuffBuilding.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "../Building/Building.h"
#include "../Player/Player.h"
#include "Transform.h"
#include "../Battlestats.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<BuffBuilding>("BuffBuilding")
        (rttr::metadata("wrapper_type_name", "ObjPtr<BuffBuilding>"));

	registration::class_<ObjPtr<BuffBuilding>>("ObjPtr<BuffBuilding>")
		.constructor(
			[]() {
				return Object::NewObject<BuffBuilding>();
			})
        .method("Inject", &ObjPtr<BuffBuilding>::Inject);
}

void MMMEngine::BuffBuilding::Start()
{
	player = GetGameObject()->Find("Player");
}

void MMMEngine::BuffBuilding::Update()
{
	if (!player) return;
	playerpos = player->GetTransform()->GetWorldPosition();
	GiveBuff();
}

void MMMEngine::BuffBuilding::GiveBuff()
{
	auto pos = GetTransform()->GetWorldPosition();
	float bestD2 = buffdist * buffdist;
	float dx = pos.x - playerpos.x;
	float dz = pos.z - playerpos.z;
	float d2 = dx * dx + dz * dz;
	bool nowInRange = (d2 < bestD2);
	if (nowInRange && !prevInRange)
	{
		//player->GetComponent<Player>()->velocity*= buff;
	}
	else if(!nowInRange && prevInRange)
	{
		//player->GetComponent<Player>()->velocity /= buff;
	}
	prevInRange = nowInRange;
}

void MMMEngine::BuffBuilding::LevelApply(int level)
{
	if (level == 1)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 50;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		buff = 1.1f;
	}
	if (level == 2)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 50;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		buff = 1.2f;
	}
	if (level == 3)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 75;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		buff = 1.3f;
	}
	if (level == 4)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 75;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		buff = 1.4f;
	}
	if (level == 5)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 100;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		buff = 1.5f;
	}
}