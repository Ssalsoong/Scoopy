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

	CalSnowDamageDelay();

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
		RequestState(EnemyState::GoToBuilding);
		if (state == EnemyState::GoToBuilding)
		{
			GoToBuilding();
			return;
		}
	}


	if (state != EnemyState::AttackPlayer && state != EnemyState::ChasePlayer)
	{
		if (HitByPlayer)
		{
			HitByPlayer = false;
			RequestState(EnemyState::ChasePlayer);
		}
		else if(CheckPlayer())
			RequestState(EnemyState::ChasePlayer);
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
	if (next == EnemyState::GoToCastle || next == EnemyState::ChasePlayer || next == EnemyState::GoToBuilding)
	{
		stateLocked = false;
		pendingState = EnemyState::None;
	}
	else if (next == EnemyState::AttackCastle || next == EnemyState::AttackPlayer || next == EnemyState::AttackBuilding)
	{
		stateLocked = true;
		pendingState = EnemyState::None;
	}
}

void MMMEngine::Enemy::RequestState(EnemyState next)
{
	if (state == next)
		return;

	if (stateLocked)
	{
		pendingState = next;
		return;
	}
	ChangeState(next);
}

void MMMEngine::Enemy::FinishAttackCycle()
{
	attackTimer = 0.0f;
	stateLocked = false;

	if (pendingState != EnemyState::None)
	{
		EnemyState next = pendingState;
		pendingState = EnemyState::None;
		ChangeState(next);
	}
	else
		stateLocked = true;
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
		if (pendingState == EnemyState::None) {
			if (GetComponent<ArrowEnemy>())
				GetComponent<ArrowEnemy>()->ArrowAttack(castle);
			else
				BattleManager::instance->Attack(castle, atk);
		}
		FinishAttackCycle();
	}
}

void MMMEngine::Enemy::ChasePlayer()
{
	if (LostPlayer())
	{
		RequestState(EnemyState::GoToCastle);
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
		RequestState(EnemyState::GoToCastle);
	}
	LookAt(playerpos);
	float dx = playerpos.x - pos.x;
	float dz = playerpos.z - pos.z;
	float d2 = dx * dx + dz * dz;
	if (d2 > battledist * battledist)
	{
		RequestState(EnemyState::ChasePlayer);
	}
	attackTimer += Time::GetDeltaTime();
	if (attackTimer >= attackDelay)
	{
		if (pendingState == EnemyState::None) {
			if (GetComponent<ArrowEnemy>())
				GetComponent<ArrowEnemy>()->ArrowAttack(player);
			else
				BattleManager::instance->Attack(player, atk);
		}
		FinishAttackCycle();
	}

}

void MMMEngine::Enemy::GoToBuilding()
{
	if (!buildingTarget.IsValid() ||
		buildingTarget->GetComponent<Building>() == nullptr ||
		buildingTarget->GetComponent<Building>()->isDead)
	{
		buildingTarget = nullptr;
		RequestState(EnemyState::GoToCastle);
		return;
	}
	bool moving = MoveToTarget(buildingpos, battledist);
	if (!moving)
		ChangeState(EnemyState::AttackBuilding);
}

void MMMEngine::Enemy::AttackBuilding()
{
	bool invalid = false;
	if (!buildingTarget.IsValid())
	{
		RequestState(EnemyState::GoToCastle);
		invalid = true;
	}
	else {
		auto b = buildingTarget->GetComponent<Building>();
		if (!b || b->isDead)
		{
			RequestState(EnemyState::GoToCastle);
			invalid = true;
		}
	}
	if(!invalid)
		LookAt(buildingpos);
	attackTimer += Time::GetDeltaTime();
	if (attackTimer >= attackDelay)
	{
		if (!invalid && pendingState == EnemyState::None) {
			if (GetComponent<ArrowEnemy>())
				GetComponent<ArrowEnemy>()->ArrowAttack(buildingTarget);
			else
				BattleManager::instance->Attack(buildingTarget, atk);
		}
		if (invalid) buildingTarget = nullptr;
		FinishAttackCycle();
	}
}

void MMMEngine::Enemy::Dead()
{
	GetTransform()->SetWorldPosition(200.f, 200.f, 200.f);
	ChangeState(EnemyState::GoToCastle);
	pendingState = EnemyState::None;
	attackTimer = 0.0f;
	HitByPlayer = false;
	buildingTarget = nullptr;
	EnemySpawner::instance->EnemyDeath(GetGameObject());
	GetGameObject()->SetActive(false);
	GetComponent<Battlestats>()->HP = 1;
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