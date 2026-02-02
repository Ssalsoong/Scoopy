#include "Building.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Buildingball.h"
#include "../Enemy/Enemy.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "StaticMesh.h"
#include "MMMTime.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<Building>("Building")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Building>"));

	registration::class_<ObjPtr<Building>>("ObjPtr<Building>")
		.constructor(
			[]() {
				return Object::NewObject<Building>();
			}).method("Inject", &ObjPtr<Building>::Inject);
}

void MMMEngine::Building::Start()
{
	buildingballmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/DefaultMesh/Sphere_StaticMesh.staticmesh");
	for (int i = 0; i < 10;++i)
	{
		auto obj = NewObject<GameObject>();
		obj->SetName("Buildingball");
		obj->SetTag("Buildingball");
		obj->GetTransform()->SetParent(GetTransform());
		obj->AddComponent<Buildingball>();
		obj->GetComponent<Buildingball>()->SetOwner(GetGameObject());
		obj->AddComponent<MeshRenderer>();
		obj->GetComponent<MeshRenderer>()->SetMesh(buildingballmesh);
		obj->GetTransform()->SetLocalPosition(0.f, 0.f, 0.f);
		obj->GetTransform()->SetWorldScale(0.2f, 0.2f, 0.2f);
		obj->SetActive(false);
		Buildingballs.push(obj);
	}
}

void MMMEngine::Building::Update()
{
	pos = GetTransform()->GetWorldPosition();
	CheckDead();
	CheckEnemy();
	AutoAttack();
}

void MMMEngine::Building::CheckDead()
{
	if (HP <= 0)
	{
		isDead = true;
		GetGameObject()->SetActive(false);
		return;
	}
}

void MMMEngine::Building::CheckEnemy()
{
	if (enemyTarget)
		return;
	auto enemys = GameObject::FindGameObjectsWithTag("Enemy");
	if (enemys.empty()) return;

	float bestD2 = attackdist * attackdist;
	ObjPtr<GameObject> best = nullptr;

	for (auto& e : enemys)
	{
		if (!e) continue;
		auto epos = e->GetTransform()->GetWorldPosition();
		float dx = epos.x - pos.x;
		float dz = epos.z - pos.z;
		float d2 = dx * dx + dz * dz;

		if (d2 < bestD2)
		{
			bestD2 = d2;
			best = e;
		}
	}
	if (best)
		enemyTarget = best;
}

void MMMEngine::Building::AutoAttack()
{
	if (point <= 0 || enemyTarget == nullptr)
		return;
	float bestD2 = attackdist * attackdist;
	auto epos = enemyTarget->GetTransform()->GetWorldPosition();
	float dx = epos.x - pos.x;
	float dz = epos.z - pos.z;
	float d2 = dx * dx + dz * dz;
	if (d2 > bestD2)
	{
		enemyTarget = nullptr;
		attackTimer = 0.0f;
		return;
	}
	if (attackTimer == 0.0f)
	{
		if (Buildingballs.empty())
			return;
		auto obj = Buildingballs.front();
		Buildingballs.pop();
		if (!obj)
			return;
		obj->SetActive(true);
		obj->GetComponent<Buildingball>()->SetTarget(enemyTarget);
		point--;
	}
	attackTimer += Time::GetDeltaTime();
	if (attackTimer >= attackDelay)
	{
		attackTimer = 0.0f;
	}
}

void MMMEngine::Building::ReturnBall(ObjPtr<GameObject> obj)
{
	Buildingballs.push(obj);
}

void MMMEngine::Building::PointUp(int t)
{
	point += t;
	exp += 10 * t;
}
