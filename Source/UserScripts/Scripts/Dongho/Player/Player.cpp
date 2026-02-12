#include "ScriptBehaviour.h"
#include "Player.h"
#include "MMMTime.h"
#include "MMMInput.h"
#include "Transform.h"
#include "../Manager/GameManager.h"
#include "../Snow/Snowball.h"
#include "../Building/BuildingPoint.h"
#include "../Manager/BuildingManager.h"
#include "../Manager/BattleManager.h"
#include "../Battlestats.h"
#include "../../test/PlayerController.h"
#include "../../Sunken/PlayerAnimController.h"
#include "../../test/PlayerMove.h"
#include "../../Sunken/ControlManager.h"

void MMMEngine::Player::Start()
{
	mPAController = GetComponent<PlayerAnimController>();

	if (!mPAController) {
		std::cout << "Player::PAController Not Found!!" << std::endl;
	}
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
	//���⼭ ��������� �� ������ ���ϰ� ���ƾ� ��
	if (GetComponent<PlayerController>()->IsHoldingSpace()) {
		mPAController->SetAttack(false);
		attackTimer = 0.0f;
		return;
	}
		

	auto enemies = GameObject::FindGameObjectsWithTag("Enemy");


	const float range = battledist;
	const float rangeSq = range * range;

	// �÷��̾� Forward (XZ ��� ����)
	Vector3 forward = -GetTransform()->GetWorldMatrix().Forward();
	forward.y = 0.0f;
	forward.Normalize();

	bool hasEnemyInRange = false;

	// ���� �� ���� �ִ��� üũ
	for (auto& e : enemies)
	{
		if (!e) continue;


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
		mPAController->SetAttack(false);
		return;
	}

	mPAController->SetAttack(true);
	attackTimer += Time::GetDeltaTime();

	if (attackTimer < attackDelay)
		return;

	attackTimer = 0.0f;

	for (auto& e : enemies)
	{
		if (!e) continue;

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
		float damage = atk + m_AttackBuffMax;
		if (criticalOn)
		{
			float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			if (r < 0.5f)
			{
				damage *= 2.0f;
			}
		}

		BattleManager::instance->Attack(GetGameObject(), e, damage);
	}
}


void MMMEngine::Player::AutoHeal()
{
	if (!GetComponent<Battlestats>())
		return;
	auto HP = GetComponent<Battlestats>()->GetHP();
	
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
	GetComponent<Battlestats>()->SetHP(HP);
}

void MMMEngine::Player::BuildOn()
{
	if (isBuildable) {
		auto input = ControlManager::Get();

		if (input->GetKeyDown(KeyCode::Space, 5))
			misInited = true;

		if (misInited && input->GetKey(KeyCode::Space, 5)) {
			mBuildElipsed += Time::GetDeltaTime();

			if (mBuildElipsed > mBuildTime) {
				misInited = false;

				auto buildingpoints = GetGameObject()->FindGameObjectsWithTag("BuildingPoint");
				for (auto& bp : buildingpoints)
				{
					if (bp->GetComponent<BuildingPoint>()->GetcanBuild()) {
						BuildingManager::instance->Build(bp);
						buildchance = false;
					}
				}
			}
		}
		else
			mBuildElipsed = 0.0f;
	}
}

void MMMEngine::Player::LevelUp()
{ 
	if (level >= 10)
		return;
	level++;
	maxpoint += 2;
	atk++;
	if (auto playerController = GetComponent<PlayerController>())
		playerController->SetMaxScoop(maxpoint - 1);
	if (auto playerMove = GetComponent<PlayerMove>())
		playerMove->SetSnowMaxScoop(maxpoint - 1);
	GameManager::instance->levelsum++;
}

void MMMEngine::Player::CalDamageDelay()
{
	if (damageTimer > 0.0f)
	{
		damageTimer = std::max(damageTimer - Time::GetDeltaTime(), 0.0f);
	}

}

void MMMEngine::Player::GetDamage(ObjPtr<GameObject>attacker, int t)
{
	if (damageTimer > 0.0f)
		return;
	auto stats = GetComponent<Battlestats>();
	if (!stats) return;
	stats->ApplyDamage(t);
	if (reflectOn)
		BattleManager::instance->Attack(GetGameObject(), attacker, t / 2);
	damageTimer = damageDelay;
}

void MMMEngine::Player::Dead()
{
	GameManager::instance->GameOver = true;
}

void MMMEngine::Player::Level5Apply(int value)
{
	if (value != 1 && value != 2)
		return;
	if (value == 1)
	{
		cosHalfFov = 0.17f;
		attackDelay = 0.4f;
	}
	if (value == 2)
	{
		criticalOn = true;
	}
}

void MMMEngine::Player::Level10Apply(int value)
{
	if (value != 1 && value != 2)
		return;

	if (value == 1)
	{
		reflectOn = true;
	}
	else if (value == 2)
	{
		maxHP = 150;
		GetComponent<Battlestats>()->SetHP(maxHP);
	}
}


void MMMEngine::Player::AddAttackBuffSource(const void* src, int value)
{
	m_AttackBuffSources[src] = value;
	RecalcAttackBuffMax();
}

void MMMEngine::Player::RemoveAttackBuffSource(const void* src)
{
	m_AttackBuffSources.erase(src);
	RecalcAttackBuffMax();
}

void MMMEngine::Player::UpdateAttackBuffSource(const void* src, int value)
{
	auto it = m_AttackBuffSources.find(src);
	if (it != m_AttackBuffSources.end())
	{
		it->second = value;
		RecalcAttackBuffMax();
	}
}

void MMMEngine::Player::RecalcAttackBuffMax()
{
	int best = 0; // 버프 없으면 0
	for (auto& [k, v] : m_AttackBuffSources)
		if (v > best) best = v;

	m_AttackBuffMax = best;
}

int MMMEngine::Player::GetAttackFinal() const
{
	return atk + m_AttackBuffMax;
}