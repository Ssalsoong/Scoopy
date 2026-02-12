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
#include "Prefab.h"

MMMEngine::ObjPtr<MMMEngine::BuildingManager> MMMEngine::BuildingManager::instance = nullptr;

void MMMEngine::BuildingManager::Awake()
{
	instance = GetGameObject()->GetComponent<BuildingManager>();

	if (!pre_building || !mBuildingPoint) {
		std::cout << "BuildingManager::Prefab not Found!!" << std::endl;
	}
}

void MMMEngine::BuildingManager::Start()
{
	//pre_building = ResourceManager::Get().Load<Prefab>(L"Assets/Prefab/Building.Prefab");

	if (!pre_building) {
		std::cout << "BuildingManager::No Building Prefab!!!" << std::endl;
	}

	buildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Tower/Mesh/nomalTower_StaticMesh.staticmesh");
	HPbuildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Tower/Mesh/nomal2Tower_StaticMesh.staticmesh");
	buffbuildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Tower/Mesh/lightingTower_StaticMesh.staticmesh");
	debuffbuildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Tower/Mesh/fireTower_StaticMesh.staticmesh");
	snowbuildingmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Tower/Mesh/iceTower_StaticMesh.staticmesh");
	for (int i = 0; i < BuildingPointCount; ++i)
	{
		if (mBuildingPoint) {
			auto obj = Instantiate(mBuildingPoint);
			/*obj->SetName("BuildingPoint");
			obj->SetTag("BuildingPoint");
			obj->AddComponent<BuildingPoint>();
			obj->AddComponent<MeshRenderer>();
			obj->GetTransform()->GetChild(0)->GetComponent<MeshRenderer>()->SetMesh(cube);
			obj->GetTransform()->SetWorldScale(0.1f, 0.5f, 0.1f);
			*/
			obj->GetTransform()->SetWorldPosition(BuildingPos[i]);
			obj->GetTransform()->SetParent(GetTransform());
			buildingpoints.push_back(obj);
		}
	}
}

void MMMEngine::BuildingManager::Update()
{
}

void MMMEngine::BuildingManager::Build(ObjPtr<GameObject> obj)
{
	obj->GetComponent<BuildingPoint>()->Setalreadybuilt(true);

	auto building = Instantiate(pre_building);
	//building->GetTransform()->SetParent(obj->GetTransform());
	building->GetTransform()->SetParent(GetTransform());
	building->GetTransform()->SetWorldPosition(obj->GetTransform()->GetWorldPosition());
	Buildings.push_back(building);
	if (distup)
		building->GetComponent<Building>()->SetAttackDist(5.5f);
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
		auto maxHP = obj->GetComponent<Building>()->maxHP;
		obj->GetComponent<Battlestats>()->SetHP(maxHP);
	}
}

void MMMEngine::BuildingManager::LevelUpHP(ObjPtr<GameObject> obj)
{
	if (obj->GetComponent<SnowBuilding>() || obj->GetComponent<BuffBuilding>()
		|| obj->GetComponent<DebuffBuilding>())
		return;
	if (!obj->GetComponent<HPBuilding>()) {
		obj->AddComponent<HPBuilding>();
		obj->GetTransform()->GetChild(0)->GetComponent<MeshRenderer>()->SetMesh(HPbuildingmesh);
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
		obj->GetTransform()->GetChild(0)->GetComponent<MeshRenderer>()->SetMesh(buffbuildingmesh);
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
		obj->GetTransform()->GetChild(0)->GetComponent<MeshRenderer>()->SetMesh(debuffbuildingmesh);
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
		obj->GetTransform()->GetChild(0)->GetComponent<MeshRenderer>()->SetMesh(snowbuildingmesh);
	}
	if (obj->GetComponent<Building>()->level >= 5)
		return;
	obj->GetComponent<Building>()->level++;
	obj->GetComponent<SnowBuilding>()->LevelApply(obj->GetComponent<Building>()->level);
}

void MMMEngine::BuildingManager::BuildingsDistUP()
{
	for (auto& e : Buildings)
	{
		e->GetComponent<Building>()->SetAttackDist(5.5f);
		distup = true;
	}
}