#include "Export.h"
#include "ScriptBehaviour.h"
#include "EnemyMove.h"
#include "Transform.h"
#include "MMMTime.h"
#include "TileMap.h"
#include "CapsuleColliderComponent.h"

using namespace DirectX::SimpleMath;

static float WrapPi(float a)
{
	while (a > DirectX::XM_PI)  a -= DirectX::XM_2PI;
	while (a < -DirectX::XM_PI)  a += DirectX::XM_2PI;
	return a;
}

static float LerpAngleRad(float cur, float target, float a)
{
	float d = WrapPi(target - cur);
	return cur + d * a;
}

static float YawFromQuat_YUp(const Quaternion& q)
{
	float siny_cosp = 2.f * (q.w * q.y + q.x * q.z);
	float cosy_cosp = 1.f - 2.f * (q.y * q.y + q.x * q.x);
	return atan2f(siny_cosp, cosy_cosp);
}


void MMMEngine::EnemyMove::Start()
{
	myPos = GetGameObject()->GetTransform()->GetWorldPosition();
	m_GO = GetGameObject();

	if (auto go = GameObject::Find("TileMap"); go.IsValid())
	{
		m_T = go->GetComponent<TileMap>();
	}
}

void MMMEngine::EnemyMove::FixedUpdate()
{

	Vector3 targetPos = m_hasTargetOverride
		? m_targetOverride
		: (Obj_target.IsValid() ? Obj_target->GetTransform()->GetWorldPosition()
			: m_GO->GetTransform()->GetWorldPosition());

	FaceTargetYaw(targetPos);


	auto rb = GetComponent<RigidBodyComponent>();
	if (!is_move)
	{
		rb->SetLinearVelocity(DirectX::SimpleMath::Vector3::Zero);
		return;
	}


	auto col = GetComponent<CapsuleColliderComponent>();
	float dt = TimeManager::Get().GetFixedDeltaTime();

	//목표 속도
	Vector3 desiredVel = ComputeChaseVelocity();
	Vector3 chaseDir = desiredVel;
	chaseDir.y = 0.f;

	if (chaseDir.LengthSquared() > 1e-6f)
		chaseDir.Normalize();
	else
		chaseDir = Vector3(0, 0, 1);

	//SweepSphere로 "앞에 벽" 미리 감지
	SweepHit sh{};
	Vector3 my = m_GO->GetTransform()->GetWorldPosition();
	my.y = 0.f;

	// layer는 자기자신의 layer를 넣음
	uint32_t wallLayer = 15;

	bool hit = PhysxManager::Get().SweepSphere(
		my, sweepRadius,
		chaseDir, sweepAhead,
		sh, wallLayer,
		col,
		rb,
		false
	);

	if (hit && sh.gameObject.IsValid())
	{
		//hit.normal로 슬라이드 방향 계산 (후진/왕복 방지)
		Vector3 n = sh.normal;
		n.y = 0.f;

		if (n.LengthSquared() > 1e-6f)
		{
			n.Normalize();

			// (a) chaseDir에서 벽 안으로 파고드는 성분 제거
			Vector3 slide = chaseDir;
			float dn = slide.Dot(n);
			if (dn < 0.f) slide -= n * dn;

			// (b) 정면 막힘이면 tangent 강제 + side 고정
			if (slide.LengthSquared() < 1e-4f)
			{
				Vector3 t = Vector3(0, 1, 0).Cross(n);
				t.y = 0.f;
				if (t.LengthSquared() > 1e-6f)
				{
					t.Normalize();

					// side 고정 로직(프레임마다 뒤집히면 지터)
					if (!hasAvoidSide)
					{
						// 목표 방향과 더 가까운 쪽 선택
						float s = t.Dot(chaseDir);
						avoidSide = (s >= 0.f) ? +1 : -1;
						hasAvoidSide = true;
					}
					slide = t * (float)avoidSide;
				}
			}
			else
			{
				// 슬라이드가 잘 나오면 side 고정 해제
				hasAvoidSide = false;
			}

			//최종 방향 섞기 (슬라이드만 써도 되고, 섞으면 더 부드러움)
			Vector3 dir = chaseDir + slide * avoidGain;
			if (dir.LengthSquared() > 1e-6f) dir.Normalize();

			desiredVel = dir * movespeed;
		}
	}
	else
	{
		hasAvoidSide = false;
	}

	float m_sharpness = 8.0f;

	//관성(가속 제한)
	curVel = DirectX::SimpleMath::Vector3::Lerp(curVel, desiredVel, m_sharpness * Time::GetFixedDeltaTime());
	curVel.y = 0.f;

	curVel *= Time::GetFixedDeltaTime();
	curVel *= debuffSpeed;

	rb->SetLinearVelocity(curVel);

	if (is_move)
	{
		rb->SetLockPosX(false);
		rb->SetLockPosZ(false);
		rb->SetLockRotY(false);
		FaceVelocityYaw(curVel);
	}
	else
	{
		rb->SetLockPosX(true);
		rb->SetLockPosZ(true);
		rb->SetLockRotY(true);
		FaceTargetYaw(targetPos);
	}
}


