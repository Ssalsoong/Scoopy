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
#include "TargetSlotProvider.h"

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
	if (!m_hasSlot && m_CurTarget.IsValid())
	{
		if (--m_slotRetryFrames <= 0)
		{
			TryAcquireSlot();
			m_slotRetryFrames = m_slotRetryInterval;
		}
	}
	UpdateDistance();
	ChangeState();
	CheckState();

	AttackTarget();
	CalSnowDamageDelay();
	HurtCal();
}


//Enemy 초기값 설정용
void MMMEngine::EnemyController::InitEnemy(EnemyType type, DirectX::SimpleMath::Vector3 pos, int hp)
{
	GetTransform()->SetWorldPosition(pos);
	m_Sensor = m_SensorObj->GetComponent<EnemySensor>();
	if (!m_Sensor.IsValid())
	{
		std::cout << "Sensor not found" << std::endl;
	}
	m_Move = GetComponent<EnemyMove>();
	m_Move->ResetPos(pos);
	m_Move->ChangeTarget(m_MainTarget);

	curState = EnemyState::Move;
	prevState = EnemyState::Dead;

	m_EnemyType = type;


	m_CurTarget = m_MainTarget;
	TryAcquireSlot();
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
		E_state.AS = 6.5f;
		E_state.Range = 0.3f;
		break;
	}
	}
}


void MMMEngine::EnemyController::UpdateDistance()
{
	if (m_CurTarget == nullptr) m_CurTarget = m_MainTarget;

	if (!m_CurTarget.IsValid())
	{
		ReleaseSlot();
		distance = 1e9f;
		if (m_Move.IsValid())
			m_Move->ClearTargetOverride();
		return;
	}

	auto EnemyPos = GetTransform()->GetWorldPosition();
	DirectX::SimpleMath::Vector3 TargetPos{};
	bool useSlot = false;

	if (m_hasSlot && m_SlotProvider.IsValid() && m_SlotTarget == m_CurTarget)
	{
		if (m_SlotProvider->GetSlotWorldPos(m_slotRing, m_slotIndex, TargetPos))
		{
			useSlot = true;
		}
		else
		{
			ReleaseSlot();
		}
	}

	if (!useSlot)
		TargetPos = m_CurTarget->GetTransform()->GetWorldPosition();

	m_effectiveTargetPos = TargetPos;
	toTarget = TargetPos - EnemyPos;
	distance = toTarget.Length();

	if (m_Move.IsValid())
	{
		if (useSlot)
			m_Move->SetTargetOverride(m_effectiveTargetPos);
		else
			m_Move->ClearTargetOverride();
	}

	m_usingSlotTarget = useSlot;
}



void MMMEngine::EnemyController::ChangeState()
{
	/*if (curState == EnemyState::Attack && attackTimer > 0.0f)
		return;*/
    prevState = curState;

    float extra = 0.0f;
    const auto& tag = m_CurTarget->GetTag();

    if (tag == "Castle"){ extra = 0.6f;}
    else if (tag == "Building") extra = 0.3f;
    else if (tag == "Player") extra = 0.2f;
    else if (tag == "Snow") extra = 0.4f;

	if (m_usingSlotTarget)
		curState = (distance <= slotArriveRadius) ? EnemyState::Attack : EnemyState::Move;
	else
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
		attackTimer = 0.0f;
		RecoverTimer = 0.0f;
		break;
	}
	case EnemyState::Attack:
	{
		m_Move->MoveTriggerSet(false);
		battletarget = m_CurTarget;
		m_attackPhase = AttackPhase::Motion;
		attackTimer = 0.0f;
		RecoverTimer = 0.0f;
		MotionEnter();
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
		RecoverTimer = 0.0f;
		EnemySpawner::instance->EnemyDeath(GetGameObject());
		ReleaseSlot();
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
			ReleaseSlot();
			m_CurTarget = target;
			m_Move->ChangeTarget(m_CurTarget); // 여기서 바로 갱신
			TryAcquireSlot();
		}
		return true;
	}

	if (!target.IsValid())
	{
		if (m_CurTarget != m_MainTarget)
		{
			ReleaseSlot();
			m_CurTarget = m_MainTarget;
			m_Move->ChangeTarget(m_CurTarget);
			TryAcquireSlot();
		}
		return false;
	}

	return false;
}

