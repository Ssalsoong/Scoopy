#include "Enemy.h"
#include "../Player/Player.h"
#include "../Castle/Castle.h"
#include "../Building/Building.h"
#include "../Manager/EnemySpawner.h"
#include "MMMTime.h"
#include "Transform.h"
#include "ArrowEnemy.h"
#include "../Manager/BattleManager.h"
#include "../Battlestats.h"
#include "../../test/EnemyController.h"
#include "../Snow/Snowball.h"

void MMMEngine::Enemy::Start()
{

}


void MMMEngine::Enemy::Update()
{
	CalSnowDamageDelay();
}




void MMMEngine::Enemy::Dead()
{
	GetTransform()->SetWorldPosition(200.f, 200.f, 200.f);
	attackTimer = 0.0f;
	HitByPlayer = false;
	EnemySpawner::instance->EnemyDeath(GetGameObject());
	GetGameObject()->SetActive(false);
}


void MMMEngine::Enemy::PlayerHitMe()
{
	HitByPlayer = true;
}

void MMMEngine::Enemy::CalSnowDamageDelay()
{
	if (snowDamageTimer > 0.0f)
	{
		snowDamageTimer = std::max(snowDamageTimer - Time::GetDeltaTime(), 0.0f);
	}
}

bool MMMEngine::Enemy::ApplySnowDamage()
{
	if (snowDamageTimer > 0.0f) return false;
	snowDamageTimer = snowDamageDelay;
	return true;
}


void MMMEngine::Enemy::AttackTarget(ObjPtr<GameObject> target)
{
	attackTimer += Time::GetDeltaTime();
	if (attackTimer >= attackDelay)
	{
		if (target->GetName() == "Snow")
		{
			target->GetComponent<Snowball>()->lifecount--;
			return;
		}
		if (auto arrowenemy = GetComponent<ArrowEnemy>())
			arrowenemy->ArrowAttack(target);
		BattleManager::instance->Attack(GetGameObject(), target, atk);
		attackTimer == 0.0f;
	}
}