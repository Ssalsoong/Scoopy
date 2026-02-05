#include "Export.h"
#include "ScriptBehaviour.h"
#include "Castle.h"
#include "MMMTime.h"
#include "StaticMesh.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "../Enemy/Enemy.h"
#include "Castleball.h"

void MMMEngine::Castle::Start()
{
	castleballmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/Snowball/snowball_StaticMesh.staticmesh");
	for (int i = 0; i < 10;++i)
	{
		auto obj = NewObject<GameObject>();
		obj->SetName("Castleball");
		obj->SetTag("Castleball");
		obj->GetTransform()->SetParent(GetTransform());
		obj->AddComponent<Castleball>();
		obj->GetComponent<Castleball>()->SetOwner(GetGameObject());
		obj->AddComponent<MeshRenderer>();
		obj->GetComponent<MeshRenderer>()->SetMesh(castleballmesh);
		obj->GetTransform()->SetLocalPosition(0.f,0.f,0.f);
		obj->GetTransform()->SetWorldScale(0.2f, 0.2f, 0.2f);
		obj->SetActive(false);
		Castleballs.push(obj);
	}
}

void MMMEngine::Castle::Update()
{
	pos = GetTransform()->GetWorldPosition();
	CheckEnemy();
	AutoAttack();
	AutoHeal();
}

void MMMEngine::Castle::CheckEnemy()
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

void MMMEngine::Castle::AutoAttack()
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
	attackTimer += Time::GetDeltaTime();
	if (attackTimer >= attackDelay)
	{
		if (Castleballs.empty()) {
			attackTimer = 0.0f;
			return;
		}
		auto obj = Castleballs.front();
		Castleballs.pop();
		if (!obj)
			return;
		obj->SetActive(true);
		obj->GetComponent<Castleball>()->SetTarget(enemyTarget);
		attackTimer = 0.0f;
		point --;
	}
}

void MMMEngine::Castle::ReturnBall(ObjPtr<GameObject> obj)
{
	Castleballs.push(obj);
}

void MMMEngine::Castle::AutoHeal()
{
	if (prevHP > HP)
	{
		fighting = true;
		NonfightTimer = 0.0f;
	}
	prevHP = HP;
	if (fighting)
	{
		NonfightTimer += Time::GetDeltaTime();
		if (NonfightTimer >= NonfightDelay)
		{
			fighting = false;
			healTimer = 0.0f;
		}
	}
	else if (HP < maxHP)
	{
		healTimer += Time::GetDeltaTime();
		if (healTimer >= healDelay)
		{
			HP = std::min(HP + healHP, maxHP);
			healTimer = 0.0f;
		}
	}
}

void MMMEngine::Castle::PointUp(int t)
{
	point += t;
	exp += 10 * t;
}

void MMMEngine::Castle::LevelUp()
{
	if (level >= 10)
		return;
	atk += 2;
}