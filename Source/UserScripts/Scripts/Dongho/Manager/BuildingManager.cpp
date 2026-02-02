#include "Export.h"
#include "ScriptBehaviour.h"
#include "BuildingManager.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "../Building/Building.h"
#include "MeshRenderer.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "../Building/HPBuilding.h"
#include "../Building/BuffBuilding.h"
#include "../Building/DebuffBuilding.h"
#include "../Building/SnowBuilding.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<BuildingManager>("BuildingManager")
        (rttr::metadata("wrapper_type_name", "ObjPtr<BuildingManager>"));

	registration::class_<ObjPtr<BuildingManager>>("ObjPtr<BuildingManager>")
		.constructor(
			[]() {
				return Object::NewObject<BuildingManager>();
			})
        .method("Inject", &ObjPtr<BuildingManager>::Inject);
}

MMMEngine::ObjPtr<MMMEngine::BuildingManager> MMMEngine::BuildingManager::instance = nullptr;

void MMMEngine::BuildingManager::Start()
{
	buildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/DefaultMesh/Cube_StaticMesh.staticmesh");
	HPbuildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/DefaultMesh/Cube_StaticMesh.staticmesh");
	buffbuildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/DefaultMesh/Cube_StaticMesh.staticmesh");
	debuffbuildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/DefaultMesh/Cube_StaticMesh.staticmesh");
	snowbuildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/DefaultMesh/Cube_StaticMesh.staticmesh");
	instance = GetGameObject()->GetComponent<BuildingManager>();
}

void MMMEngine::BuildingManager::Update()
{
}

void MMMEngine::BuildingManager::BuildingReturn()
{
	for (auto& obj : Buildings)
	{
		if (obj->GetComponent<Building>()->isDead)
		{
			obj->GetComponent<Building>()->isDead = false;
			obj->SetActive(true);
			obj->GetComponent<Building>()->HP = obj->GetComponent<Building>()->maxHP;
		}
	}
}

void MMMEngine::BuildingManager::MakeBuilding(const DirectX::SimpleMath::Vector3 pos)
{
	auto obj = NewObject<GameObject>();
	obj->SetName("Building");
	obj->SetTag("Building");
	obj->AddComponent<Building>();
	obj->AddComponent<MeshRenderer>();
	obj->GetComponent<MeshRenderer>()->SetMesh(buildingmesh);
	obj->GetTransform()->SetWorldPosition(pos);
	obj->GetTransform()->SetWorldScale(buildingscale);
	Buildings.push_back(obj);
}

void MMMEngine::BuildingManager::LevelUpHP(ObjPtr<GameObject> obj)
{
	if (obj->GetComponent<SnowBuilding>() || obj->GetComponent<BuffBuilding>()
		|| obj->GetComponent<DebuffBuilding>())
		return;
	if (!obj->GetComponent<HPBuilding>()) {
		obj->AddComponent<HPBuilding>();
		obj->GetComponent<MeshRenderer>()->SetMesh(HPbuildingmesh);
	}
	if (obj->GetComponent<Building>()->level >= 5)
		return;
	obj->GetComponent<Building>()->level++;
	obj->GetComponent<HPBuilding>()->LevelApply(obj->GetComponent<Building>()->level);
}

void MMMEngine::BuildingManager::LevelUpBuff(ObjPtr<GameObject> obj)
{
	if (obj->GetComponent<SnowBuilding>() || obj->GetComponent<HPBuilding>()
		|| obj->GetComponent<DebuffBuilding>())
		return;
	if (!obj->GetComponent<BuffBuilding>()) {
		obj->AddComponent<BuffBuilding>();
		obj->GetComponent<MeshRenderer>()->SetMesh(buffbuildingmesh);
	}
	if (obj->GetComponent<Building>()->level >= 5)
		return;
	obj->GetComponent<Building>()->level++;
	obj->GetComponent<BuffBuilding>()->LevelApply(obj->GetComponent<Building>()->level);
}

void MMMEngine::BuildingManager::LevelUpDeBuff(ObjPtr<GameObject> obj)
{
	if (obj->GetComponent<SnowBuilding>() || obj->GetComponent<BuffBuilding>()
		|| obj->GetComponent<HPBuilding>())
		return;
	if (!obj->GetComponent<DebuffBuilding>()) {
		obj->AddComponent<DebuffBuilding>();
		obj->GetComponent<MeshRenderer>()->SetMesh(debuffbuildingmesh);
	}
	if (obj->GetComponent<Building>()->level >= 5)
		return;
	obj->GetComponent<Building>()->level++;
	obj->GetComponent<DebuffBuilding>()->LevelApply(obj->GetComponent<Building>()->level);
}

void MMMEngine::BuildingManager::LevelUpSnow(ObjPtr<GameObject> obj)
{
	if (obj->GetComponent<DebuffBuilding>() || obj->GetComponent<BuffBuilding>()
		|| obj->GetComponent<HPBuilding>())
		return;
	if (!obj->GetComponent<SnowBuilding>()) {
		obj->AddComponent<SnowBuilding>();
		obj->GetComponent<MeshRenderer>()->SetMesh(snowbuildingmesh);
	}
	if (obj->GetComponent<Building>()->level >= 5)
		return;
	obj->GetComponent<Building>()->level++;
	obj->GetComponent<SnowBuilding>()->LevelApply(obj->GetComponent<Building>()->level);
}