#include "EnemySpawner.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/NormalEnemy.h"
#include "../Enemy/ArrowEnemy.h"
#include "../Enemy/ThiefEnemy.h"
#include "../Player/Player.h"
#include "MMMTime.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "StaticMesh.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<EnemySpawner>("EnemySpawner")
		(rttr::metadata("wrapper_type_name", "ObjPtr<EnemySpawner>"));

	registration::class_<ObjPtr<EnemySpawner>>("ObjPtr<EnemySpawner>")
		.constructor(
			[]() {
				return Object::NewObject<EnemySpawner>();
			}).method("Inject", &ObjPtr<EnemySpawner>::Inject);
}

MMMEngine::ObjPtr<MMMEngine::EnemySpawner> MMMEngine::EnemySpawner::instance = nullptr;

void MMMEngine::EnemySpawner::Start()
{
	instance = GetGameObject()->GetComponent<EnemySpawner>();
	normalenemymesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/DefaultMesh/Monkey_StaticMesh.staticmesh");
	for (int i = 0; i < 20; ++i)
	{
		auto obj = NewObject<GameObject>();
		obj->SetName("NormalEnemy");
		obj->SetTag("Enemy");
		obj->GetTransform()->SetParent(GetTransform());
		obj->AddComponent<Enemy>();
		obj->AddComponent<NormalEnemy>();
		obj->AddComponent<MeshRenderer>();
		obj->GetComponent<MeshRenderer>()->SetMesh(normalenemymesh);
		obj->GetTransform()->SetWorldScale(normalsize);
		obj->GetTransform()->SetWorldPosition(200.f, 200.f, 200.f);
		obj->SetActive(false);
		NormalEnemys.push(obj);
	}
	arrowenemymesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/DefaultMesh/Monkey_StaticMesh.staticmesh");
	for (int i = 0; i < 20; ++i)
	{
		auto obj = NewObject<GameObject>();
		obj->SetName("ArrowEnemy");
		obj->SetTag("Enemy");
		obj->GetTransform()->SetParent(GetTransform());
		obj->AddComponent<Enemy>();
		obj->AddComponent<ArrowEnemy>();
		obj->AddComponent<MeshRenderer>();
		obj->GetComponent<MeshRenderer>()->SetMesh(arrowenemymesh);
		obj->GetTransform()->SetWorldScale(arrowsize);
		obj->GetTransform()->SetWorldPosition(200.f, 200.f, 200.f);
		obj->SetActive(false);
		ArrowEnemys.push(obj);
	}
	thiefenemymesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/DefaultMesh/Monkey_StaticMesh.staticmesh");
	for (int i = 0; i < 20; ++i)
	{
		auto obj = NewObject<GameObject>();
		obj->SetName("ThiefEnemy");
		obj->SetTag("Enemy");
		obj->GetTransform()->SetParent(GetTransform());
		obj->AddComponent<Enemy>();
		obj->AddComponent<ThiefEnemy>();
		obj->AddComponent<MeshRenderer>();
		obj->GetComponent<MeshRenderer>()->SetMesh(thiefenemymesh);
		obj->GetTransform()->SetWorldScale(thiefsize);
		obj->GetTransform()->SetWorldPosition(200.f, 200.f, 200.f);
		obj->SetActive(false);
		ThiefEnemys.push(obj);
	}
}

void MMMEngine::EnemySpawner::Update()
{

}

void MMMEngine::EnemySpawner::SpawnNormalEnemy()
{
	if (NormalEnemys.empty())
		return;
	auto obj = NormalEnemys.front();
	NormalEnemys.pop();

	if (!obj)
		return;
	obj->GetTransform()->SetWorldPosition(normalspawnpos);
	obj->GetComponent<NormalEnemy>()->ApplyStats();
	obj->SetActive(true);
}

void MMMEngine::EnemySpawner::SpawnArrowEnemy()
{
	if (ArrowEnemys.empty())
		return;
	auto obj = ArrowEnemys.front();
	ArrowEnemys.pop();

	if (!obj)
		return;
	obj->GetTransform()->SetWorldPosition(arrowspawnpos);
	obj->GetComponent<ArrowEnemy>()->ApplyStats();
	obj->SetActive(true);
}

void MMMEngine::EnemySpawner::SpawnThiefEnemy()
{
	if (ThiefEnemys.empty())
		return;
	auto obj = ThiefEnemys.front();
	ThiefEnemys.pop();

	if (!obj)
		return;
	obj->GetTransform()->SetWorldPosition(thiefspawnpos);
	obj->GetComponent<ThiefEnemy>()->ApplyStats();
	obj->SetActive(true);
}

void MMMEngine::EnemySpawner::ReturnEnemy(ObjPtr<GameObject> obj)
{
	if (obj->GetComponent<NormalEnemy>())
		NormalEnemys.push(obj);
	if (obj->GetComponent<ArrowEnemy>())
		ArrowEnemys.push(obj);
	if (obj->GetComponent<ThiefEnemy>())
		ThiefEnemys.push(obj);
}