DirectX::SimpleMath::Vector3 MMMEngine::EnemyMove::ComputeChaseVelocity()
{
	myPos = m_GO->GetTransform()->GetWorldPosition();

	if (!Obj_target && !m_hasTargetOverride)
	{
		std::cout << "Target Nullptr" << std::endl;
		return Vector3{ 0,0,0 };
	}

	if (m_hasTargetOverride)
		Target = m_targetOverride;
	else
		Target = Obj_target->GetTransform()->GetWorldPosition();


	//Target = Obj_target->GetTransform()->GetWorldPosition();

	DirectX::SimpleMath::Vector3 dir = (Target - myPos);
	dir.y = 0.f;

	DirectX::SimpleMath::Vector3 EnemyVec = DirectX::SimpleMath::Vector3{ 0, 0, 0 };

	if (dir.LengthSquared() > 0.f)
	{
		dir.Normalize();
		EnemyVec = dir * movespeed;
	}

	return EnemyVec;
}


void MMMEngine::EnemyMove::FaceVelocityYaw(const DirectX::SimpleMath::Vector3& vel)
{
    Vector3 v = vel;
    v.y = 0.f;

    if (v.LengthSquared() < 0.01f)
        return;

    v.Normalize();

    float targetYaw = atan2f(v.x, v.z);

    auto tr = m_GO->GetTransform();

    Quaternion curQ = tr->GetWorldRotation();
    float curYaw = YawFromQuat_YUp(curQ);

    float newYaw = LerpAngleRad(curYaw, targetYaw, yawLerpA);

    Quaternion newQ =
        Quaternion::CreateFromYawPitchRoll(newYaw, 0.f, 0.f);

    tr->GetComponent<RigidBodyComponent>()->SnapRotation(newQ);
}


//trigger 함수들
void MMMEngine::EnemyMove::AddObjPtr(ObjPtr<GameObject> snow)
{
	if (!snow.IsValid()) return;
	for (auto& o : SnowObjPtr)
		if (o == snow) return;
	SnowObjPtr.push_back(snow);
}

void MMMEngine::EnemyMove::Remove(ObjPtr<GameObject> snow)
{
	SnowObjPtr.erase(
		std::remove_if(SnowObjPtr.begin(), SnowObjPtr.end(),
			[&](const ObjPtr<GameObject>& o) { return !o.IsValid() || o == snow; }),
		SnowObjPtr.end());
}

void MMMEngine::EnemyMove::ChangeTarget(ObjPtr<GameObject> target)
{
    Obj_target = target;
}

void MMMEngine::EnemyMove::SetTargetOverride(const DirectX::SimpleMath::Vector3& pos)
{
	m_targetOverride = pos;
	m_hasTargetOverride = true;
}

void MMMEngine::EnemyMove::ClearTargetOverride()
{
	m_hasTargetOverride = false;
}


void MMMEngine::EnemyMove::MoveTriggerSet(bool value)
{
    is_move = value;
}

void MMMEngine::EnemyMove::SetEnemySpeed(float speedvalue)
{
	movespeed = speedvalue;
}

void MMMEngine::EnemyMove::SetDebuffSpeed(float value)
{
	debuffSpeed = value;
}

void MMMEngine::EnemyMove::ResetPos(DirectX::SimpleMath::Vector3 pos)
{
	auto m_Trans = GetTransform();
	auto m_Rigid = GetComponent<RigidBodyComponent>();
	if (m_Rigid.IsValid())
	{
		m_Rigid->Teleport(pos, Quaternion::Identity);
		m_Rigid->SetLinearVelocity(Vector3::Zero);
		m_Rigid->SetAngularVelocity(Vector3::Zero);
		curVel = Vector3::Zero;
		is_move = false;
	}
}

void MMMEngine::EnemyMove::FaceTargetYaw(const DirectX::SimpleMath::Vector3& targetPos)
{
	Vector3 my = m_GO->GetTransform()->GetWorldPosition();
	Vector3 dir = targetPos - my;
	dir.y = 0.f;

	//변화가 없다면
	if (dir.LengthSquared() < 1e-6f)
		return;

	dir.Normalize();
	float targetYaw = atan2f(dir.x, dir.z);

	auto rb = m_GO->GetComponent<RigidBodyComponent>();

	// 회전은 SnapRotation으로 제어 ( x y z 다 잠궈둠 )
	Quaternion newQ = Quaternion::CreateFromYawPitchRoll(targetYaw, 0.f, 0.f);
	rb->SnapRotation(newQ);

	// 물리 회전 튐 방지
	rb->SetAngularVelocity(Vector3::Zero);
}


void MMMEngine::EnemyMove::AddSpeedDebuffSource(const void* src, float mult)
{
	m_SpeedDebuffSources[src] = mult;
	RecalcSpeedMult();
}

void MMMEngine::EnemyMove::RemoveSpeedDebuffSource(const void* src)
{
	m_SpeedDebuffSources.erase(src);
	RecalcSpeedMult();
}

void MMMEngine::EnemyMove::UpdateSpeedDebuffSource(const void* src, float mult)
{
	auto it = m_SpeedDebuffSources.find(src);
	if (it != m_SpeedDebuffSources.end())
	{
		it->second = mult;
		RecalcSpeedMult();
	}
}

void MMMEngine::EnemyMove::RecalcSpeedMult()
{
	float best = 1.0f; // min 룰 (가장 느리게)
	for (auto& [k, v] : m_SpeedDebuffSources)
		if (v < best) best = v;

	debuffSpeed = best;
}