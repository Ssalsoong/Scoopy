#include "Export.h"
#include "ScriptBehaviour.h"
#include "EnemyController.h"
#include "Transform.h"
#include "EnemySensor.h"
#include "EnemyMove.h"
#include "../Dongho/Snow/Snowball.h"
#include "../Dongho/Manager/BattleManager.h"
#include "../Dongho/Enemy/ArrowEnemy.h"
#include "MMMTime.h"
#include "../Dongho/Battlestats.h"
#include "../Dongho/Manager/EnemySpawner.h"

void MMMEngine::EnemyController::Start()
{
	m_Sensor = GetComponent<EnemySensor>();
	m_Move = GetComponent<EnemyMove>();

	if (auto go = GameObject::Find("Castle"); go.IsValid())
	{
		m_MainTarget = go;
	}
}

void MMMEngine::EnemyController::Update()
{
	UpdateTarget();
	UpdateDistance();
	ChangeState();
	CheckState();
}


//Enemy 초기값 설정용
void MMMEngine::EnemyController::InitEnemy(EnemyType type, DirectX::SimpleMath::Vector3 pos, int hp)
{
	GetTransform()->SetWorldPosition(pos);
	m_Sensor = GetComponent<EnemySensor>();
	m_Move->ResetPos(pos);
	m_Move = GetComponent<EnemyMove>();
	m_Move->ChangeTarget(m_MainTarget);

	curState = EnemyState::Move;
	prevState = EnemyState::Dead;

	m_EnemyType = type;


	m_CurTarget = m_MainTarget;
	m_Move->ChangeTarget(m_CurTarget);
	m_Move->MoveTriggerSet(true);

	auto HPstats = GetComponent<Battlestats>();
	HPstats->SetHP(hp);
	switch (type)
	{
	case EnemyType::Warrior:
	{
		m_Move->SetEnemySpeed(250.f);
		E_state.AD = 4.f;
		E_state.AS = 0.65f;
		E_state.Range = 0.3f;
		break;
	}

	case EnemyType::Archer:
	{
		m_Move->SetEnemySpeed(250.f);
		E_state.AD = 2.f;
		E_state.AS = 0.4f;
		E_state.Range = 2.0f;
		break;
	}

	case EnemyType::Scout:
	{
		m_Move->SetEnemySpeed(270.f);
		E_state.AD = 3.f;
		E_state.AS = 0.65f;
		E_state.Range = 0.3f;
		break;
	}
	}
}


void MMMEngine::EnemyController::UpdateDistance()
{
	if (m_CurTarget == nullptr) m_CurTarget = m_MainTarget;

	auto EnemyPos = GetTransform()->GetWorldPosition();
	auto TargetPos = m_CurTarget->GetTransform()->GetWorldPosition();
	toTarget = TargetPos - EnemyPos;
	distance = toTarget.Length();
}


void MMMEngine::EnemyController::ChangeState()
{
    prevState = curState;

    float extra = 0.0f;
    const auto& tag = m_CurTarget->GetTag();

    if (tag == "Castle") extra = 0.8f;
    else if (tag == "Tower") extra = 0.4f;
    else if (tag == "Player") extra = 0.25f;
    else if (tag == "Snow") extra = 0.6f;

    curState = (distance <= (E_state.Range + extra)) ? EnemyState::Attack : EnemyState::Move;
}

void MMMEngine::EnemyController::CheckState()
{

	if (prevState != curState)
	{
		OnStateEnter(curState);
	}
}


void MMMEngine::EnemyController::OnStateEnter(EnemyState state)
{
	switch (state)
	{
	case EnemyState::Move:
	{
		m_Move->ChangeTarget(m_CurTarget);
		m_Move->MoveTriggerSet(true);
		break;
	}
	case EnemyState::Attack:
	{
		m_Move->MoveTriggerSet(false);
		//Todo :: 공격관련
		break;
	}
	case EnemyState::Dead:
	{
		m_Move->SetEnemySpeed(0.f);
		m_Move->MoveTriggerSet(false);
		GetTransform()->SetWorldPosition(200.f, 200.f, 200.f);
		attackTimer = 0.0f;
		EnemySpawner::instance->EnemyDeath(GetGameObject());
		GetGameObject()->SetActive(false);
		break;
	}
	}
}

bool MMMEngine::EnemyController::UpdateTarget()
{
	ObjPtr<GameObject> target = m_Sensor->GetTarget();
	if (target.IsValid())
	{
		if (m_CurTarget != target)
		{
			m_CurTarget = target;
			m_Move->ChangeTarget(m_CurTarget); // 여기서 바로 갱신
		}
		return true;
	}
	return false;
}

bool MMMEngine::EnemyController::CheckHurt()
{
	return EnemyHurt;
}

void MMMEngine::EnemyController::OnHurtFlag(bool value)
{
	EnemyHurt = value;
}

void MMMEngine::EnemyController::AttackTarget()
{
	if (curState != EnemyState::Attack)
		return;
	attackTimer += Time::GetDeltaTime();
	if (attackTimer >= E_state.AS)
	{
		if (m_CurTarget->GetName() == "Snow")
		{
			m_CurTarget->GetComponent<Snowball>()->lifecount--;
			return;
		}
		if (auto arrowenemy = GetComponent<ArrowEnemy>())
			arrowenemy->ArrowAttack(m_CurTarget);
		BattleManager::instance->Attack(GetGameObject(), m_CurTarget, E_state.AD);
		attackTimer == 0.0f;
	}
}

void MMMEngine::EnemyController::CalSnowDamageDelay()
{
	if (snowDamageTimer > 0.0f)
	{
		snowDamageTimer = std::max(snowDamageTimer - Time::GetDeltaTime(), 0.0f);
	}
}

bool MMMEngine::EnemyController::ApplySnowDamage()
{
	if (snowDamageTimer > 0.0f) return false;
	snowDamageTimer = snowDamageDelay;
	return true;
}