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
#include "../Building/BuildingPoint.h"
#include "../Battlestats.h"

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
	buildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Tower/Mesh/nomalTower_StaticMesh.staticmesh");
	HPbuildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Tower/Mesh/nomal2Tower_StaticMesh.staticmesh");
	buffbuildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Tower/Mesh/lightingTower_StaticMesh.staticmesh");
	debuffbuildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Tower/Mesh/fireTower_StaticMesh.staticmesh");
	snowbuildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Tower/Mesh/iceTower_StaticMesh.staticmesh");
	ResPtr<StaticMesh> cube = ResourceManager::Get().Load<StaticMesh>(L"Assets/DefaultMesh/Cube_StaticMesh.staticmesh");
	for (int i = 0; i < BuildingPointCount; ++i)
	{
		auto obj = NewObject<GameObject>();
		obj->SetName("BuildingPoint");
		obj->SetTag("BuildingPoint");
		obj->AddComponent<BuildingPoint>();
		obj->AddComponent<MeshRenderer>();
		obj->GetComponent<MeshRenderer>()->SetMesh(cube);
		obj->GetTransform()->SetWorldScale(0.1f, 0.1f, 0.1f);
		obj->GetTransform()->SetWorldPosition(BuildingPos[i]);
		obj->GetTransform()->SetParent(GetTransform());
		buildingpoints.push_back(obj);
	}

	instance = GetGameObject()->GetComponent<BuildingManager>();
}

void MMMEngine::BuildingManager::Update()
{
}

void MMMEngine::BuildingManager::Build(ObjPtr<GameObject> obj)
{
	auto it = std::find(buildingpoints.begin(), buildingpoints.end(), obj);
	if (it == buildingpoints.end())
		return;
	buildingpoints.erase(it);

	Destroy(obj->GetComponent<BuildingPoint>());
	obj->SetName("Building");
	obj->SetTag("Building");
	obj->AddComponent<Building>();
	obj->AddComponent<Battlestats>();
	obj->GetComponent<Battlestats>()->HP = 50;
	obj->GetComponent<MeshRenderer>()->SetMesh(buildingmesh);
	obj->GetTransform()->SetWorldScale(buildingscale);
	Buildings.push_back(obj);
}

void MMMEngine::BuildingManager::BuildingReturn()
{
	for (auto& obj : Buildings)
	{
		if (obj->GetComponent<Building>()->isDead)
		{
			obj->GetComponent<Building>()->isDead = false;
			obj->SetActive(true);
		}
		obj->GetComponent<Battlestats>()->HP = obj->GetComponent<Building>()->maxHP;
	}
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