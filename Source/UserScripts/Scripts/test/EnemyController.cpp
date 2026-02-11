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
#include "SphereColliderComponent.h"
#include "SnowCollider.h"

void MMMEngine::EnemyController::Start()
{
	m_Sensor = m_SensorObj->GetComponent<EnemySensor>();
	m_Move = GetComponent<EnemyMove>();

	//if (auto TriggerCol = GetComponent<SphereColliderComponent>(); TriggerCol.IsValid())
	//{
	//	TriggerCol->SetTriggerQueryEnabled(false);
	//}
	

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

	AttackTarget();
	CalSnowDamageDelay();
}


//Enemy 초기값 설정용
void MMMEngine::EnemyController::InitEnemy(EnemyType type, DirectX::SimpleMath::Vector3 pos, int hp)
{
	GetTransform()->SetWorldPosition(pos);
	m_Sensor = m_SensorObj->GetComponent<EnemySensor>();
	m_Move = GetComponent<EnemyMove>();
	m_Move->ResetPos(pos);
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
		E_state.AD = 4;
		E_state.AS = 0.65f;
		E_state.Range = 0.3f;
		break;
	}

	case EnemyType::Archer:
	{
		m_Move->SetEnemySpeed(250.f);
		E_state.AD = 2;
		E_state.AS = 0.4f;
		E_state.Range = 2.0f;
		break;
	}

	case EnemyType::Scout:
	{
		m_Move->SetEnemySpeed(270.f);
		E_state.AD = 3;
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
	if (curState == EnemyState::Attack && attackTimer > 0.0f)
		return;
    prevState = curState;

    float extra = 0.0f;
    const auto& tag = m_CurTarget->GetTag();

    if (tag == "Castle"){ extra = 0.4f;}
    else if (tag == "Building") extra = 0.3f;
    else if (tag == "Player") extra = 0.2f;
    else if (tag == "Snow") extra = 0.6f;

    curState = (distance <= (E_state.Range + extra)) ? EnemyState::Attack : EnemyState::Move;

	/*if (curState == EnemyState::Attack)
	{
		std::cout << "now Attack" << std::endl;
	}
	if (curState == EnemyState::Move)
	{
		std::cout << "now Move" << std::endl;
	}*/
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
		attacktarget = m_CurTarget;
		break;
	}
	case EnemyState::Dead:
	{
		DirectX::SimpleMath::Vector3 deadpos = { 200.f,200.f,200.f };
		m_Move->SetEnemySpeed(0.f);
		m_Move->ResetPos(deadpos);
		m_Move->MoveTriggerSet(false);
		GetTransform()->SetWorldPosition(deadpos);
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
	{
		attackTimer = 0.0f;
		return;
	}
	attackTimer += Time::GetDeltaTime();
	if (attackTimer >= E_state.AS)
	{
		if (attacktarget == m_CurTarget)
		{
			if (attacktarget->GetName() == "Snow")
			{
				if (auto snowCollider = attacktarget->GetComponent<SnowCollider>(); snowCollider.IsValid())
				{
					if (!snowCollider->CheckOnPlayer())
					{
						snowCollider->lifeCount--;
						if (snowCollider->lifeCount <= 0)
						{
							snowCollider->DestroyByEnemy();
							m_CurTarget = m_MainTarget;
							OnStateEnter(EnemyState::Move);
						}
					}
				}
				else
				{
					m_CurTarget = m_MainTarget;
					OnStateEnter(EnemyState::Move);
				}
				attackTimer = 0.0f;
				return;
			}
			if (auto arrowenemy = GetComponent<ArrowEnemy>(); arrowenemy.IsValid())
				arrowenemy->ArrowAttack(attacktarget);
			else
				BattleManager::instance->Attack(GetGameObject(), attacktarget, E_state.AD);
		}
		if (auto targetstats = m_CurTarget->GetComponent<Battlestats>(); targetstats.IsValid())
		{
			if (targetstats->HP <= 0)
			{
				m_CurTarget = m_MainTarget;
				OnStateEnter(EnemyState::Move);
			}
		}
		else
		{
			m_CurTarget = m_MainTarget;
			OnStateEnter(EnemyState::Move);
		}
		attackTimer = 0.0f;
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