#include "ScriptBehaviour.h"
#include "Player.h"
#include "MMMTime.h"
#include "MMMInput.h"
#include "Transform.h"
#include "../Enemy/Enemy.h"
#include "../Manager/GameManager.h"
#include "../Manager/SnowballManager.h"
#include "../Snow/Snowball.h"
#include "../Building/BuildingPoint.h"
#include "../Manager/BuildingManager.h"
#include "../Manager/BattleManager.h"
#include "../../test/PlayerMove.h"
#include "../Battlestats.h"

void MMMEngine::Player::Start()
{
}

void MMMEngine::Player::Update()
{
	pos = GetTransform()->GetWorldPosition();
	//HandleMovement();
	Velocitydown();
	if(buildchance)
		BuildOn();
	UpdateScoop();
	HandleAttack();
	AutoHeal();
	CalDamageDelay();
}

// 임시 이동이므로 이후 반드시 삭제
void MMMEngine::Player::HandleMovement()
{
	float dx = 0.0f;
	float dz = 0.0f;
	if (Input::GetKey(KeyCode::LeftArrow))  dx -= 1.0f;
	if (Input::GetKey(KeyCode::RightArrow)) dx += 1.0f;
	if (Input::GetKey(KeyCode::UpArrow))    dz += 1.0f;
	if (Input::GetKey(KeyCode::DownArrow))  dz -= 1.0f;
	isMoving = (dx != 0.0f || dz != 0.0f);
	if (isMoving) {
		float len = sqrtf(dx * dx + dz * dz);
		dx /= len;
		dz /= len;

		pos.x += dx * (velocity - velocitydown) * Time::GetDeltaTime();
		pos.z += dz * (velocity - velocitydown) * Time::GetDeltaTime();
		GetTransform()->SetWorldPosition(pos);
	}
}

void MMMEngine::Player::UpdateScoop()
{
	if (Input::GetKeyDown(KeyCode::Space)) {
		SnowballManager::instance->OnScoopStart(*this);
		scoopHeld = true;
	}
	if (Input::GetKey(KeyCode::Space)) {
		SnowballManager::instance->OnScoopHold(*this);
	}
	if (Input::GetKeyUp(KeyCode::Space)) {
		SnowballManager::instance->OnScoopEnd(*this);
		scoopHeld = false;
	}
}

bool MMMEngine::Player::AttachSnowball(ObjPtr<GameObject> snow)
{
	if (!snow) return false;

	if (matchedSnowball == snow)
		return true;
	if (matchedSnowball) DetachSnowball();
	matchedSnowball = snow;
	matchedSnowball->GetTransform()->SetParent(GetTransform());
	float scale = matchedSnowball->GetComponent<Snowball>()->GetScale();
	float distance = baseRadius * scale * k;
	matchedSnowball->GetTransform()->SetLocalPosition(DirectX::SimpleMath::Vector3::Backward * distance);
	return true;
}

void MMMEngine::Player::DetachSnowball()
{
	if (!matchedSnowball) return;
	matchedSnowball->GetTransform()->SetParent(nullptr);
	matchedSnowball = nullptr;
	
	auto t_move = GetComponent<PlayerMove>();
	t_move->SetScoopMode(false, nullptr);
	
}

void MMMEngine::Player::SnapToSnowball(ObjPtr<GameObject> snow)
{
	if (!snow) return;

	auto tr = GetTransform();
	auto sTr = snow->GetTransform();
	if (!tr || !sTr) return;

	auto sc = snow->GetComponent<Snowball>();
	if (!sc) return;

	auto snowPos = sTr->GetWorldPosition();

	// 1) 먼저 바라보기
	LookAt(snowPos);

	// 2) 붙을 위치 계산 (dir 기반)
	auto playerPos = tr->GetWorldPosition();
	auto dir = snowPos - playerPos;
	dir.y = 0.0f;
	if (dir.LengthSquared() < 1e-8f) return;
	dir.Normalize();

	float distance = baseRadius * sc->GetScale() * k;

	// 플레이어는 눈에서 distance만큼 뒤로 물러난 위치로
	auto targetPos = snowPos - dir * distance;

	playerPos.x = targetPos.x;
	playerPos.z = targetPos.z;
	tr->SetWorldPosition(playerPos);
}

void MMMEngine::Player::LookAt(const DirectX::SimpleMath::Vector3& target)
{
	auto dir = target - pos;
	dir.y = 0.0f;

	float len2 = dir.LengthSquared();
	if (len2 < 1e-8f) return;

	dir.Normalize();

	float yaw = atan2f(dir.x, dir.z);
	auto rot = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, 0, 0);
	GetTransform()->SetWorldRotation(rot);
}

void MMMEngine::Player::HandleAttack()
{
	if (scoopHeld)
		return;
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

void MMMEngine::Player::Velocitydown()
{
	if (GetMatchedSnowball())
	{
		velocitydown = matchedSnowball->GetComponent<Snowball>()->GetPoint() * 0.13;
	}
}

void MMMEngine::Player::LevelUp()
{ 
	if (level >= 10)
		return;
	level += 1;
	maxpoint += 2;
	atk += 1;
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