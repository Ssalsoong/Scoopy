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
#include "../Sunken/EnemyAnimController.h"
#include "RigidBodyComponent.h"


void MMMEngine::EnemyController::Start()
{
	m_Sensor = m_SensorObj->GetComponent<EnemySensor>();
	m_Move = GetComponent<EnemyMove>();
	m_Rigid = GetComponent<RigidBodyComponent>();

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
	// 슬롯 타임아웃 재요청
	const float dt = Time::GetDeltaTime();
	m_slotReassignTimer = std::max(0.0f, m_slotReassignTimer - dt);

	if (m_hasSlot && m_usingSlotTarget)
	{
		m_slotElapsed += dt;

		if (distance <= slotArriveRadius)
			m_slotElapsed = 0.0f;

		if (m_slotElapsed >= m_slotTimeout && m_slotReassignTimer <= 0.0f)
		{
			ReleaseSlot();
			TryAcquireSlot();
			m_slotElapsed = 0.0f;
			m_slotReassignTimer = m_slotReassignCooldown;
		}
	}
	else
	{
		m_slotElapsed = 0.0f;
	}
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

	//랜덤변수
	static std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> dist(0.f, DirectX::XM_2PI);
	m_rangedAngle = dist(rng);

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
		E_state.AS = 0.65f;
		E_state.Range = 0.3f;
		break;
	}
	}
}


void MMMEngine::EnemyController::ChangeState()
{
	prevState = curState;

	float extra = 0.0f;
	const auto& tag = m_CurTarget->GetTag();

	if (tag == "Castle") extra = 0.8f;
	else if (tag == "Building") extra = 0.4f;
	else if (tag == "Player") extra = 0.25f;
	else if (tag == "Snow") extra = 0.3f;

	float rangeEnter = E_state.Range + extra;
	float rangeExit = rangeEnter + 0.2f;

	float slotEnter = slotArriveRadius;   // 근접 공격은 이 값만 사용
	float slotExit = slotEnter + 0.1f;

	// Attack 상태면 먼저 이탈 체크
	if (curState == EnemyState::Attack)
	{
		if (!m_canExitAttack)
			return;
		float exitDist = (IsBruiser() ? slotExit : rangeExit);
		if (!m_CurTarget.IsValid() || distance > exitDist)
		{
			curState = EnemyState::Move;
			m_canExitAttack = false;
			return;
		}

		m_canExitAttack = false;
		m_attackPhase = AttackPhase::Motion;
		attackTimer = 0.0f;
		RecoverTimer = 0.0f;
		MotionEnter();
		return;
	}

	// 일반 진입 조건
	if (IsBruiser())
	{
		if (m_usingSlotTarget)
			curState = (distance <= slotEnter) ? EnemyState::Attack : EnemyState::Move;
		else
			curState = (distance <= rangeEnter) ? EnemyState::Attack : EnemyState::Move;
	}
	else
	{
		curState = (distance <= rangeEnter) ? EnemyState::Attack : EnemyState::Move;
	}

}

