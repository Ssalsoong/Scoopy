#include "Enemy.h"
#include "../Player/Player.h"
#include "../Castle/Castle.h"
#include "../Building/Building.h"
#include "../Manager/EnemySpawner.h"
#include "MMMTime.h"
#include "Transform.h"
#include "ArrowEnemy.h"
#include "../Manager/BattleManager.h"

void MMMEngine::Enemy::Start()
{
	tr = GetTransform();
	player = GameObject::Find("Player");
	if (player)
		playertr = player->GetTransform();

	castle = GameObject::Find("Castle");
	if (castle)
		castletr = castle->GetTransform();
}


void MMMEngine::Enemy::Update()
{
	if (!tr || !playertr || !castletr) return;
	pos = tr->GetWorldPosition();
	playerpos = playertr->GetWorldPosition();
	castlepos = castletr->GetWorldPosition();

	if (HP <= 0) ChangeState(EnemyState::Dead);

	if (state == EnemyState::AttackBuilding)
	{
		AttackBuilding();
		return;
	}

	if (state == EnemyState::GoToBuilding)
	{
		GoToBuilding();
		return;
	}

	if (state == EnemyState::Dead)
	{
		Dead();
		return;
	}

	if (FindNearBuilding())
	{
		ChangeState(EnemyState::GoToBuilding);
		GoToBuilding();
		return;
	}


	if (state != EnemyState::AttackPlayer && state != EnemyState::ChasePlayer)
	{
		if (HitByPlayer)
		{
			HitByPlayer = false;
			ChangeState(EnemyState::ChasePlayer);
		}
		else if(CheckPlayer())
			ChangeState(EnemyState::ChasePlayer);
	}

	switch (state)
	{
	case MMMEngine::Enemy::EnemyState::GoToCastle:
		GoToCastle(); break;
	case MMMEngine::Enemy::EnemyState::AttackCastle:
		AttackCastle(); break;
	case MMMEngine::Enemy::EnemyState::ChasePlayer:
		ChasePlayer(); break;
	case MMMEngine::Enemy::EnemyState::AttackPlayer:
		AttackPlayer(); break;
	default:
		GoToCastle(); break;
	}
}

void MMMEngine::Enemy::ChangeState(EnemyState next)
{
	if (state == next) return;
	state = next;

	attackTimer = 0.0f;
}

void MMMEngine::Enemy::GoToCastle()
{
	bool moving = MoveToTarget(castlepos, battledist+0.5f);
	if (!moving)
		ChangeState(EnemyState::AttackCastle);
}

void MMMEngine::Enemy::AttackCastle()
{
	LookAt(castlepos);
	attackTimer += Time::GetDeltaTime();
	if (attackTimer >= attackDelay)
	{
		if (GetComponent<ArrowEnemy>())
			GetComponent<ArrowEnemy>()->ArrowAttack(castle);
		else
			BattleManager::instance->Attack(castle, atk);
		attackTimer = 0.0f;
	}
}

void MMMEngine::Enemy::ChasePlayer()
{
	if (LostPlayer())
	{
		ChangeState(EnemyState::GoToCastle);
		return;
	}
	bool moving = MoveToTarget(playerpos, battledist);
	if (!moving)
		ChangeState(EnemyState::AttackPlayer);
}

void MMMEngine::Enemy::AttackPlayer()
{
	if (LostPlayer())
	{
		ChangeState(EnemyState::GoToCastle);
		return;
	}
	LookAt(playerpos);
	float dx = playerpos.x - pos.x;
	float dz = playerpos.z - pos.z;
	float d2 = dx * dx + dz * dz;
	if (d2 > battledist * battledist)
	{
		ChangeState(EnemyState::ChasePlayer);
		return;
	}
	attackTimer += Time::GetDeltaTime();
	if (attackTimer >= attackDelay)
	{
		if (GetComponent<ArrowEnemy>())
			GetComponent<ArrowEnemy>()->ArrowAttack(player);
		else
			BattleManager::instance->Attack(player, atk);
		attackTimer = 0.0f;
	}

}

void MMMEngine::Enemy::GoToBuilding()
{
	if (!buildingTarget.IsValid() ||
		buildingTarget->GetComponent<Building>() == nullptr ||
		buildingTarget->GetComponent<Building>()->isDead)
	{
		buildingTarget = nullptr;
		ChangeState(EnemyState::GoToCastle);
		return;
	}
	bool moving = MoveToTarget(buildingpos, battledist);
	if (!moving)
		ChangeState(EnemyState::AttackBuilding);
}

