#include "HPBuilding.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Building.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<HPBuilding>("HPBuilding")
		(rttr::metadata("wrapper_type_name", "ObjPtr<HPBuilding>"));

	registration::class_<ObjPtr<HPBuilding>>("ObjPtr<HPBuilding>")
		.constructor(
			[]() {
				return Object::NewObject<HPBuilding>();
			}).method("Inject", &ObjPtr<HPBuilding>::Inject);
}

void MMMEngine::HPBuilding::Start()
{
	GetGameObject()->GetComponent<Building>()->HP = 100;
	GetGameObject()->GetComponent<Building>()->maxHP = 100;
}

void MMMEngine::HPBuilding::Update()
{

}

void MMMEngine::HPBuilding::LevelApply(int level)
{
	GetGameObject()->GetComponent<Building>()->maxHP = 50 *level + 50;
	GetGameObject()->GetComponent<Building>()->HP =
		GetGameObject()->GetComponent<Building>()->maxHP;
}