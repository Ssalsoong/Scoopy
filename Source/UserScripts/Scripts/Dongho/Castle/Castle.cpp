#include "Export.h"
#include "ScriptBehaviour.h"
#include "Castle.h"
#include "MMMTime.h"
#include "StaticMesh.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "../Enemy/Enemy.h"
#include "Castleball.h"
#include "../Manager/GameManager.h"
#include "../Battlestats.h"
#include "../../test/SnowBullet.h"
#include "../Manager/BuildingManager.h"

void MMMEngine::Castle::Start()
{
	for (int i = 0; i < 10;++i)
	{
		auto obj = Instantiate(pre_bullet);
		obj->GetTransform()->SetWorldPosition(0.f, 0.f, 0.f);
		obj->GetComponent<Castleball>()->SetOwner(GetGameObject());
		obj->SetActive(false);
		Castleballs.push(obj);
	}
	pos = GetTransform()->GetWorldPosition();
}

void MMMEngine::Castle::Update()
{
	CheckEnemy();
	if (doubleattack)
		CheckSecondEnemy();
	AutoAttack();
	AutoHeal();
	if (canshield)
		CalShieldDelay();
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

void MMMEngine::Castle::CheckSecondEnemy()
{
	if (!doubleattack || !enemyTarget)
	{
		enemyTarget2 = nullptr;
		return;
	}
	float maxD2 = attackdist * attackdist;
	if (enemyTarget2)
	{
		if (enemyTarget2 == enemyTarget)
		{
			enemyTarget2 = nullptr;
			return;
		}
		auto epos = enemyTarget2->GetTransform()->GetWorldPosition();
		float dx = epos.x - pos.x;
		float dz = epos.z - pos.z;
		float d2 = dx * dx + dz * dz;
		if (d2 <= maxD2)
			return;
		enemyTarget2 = nullptr;
	}
	auto enemys = GameObject::FindGameObjectsWithTag("Enemy");
	if (enemys.empty())
	{
		enemyTarget2 = nullptr;
		return;
	}
	ObjPtr<GameObject> best = nullptr;
	float bestD2 = maxD2;

	for (auto& e : enemys)
	{
		if (!e) continue;
		if (e == enemyTarget) continue;
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
	enemyTarget2 = best;
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
		enemyTarget2 = nullptr;
		attackTimer = 0.0f;
		return;
	}
	attackTimer += Time::GetDeltaTime();
	if (attackTimer >= attackDelay)
	{
		FireAt(enemyTarget);
		if (doubleattack && enemyTarget2)
			FireAt(enemyTarget2);
		attackTimer = 0.0f;
	}
}

void MMMEngine::Castle::FireAt(ObjPtr<GameObject>target)
{
	if (Castleballs.empty()) {
		return;
	}
	auto obj = Castleballs.front();
	Castleballs.pop();
	if (!obj)
		return;
	obj->SetActive(true);
	obj->GetComponent<Castleball>()->SetTarget(target);
	obj->GetComponent<Castleball>()->Setatk(atk);
	auto bulletpos = pos;
	bulletpos.y = 1.5f;
	obj->GetComponent<SnowBullet>()->StartBullet(bulletpos, bulletsize, bulletSpeed, target);
	point--;
}

void MMMEngine::Castle::ReturnBall(ObjPtr<GameObject> obj)
{
	Castleballs.push(obj);
}

void MMMEngine::Castle::AutoHeal()
{
	auto HP = GetComponent<Battlestats>()->GetHP();
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
	GetComponent<Battlestats>()->SetHP(HP);
}

void MMMEngine::Castle::CalShieldDelay()
{
	if (shieldTimer > 0.0f)
	{
		shieldTimer = std::max(shieldTimer - Time::GetDeltaTime(), 0.0f);
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
	level ++;
	atk += 3;
	GameManager::instance->levelsum++;
}

void MMMEngine::Castle::GetDamage(ObjPtr<GameObject>attacker, int t)
{
	auto stats = GetComponent<Battlestats>();
	if (!stats) return;
	if (!canshield)
		stats->ApplyDamage(t);
	else
	{
		if (shieldTimer > 0.0f)
			return;
		stats->ApplyDamage(t);
	}
}

void MMMEngine::Castle::Dead()
{
	GameManager::instance->GameOver = true;
}

void MMMEngine::Castle::Level5Apply(int value)
{
	if (value != 1 && value != 2)
		return;
	if (value == 1)
	{
		waveexp = true;
	}
	else if (value == 2)
	{
		attackdist = 7.5f;
		BuildingManager::instance->BuildingsDistUP();
	}
}

void MMMEngine::Castle::Level10Apply(int value)
{
	if (value != 1 && value != 2)
		return;
	if (value == 1)
	{
		canshield = true;
	}
	else if (value == 2)
	{
		doubleattack = true;
	}
}