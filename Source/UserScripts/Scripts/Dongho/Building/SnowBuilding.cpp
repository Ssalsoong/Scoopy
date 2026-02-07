#include "Export.h"
#include "ScriptBehaviour.h"
#include "SnowBuilding.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Building.h"
#include "MMMTime.h"
#include "../Battlestats.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<SnowBuilding>("SnowBuilding")
        (rttr::metadata("wrapper_type_name", "ObjPtr<SnowBuilding>"));

	registration::class_<ObjPtr<SnowBuilding>>("ObjPtr<SnowBuilding>")
		.constructor(
			[]() {
				return Object::NewObject<SnowBuilding>();
			})
        .method("Inject", &ObjPtr<SnowBuilding>::Inject);
}

void MMMEngine::SnowBuilding::Start()
{
	GetGameObject()->GetComponent<Battlestats>()->HP = 50;
	GetGameObject()->GetComponent<Building>()->maxHP = 50;
}

void MMMEngine::SnowBuilding::Update()
{
	UpdateSnow();
}

void MMMEngine::SnowBuilding::UpdateSnow()
{
	chargeTimer += Time::GetDeltaTime();
	if (chargeTimer >= chargeDelay)
	{
		if(snowpoint<maxsnowpoint)
		snowpoint++;
		chargeTimer = 0.0f;
	}
}

void MMMEngine::SnowBuilding::LevelApply(int level)
{
	if (level == 1)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 50;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		maxsnowpoint = 15;
	}
	if (level == 2)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 50;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		maxsnowpoint = 20;
	}
	if (level == 3)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 65;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		maxsnowpoint = 25;
	}
	if (level == 4)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 65;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		maxsnowpoint = 30;
	}
	if (level == 5)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 80;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		maxsnowpoint = 35;
	}
}