void MMMEngine::Enemy::AttackBuilding()
{
	if (!buildingTarget.IsValid() ||
		buildingTarget->GetComponent<Building>() == nullptr ||
		buildingTarget->GetComponent<Building>()->isDead)
	{
		buildingTarget = nullptr;
		ChangeState(EnemyState::GoToCastle);
		return;
	}

	auto b = buildingTarget->GetComponent<Building>();
	if (!b || b->isDead)
	{
		buildingTarget = nullptr;
		ChangeState(EnemyState::GoToCastle);
		return;
	}
	LookAt(buildingpos);
	attackTimer += Time::GetDeltaTime();
	if (attackTimer >= attackDelay)
	{
		if (GetComponent<ArrowEnemy>())
			GetComponent<ArrowEnemy>()->ArrowAttack(buildingTarget);
		else
			BattleManager::instance->Attack(buildingTarget, atk);
		attackTimer = 0.0f;
	}
}

void MMMEngine::Enemy::Dead()
{
	GetTransform()->SetWorldPosition(200.f, 200.f, 200.f);
	ChangeState(EnemyState::GoToCastle);
	EnemySpawner::instance->EnemyDeath(GetGameObject());
	GetGameObject()->SetActive(false);
}

bool MMMEngine::Enemy::FindNearBuilding()
{
	auto buildings = GameObject::FindGameObjectsWithTag("Building");
	if (buildings.empty()) return false;

	float bestD2 = checkdist * checkdist;
	ObjPtr<GameObject> best = nullptr;

	for (auto& b : buildings)
	{
		if (!b) continue;
		auto bpos = b->GetTransform()->GetWorldPosition();
		float dx = bpos.x - pos.x;
		float dz = bpos.z - pos.z;
		float d2 = dx * dx + dz * dz;

		if (d2 < bestD2)
		{
			bestD2 = d2;
			best = b;
		}
	}
	if (best)
	{
		if (!best->GetComponent<Building>()->isDead) {
			buildingTarget = best;
			buildingpos = buildingTarget->GetTransform()->GetWorldPosition();
			return true;
		}
	}
	return false;
}

void MMMEngine::Enemy::LookAt(const DirectX::SimpleMath::Vector3& target)
{
	auto dir = target - pos;
	dir.y = 0.0f;

	float len2 = dir.LengthSquared();
	if (len2 < 1e-8f) return;

	dir.Normalize();

	float yaw = atan2f(dir.x, dir.z);
	auto rot = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, 0, 0);
	tr->SetWorldRotation(rot);
}

//이 함수 내용 전체 다 물리 기반으로 바꿔야 함
bool MMMEngine::Enemy::MoveToTarget(const DirectX::SimpleMath::Vector3& target, float stopDist)
{
	float dx = target.x - pos.x;
	float dz = target.z - pos.z;

	float dist2 = dx * dx + dz * dz;
	if (dist2 <= stopDist * stopDist)
		return false; // 도착(또는 충분히 가까움)

	float dist = sqrtf(dist2);
	dx /= dist; dz /= dist;

	pos.x += dx * velocity * Time::GetDeltaTime();
	pos.z += dz * velocity * Time::GetDeltaTime();
	tr->SetWorldPosition(pos);

	float yaw = atan2f(dx, dz);
	auto rot = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, 0, 0);
	tr->SetWorldRotation(rot);
	return true;
}

bool MMMEngine::Enemy::LostPlayer()
{
	float dx = playerpos.x - pos.x;
	float dz = playerpos.z - pos.z;
	float dist2 = dx * dx + dz * dz;

	if (dist2 > checkdist * checkdist) {
		return true;
	}
	return false;
}

bool MMMEngine::Enemy::CheckPlayer()
{
	auto fwd = -tr->GetWorldMatrix().Forward();
	fwd.y = 0.0f;
	if (fwd.LengthSquared() < 1e-8f) return false;
	fwd.Normalize();

	float vx = playerpos.x - pos.x;
	float vz = playerpos.z - pos.z;
	float pd2 = vx * vx + vz * vz;
	if (pd2 < checkdist * checkdist)
	{
		float inv = 1.0f / sqrtf(pd2);
		vx *= inv; vz *= inv;
		float dot = fwd.x * vx + fwd.z * vz;

		if (dot >= 0.5f)
		{
			return true;
		}
	}
	return false;
}

void MMMEngine::Enemy::PlayerHitMe()
{
	if( state!=EnemyState::AttackPlayer && state!=EnemyState::ChasePlayer )
		HitByPlayer = true;
}