#include "Export.h"
#include "ScriptBehaviour.h"
#include "../Building/BuffBuilding.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "../Building/Building.h"

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
}

void MMMEngine::BuffBuilding::Update()
{
}

void MMMEngine::BuffBuilding::LevelApply(int level)
{
	if (level == 1)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 50;
		GetGameObject()->GetComponent<Building>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		buff = 1.1f;
	}
	if (level == 2)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 50;
		GetGameObject()->GetComponent<Building>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		buff = 1.2f;
	}
	if (level == 3)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 75;
		GetGameObject()->GetComponent<Building>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		buff = 1.3f;
	}
	if (level == 4)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 75;
		GetGameObject()->GetComponent<Building>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		buff = 1.4f;
	}
	if (level == 5)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 100;
		GetGameObject()->GetComponent<Building>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		buff = 1.5f;
	}
}