void MMMEngine::EnemyController::CheckState()
{
	if (prevState != curState)
	{
		OnStateEnter(curState);
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
		if (auto Curtr = m_CurTarget->GetTransform(); Curtr.IsValid())
		{
			TargetPos = Curtr->GetWorldPosition();
		}

	m_effectiveTargetPos = TargetPos;
	toTarget = TargetPos - EnemyPos;
	distance = toTarget.Length();

	if (m_Move.IsValid())
	{
		if (useSlot)
		{
			Vector3 center = m_CurTarget->GetTransform()->GetWorldPosition();

			Vector3 cur = EnemyPos - center; cur.y = 0.f;
			Vector3 tar = TargetPos - center; tar.y = 0.f;

			float rCur = cur.Length();
			float rTar = tar.Length();

			if (rCur > 1e-3f && rTar > 1e-3f)
			{
				float aCur = atan2f(cur.z, cur.x);
				float aTar = atan2f(tar.z, tar.x);

				float d = aTar - aCur;
				// WrapPi
				while (d > DirectX::XM_PI) d -= DirectX::XM_2PI;
				while (d < -DirectX::XM_PI) d += DirectX::XM_2PI;

				if (!m_orbiting && fabs(d) > orbitEnterAngle)
				{
					m_orbiting = true;
					m_orbitDir = (d >= 0.f) ? +1 : -1;
				}

				float radialDiff = fabs(rCur - rTar);

				// orbit 진입 조건 (거리/반경 조건으로 조절)
				if (!m_orbiting)
				{
					if (radialDiff <= orbitRadialTolerance &&
						distance <= orbitStartDist &&
						fabs(d) > orbitEnterAngle)
					{
						m_orbiting = true;
						m_orbitDir = (d >= 0.f) ? +1 : -1;
					}
				}

				// orbit 유지/해제 조건
				if (m_orbiting)
				{
					if (radialDiff > orbitRadialTolerance ||
						distance < orbitExitDist ||
						fabs(d) < orbitExitAngle)
					{
						m_orbiting = false;
					}
					else
					{
						Vector3 tangent(-cur.z, 0.f, cur.x);
						if (tangent.LengthSquared() > 1e-6f)
						{
							tangent.Normalize();
							tangent *= (float)m_orbitDir;

							TargetPos = EnemyPos + tangent * orbitStepDist;

							float useR = std::max(rCur, rTar + orbitLaneOffset);
							Vector3 toNew = TargetPos - center; toNew.y = 0.f;
							if (toNew.LengthSquared() > 1e-6f)
							{
								toNew.Normalize();
								TargetPos = center + toNew * useR;
							}
						}
					}
				}
			}
			m_Move->SetTargetOverride(TargetPos);
		}
		else
		{
			if (IsBruiser())
				m_Move->ClearTargetOverride();
		}
	}
	m_usingSlotTarget = useSlot;
}




void MMMEngine::EnemyController::OnStateEnter(EnemyState state)
{
	switch (state)
	{
	case EnemyState::Move:
	{
		m_Rigid->SetKinematic(false);
		m_Move->ChangeTarget(m_CurTarget);
		m_Move->MoveTriggerSet(true);
		attackTimer = 0.0f;
		RecoverTimer = 0.0f;
		break;
	}
	case EnemyState::Attack:
	{
		auto t_Zero = DirectX::SimpleMath::Vector3::Zero;
		m_Rigid->SetKinematic(false);
		m_Rigid->SetLinearVelocity(t_Zero);
		m_Rigid->SetAngularVelocity(t_Zero);
		m_Move->MoveTriggerSet(false);
		battletarget = m_CurTarget;
		m_attackPhase = AttackPhase::Motion;
		attackTimer = 0.0f;
		RecoverTimer = 0.0f;
		m_canExitAttack = false;
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
	ObjPtr<GameObject> raw = m_Sensor->GetTarget();
	ObjPtr<GameObject> target = ResolveTarget(raw);

	if (!IsBruiser() && m_hasSlot)
		ReleaseSlot();

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

	if (!IsBruiser())
	{
		auto targetPos = m_CurTarget->GetTransform()->GetWorldPosition();

		// 고정 각도/반경 오프셋 (개체별 고정)
		float angle = m_rangedAngle;           // Start()에서 랜덤/해시로 초기화
		float radius = m_rangedHoldRadius;     // 2.0f~4.0f 정도
		Vector3 offset(std::cos(angle) * radius, 0.f, std::sin(angle) * radius);

		m_Move->SetTargetOverride(targetPos + offset);
	}
	else
	{
		m_Move->ClearTargetOverride();
	}

	if (m_CurTarget != target)
	{
		ReleaseSlot();
		m_orbiting = false; //타겟 바뀌면 orbit 리셋
		m_CurTarget = target;
		m_Move->ChangeTarget(m_CurTarget);
		TryAcquireSlot();
	}

	return true;
}

bool MMMEngine::EnemyController::IsBruiser()
{
	return (m_EnemyType == EnemyType::Warrior || m_EnemyType == EnemyType::Scout);
}

void MMMEngine::EnemyController::TryAcquireSlot()
{
	if (!IsBruiser()) return;
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

MMMEngine::ObjPtr<MMMEngine::GameObject> MMMEngine::EnemyController::ResolveTarget(ObjPtr<GameObject> raw)
{
	if (!raw.IsValid()) return nullptr;

	for (auto tr = raw->GetTransform(); tr != nullptr; tr = tr->GetParent())
	{
		auto go = tr->GetGameObject();
		if (auto p = go->GetComponent<TargetSlotProvider>(); p.IsValid())
			return go; // 슬롯 제공자(부모)를 타겟으로 승격
	}

	return raw;
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

	// Attack stays dynamic; do not force kinematic or zero velocity here.

	if (!battletarget.IsValid() || !battletarget->IsActiveInHierarchy())
	{
		m_attackPhase = AttackPhase::Motion;
		attackTimer = 0.0f;
		RecoverTimer = 0.0f;

		m_CurTarget = m_MainTarget;
		curState = EnemyState::Move;
		OnStateEnter(EnemyState::Move);
		return;
	}

	const float dt = Time::GetDeltaTime();

	if (m_attackPhase == AttackPhase::Motion)
	{
		attackTimer += dt;

		if (attackTimer >= E_state.AS)
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

		if (RecoverTimer >= RecoverDelay * m_FinalAttackMult)
		{
			/*m_attackPhase = AttackPhase::Motion;
			attackTimer = 0.0f;
			RecoverTimer = 0.0f;

			MotionEnter();*/
			m_canExitAttack = true;
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
		curState = EnemyState::Move;
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
			curState = EnemyState::Move;
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
			curState = EnemyState::Move;
			OnStateEnter(EnemyState::Move);
		}
	}
	else
	{
		m_CurTarget = m_MainTarget;
		curState = EnemyState::Move;
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
	if (auto EnemyAni = GetComponent<EnemyAnimController>(); EnemyAni.IsValid())
	{
		EnemyAni->PlayAttack();
	}
	std::cout << "motionon" << std::endl;
}

void MMMEngine::EnemyController::PauseEnter()
{
	std::cout << "pauseon" << std::endl;
}



void MMMEngine::EnemyController::AddAttackDebuffSource(const void* src, float mult)
{
	m_AttackDebuffSources[src] = mult;
	RecalcAttackMult();
}

void MMMEngine::EnemyController::RemoveAttackDebuffSource(const void* src)
{
	m_AttackDebuffSources.erase(src);
	RecalcAttackMult();
}

void MMMEngine::EnemyController::UpdateAttackDebuffSource(const void* src, float mult)
{
	auto it = m_AttackDebuffSources.find(src);
	if (it != m_AttackDebuffSources.end())
	{
		it->second = mult;
		RecalcAttackMult();
	}
}

void MMMEngine::EnemyController::RecalcAttackMult()
{
	float best = 1.0f; // max 룰 (가장 강하게)
	for (auto& [k, v] : m_AttackDebuffSources)
		if (v > best) best = v;

	m_FinalAttackMult = best;
}