void MMMEngine::EnemyController::TryAcquireSlot()
{
	m_hasSlot = false;
	m_SlotProvider = nullptr;
	m_SlotTarget = nullptr;
	m_slotRing = -1;
	m_slotIndex = -1;

	if (!m_CurTarget.IsValid()) return;

	auto provider = m_CurTarget->GetComponent<TargetSlotProvider>();
	if (!provider.IsValid()) return;

	int ring = -1;
	int index = -1;
	if (provider->RequestSlot(GetGameObject(), ring, index))
	{
		m_hasSlot = true;
		m_SlotProvider = provider;
		m_SlotTarget = m_CurTarget;
		m_slotRing = ring;
		m_slotIndex = index;
	}

	std::cout << "m_hasSlot : " << m_hasSlot << "m_slotRing : " << m_slotRing << "m_slotIndex : " << m_slotIndex << std::endl;
}

void MMMEngine::EnemyController::ReleaseSlot()
{
	if (m_hasSlot && m_SlotProvider.IsValid())
	{
		m_SlotProvider->ReleaseSlot(m_slotRing, m_slotIndex, GetGameObject());
	}

	m_hasSlot = false;
	m_SlotProvider = nullptr;
	m_SlotTarget = nullptr;
	m_slotRing = -1;
	m_slotIndex = -1;

	if (m_Move.IsValid())
		m_Move->ClearTargetOverride();
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
		m_attackPhase = AttackPhase::Motion;
		attackTimer = 0.0f;
		RecoverTimer = 0.0f;
		return;
	}

	if (!battletarget.IsValid() || !battletarget->IsActiveInHierarchy())
	{
		m_attackPhase = AttackPhase::Motion;
		attackTimer = 0.0f;
		RecoverTimer = 0.0f;

		m_CurTarget = m_MainTarget;
		OnStateEnter(EnemyState::Move);
		return;
	}

	const float dt = Time::GetDeltaTime();

	if (m_attackPhase == AttackPhase::Motion)
	{
		attackTimer += dt;

		if (attackTimer >= E_state.AS * debuff)
		{
			DoHit();

			m_attackPhase = AttackPhase::Pause;
			RecoverTimer = 0.0f;
			attackTimer = 0.0f;

			PauseEnter();
		}
	}
	else // Pause
	{
		RecoverTimer += dt;

		if (RecoverTimer >= RecoverDelay)
		{
			m_attackPhase = AttackPhase::Motion;
			attackTimer = 0.0f;
			RecoverTimer = 0.0f;

			MotionEnter();
		}
	}
}

void MMMEngine::EnemyController::DoHit()
{
	if (!battletarget.IsValid() || !battletarget->IsActiveInHierarchy())
		return;

	// 타겟이 바뀌었으면 공격 취소
	if (battletarget != m_CurTarget)
	{
		m_CurTarget = m_MainTarget;
		OnStateEnter(EnemyState::Move);
		return;
	}

	// Snow 처리
	if (battletarget->GetName() == "Snow")
	{
		if (auto snowCollider = battletarget->GetComponent<SnowCollider>(); snowCollider.IsValid())
		{
			if (!snowCollider->CheckOnPlayer())
				snowCollider->LifeDown();
		}
		else
		{
			m_CurTarget = m_MainTarget;
			OnStateEnter(EnemyState::Move);
		}
		return;
	}

	// 원거리/근거리
	if (auto arrowenemy = GetComponent<ArrowEnemy>(); arrowenemy.IsValid())
		arrowenemy->ArrowAttack(battletarget, E_state.AD);
	else
		BattleManager::instance->Attack(GetGameObject(), battletarget, E_state.AD);

	// HP 체크
	if (auto targetstats = battletarget->GetComponent<Battlestats>(); targetstats.IsValid())
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

void MMMEngine::EnemyController::HurtCal()
{
	if (HurtTimer > 0.0f)
	{
		HurtTimer -= Time::GetDeltaTime();
		if (HurtTimer <= 0.0f)
		{
			OnHurtFlag(false);
			HurtTimer = 0.0f;
		}
	}
}

void MMMEngine::EnemyController::MotionEnter()
{
	std::cout << "motionon" << std::endl;
}

void MMMEngine::EnemyController::PauseEnter()
{
	std::cout << "pauseon" << std::endl;
}