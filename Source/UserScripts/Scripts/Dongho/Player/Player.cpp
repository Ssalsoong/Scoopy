#include "ScriptBehaviour.h"
#include "Player.h"
#include "MMMTime.h"
#include "MMMInput.h"
#include "Transform.h"
#include "../Enemy/Enemy.h"
#include "../Manager/GameManager.h"
#include "../Snow/Snowball.h"
#include "../Building/BuildingPoint.h"
#include "../Manager/BuildingManager.h"
#include "../Manager/BattleManager.h"
#include "../Battlestats.h"

void MMMEngine::Player::Start()
{
}

void MMMEngine::Player::Update()
{
	pos = GetTransform()->GetWorldPosition();
	if(buildchance)
		BuildOn();
	HandleAttack();
	AutoHeal();
	CalDamageDelay();
}


void MMMEngine::Player::HandleAttack()
{
	//여기서 스쿱상태일 때 공격을 못하게 막아야 함

	auto enemies = GameObject::FindGameObjectsWithTag("Enemy");


	const float range = battledist;
	const float rangeSq = range * range;

	const float cosHalfFov = 0.5f;

	// 플레이어 Forward (XZ 평면 기준)
	Vector3 forward = -GetTransform()->GetWorldMatrix().Forward();
	forward.y = 0.0f;
	forward.Normalize();

	bool hasEnemyInRange = false;

	// 범위 안 적이 있는지 체크
	for (auto& e : enemies)
	{
		if (!e) continue;

		auto tec = e->GetComponent<Enemy>();
		if (!tec) continue;

		auto tr = e->GetTransform();
		if (!tr) continue;

		auto p = tr->GetWorldPosition();
		Vector3 toEnemy = p - pos;
		toEnemy.y = 0.0f;

		float distSq = toEnemy.LengthSquared();
		if (distSq > rangeSq)
			continue;

		toEnemy.Normalize();

		float dot = forward.Dot(toEnemy);
		if (dot < cosHalfFov)
			continue;

		hasEnemyInRange = true;
		break;
	}

	if (!hasEnemyInRange)
	{
		attackTimer = 0.0f;
		return;
	}

	attackTimer += Time::GetDeltaTime();

	if (attackTimer < attackDelay)
		return;

	attackTimer = 0.0f;

	for (auto& e : enemies)
	{
		auto tec = e->GetComponent<Enemy>();
		if (!tec) continue;

		auto tr = e->GetTransform();
		if (!tr) continue;

		auto p = tr->GetWorldPosition();

		Vector3 toEnemy = p - pos;
		toEnemy.y = 0.0f;

		float distSq = toEnemy.LengthSquared();
		if (distSq > rangeSq)
			continue;

		toEnemy.Normalize();

		float dot = forward.Dot(toEnemy);
		if (dot < cosHalfFov)
			continue;

		BattleManager::instance->Attack(e, atk);
		tec->PlayerHitMe();
	}
}


void MMMEngine::Player::AutoHeal()
{
	if (!GetComponent<Battlestats>())
		return;
	auto HP = GetComponent<Battlestats>()->HP;
	
	if (prevHP > HP)
	{
		fighting = true;
		nonfightTimer = 0.0f;
	}
	prevHP = HP;
	if (fighting)
	{
		nonfightTimer += Time::GetDeltaTime();
		if (nonfightTimer >= nonfightDelay)
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
	GetComponent<Battlestats>()->HP = HP;
}

void MMMEngine::Player::BuildOn()
{
	if (Input::GetKeyDown(KeyCode::LeftControl))
	{
		auto buildingpoints = GetGameObject()->FindGameObjectsWithTag("BuildingPoint");
		for (auto& bp : buildingpoints)
		{
			if (bp->GetComponent<BuildingPoint>()->canBuild) {
				BuildingManager::instance->Build(bp);
				buildchance = false;
			}
		}
		
	}
}

void MMMEngine::Player::LevelUp()
{ 
	if (level >= 10)
		return;
	level ++;
	maxpoint += 2;
	atk ++;
	GameManager::instance->levelsum++;
}

void MMMEngine::Player::CalDamageDelay()
{
	if (damageTimer > 0.0f)
	{
		damageTimer = std::max(damageTimer - Time::GetDeltaTime(), 0.0f);
	}

}

void MMMEngine::Player::GetDamage(int t)
{
	if (damageTimer > 0.0f)
		return;
	auto stats = GetComponent<Battlestats>();
	if (!stats) return;
	if (stats->HP <= 0)
		return;
	stats->HP = std::max(stats->HP - t, 0);

	damageTimer = damageDelay;
}

void MMMEngine::Player::Dead()
{
	GameManager::instance->GameOver = true;
}