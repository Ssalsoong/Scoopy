#include "Export.h"
#include "ScriptBehaviour.h"
#include "DebuffBuilding.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Building.h"

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
	GetGameObject()->GetComponent<Building>()->HP = 50;
	GetGameObject()->GetComponent<Building>()->maxHP = 50;
}

void MMMEngine::DebuffBuilding::Update()
{
}

void MMMEngine::DebuffBuilding::LevelApply(int level)
{
	if (level == 1)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 50;
		GetGameObject()->GetComponent<Building>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		debuff = 0.8f;
	}
	if (level == 2)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 50;
		GetGameObject()->GetComponent<Building>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		debuff = 0.65f;
	}
	if (level == 3)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 75;
		GetGameObject()->GetComponent<Building>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		debuff = 0.65f;
	}
	if (level == 4)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 75;
		GetGameObject()->GetComponent<Building>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		debuff = 0.5f;
	}
	if (level == 5)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 100;
		GetGameObject()->GetComponent<Building>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		debuff = 0.5f;
	}
}