#include "Export.h"
#include "ScriptBehaviour.h"
#include "SnowCollider.h"
#include "Transform.h"
#include "MMMTime.h"
#include "SnowTrigger.h"
#include "SnowballManager2.h"
#include "SphereColliderComponent.h"
#include "../Dongho/Snow/Snowball.h"

const float EPS2 = 1e-6f;

using namespace DirectX::SimpleMath;

DirectX::SimpleMath::Quaternion MMMEngine::SnowCollider::ComputeRollingRotation(DirectX::SimpleMath::Vector3& delta, float dt) const
{
	float dist = delta.Length();
	if (dist < 1e-6f) return Quaternion::Identity;

	DirectX::SimpleMath::Vector3 dir = delta / dist;
	DirectX::SimpleMath::Vector3 axis = Vector3::Up.Cross(dir);
	if (axis.LengthSquared() < 1e-6f) return Quaternion::Identity;
	axis.Normalize();

	float safeSize = (m_Size > 1e-4f) ? m_Size : 1e-4f;
	float angle = dist / (safeSize * 2.0f);

	DirectX::SimpleMath::Quaternion dq = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, angle);

	float t = 1.0f - expf(-m_Rolesmooth * dt);
	return DirectX::SimpleMath::Quaternion::Slerp(DirectX::SimpleMath::Quaternion::Identity, dq, t);

}


//눈이 합쳐지거나 성에 넣어서 사라질때 호출할 함수
void MMMEngine::SnowCollider::SnowDestory()
{
	auto my_ptr = GetGameObject();
	SnowManager->GetComponent<SnowballManager2>()->RemoveFromList(my_ptr);
	TriggerCollider->GetComponent<SnowTrigger>()->DestoryTrigger();
}

void MMMEngine::SnowCollider::SetScoopCount(int count)
{
	//if(scoopCount < MaxSnowCount)
	//{
	//	//Todo :: 사이즈 최대수정 및 최대치에 도달했을때 더 캘수있을지 정하기
	//	scoopCount = std::max(0, count);
	//	//스쿱 1당 사이즈 셋팅
	//	SetSize(0.1f + 0.05f * scoopCount);
	//}
	scoopCount = std::clamp(count, 0, MaxSnowCount);
	SetSize(0.1f + 0.05f * scoopCount);
	GetComponent<Snowball>()->SetPoint(scoopCount+1);
}

int MMMEngine::SnowCollider::GetScoopCount()
{
	return scoopCount;
}

void MMMEngine::SnowCollider::AddScoop(int delta)
{
	SetScoopCount(scoopCount + delta);
}

void MMMEngine::SnowCollider::MaxSnow(int count)
{
	MaxSnowCount = count;
}



void MMMEngine::SnowCollider::Start()
{	
	auto selfPtr = GetGameObject();
	auto childTrans = GetTransform()->Find("TriggerCollider");
	if (childTrans.IsValid()) { 
		TriggerCollider = childTrans->GetGameObject();
		TriggerCollider->GetComponent<SnowTrigger>()->SetParentPtr(selfPtr);
	}

	//SetSize(1.0f);
	m_currPos = GetTransform()->GetWorldPosition();
	m_lastPos = GetTransform()->GetWorldPosition();
	m_lastRot = GetTransform()->GetWorldRotation();
	m_currRot = GetTransform()->GetWorldRotation();
}

void MMMEngine::SnowCollider::Update()
{
	GetTransform()->SetWorldPosition(Vector3::Lerp(m_lastPos, m_currPos, TimeManager::Get().GetInterpolationAlpha()));
	GetTransform()->SetWorldRotation(Quaternion::Slerp(m_lastRot, m_currRot, TimeManager::Get().GetInterpolationAlpha()));
}

void MMMEngine::SnowCollider::FixedUpdate()
{
	m_lastPos = m_currPos;
	m_lastRot = m_currRot;
	if (!On_Player || !m_player) return;
	
	

    float dt = TimeManager::Get().GetFixedDeltaTime();
    if (dt <= 0.f) return;

    auto pTr = m_player->GetTransform();
	DirectX::SimpleMath::Vector3 playerPos = pTr->GetWorldPosition();
	DirectX::SimpleMath::Quaternion playerRot = pTr->GetWorldRotation();

    // 플레이어 전방 벡터
	DirectX::SimpleMath::Vector3 forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, playerRot);
	//DirectX::SimpleMath::Vector3 forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, playerRot);
	forward.y = 0.f;
	if (forward.LengthSquared() < 1e-6f) return;
	forward.Normalize();

	//m_holdDistance = (m_Size / 2) + 0.2f;
	//m_holdHeight = m_Size / 2;

	float snowRadius = m_Size * 0.5f;
	float playerRadius = 0.4;
	float minXZ = playerRadius + snowRadius + 0.02f;

	m_holdDistance = std::max(minXZ, snowRadius + 0.2f); // 수평만
	m_holdHeight = snowRadius;

	// 홀드 포지션
	float yaw = std::atan2(forward.x, forward.z);
	DirectX::SimpleMath::Quaternion yawRot = Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, yaw);

	// holdPos 계산
	DirectX::SimpleMath::Vector3 holdPos = playerPos + forward * m_holdDistance + DirectX::SimpleMath::Vector3(0, m_holdHeight, 0);

	if (!m_hasPrevHoldPos)
	{
		m_prevHoldPos = holdPos;
		m_hasPrevHoldPos = true;
	}

	DirectX::SimpleMath::Vector3 move = playerPos - m_prevPlayerPos;
	move.y = 0.f;

	// forward 방향 이동량만 굴림에 사용 (대각선이면 자연스레 줄어듦)
	float dist = move.Dot(forward);

	float safeSize = (m_Size > 1e-4f) ? m_Size : 1e-4f;
	if (fabsf(dist) > 1e-6f)
	{
		m_rollAngle += (dist / (safeSize * 2.0f)) * m_rollSpeed;
	}

	m_prevPlayerPos = playerPos;
	m_prevHoldPos = holdPos;

	// --- 롤 축을 forward 기반으로 계산 ---
	DirectX::SimpleMath::Vector3 axis = DirectX::SimpleMath::Vector3::Up.Cross(forward);
	if (axis.LengthSquared() < 1e-6f)
		axis = m_rollAxis; // 이전 축 유지
	else
	{
		axis.Normalize();
		// 축이 갑자기 반전되면(180도 점프) 부호 고정
		if (m_hasRollAxis && axis.Dot(m_rollAxis) < 0.f)
			axis = -axis;
	}

	m_rollAxis = axis;
	m_hasRollAxis = true;

	DirectX::SimpleMath::Quaternion rollRot = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, m_rollAngle);

	// yawRot * rollRot 같은 합성 제거
	auto rb = GetComponent<RigidBodyComponent>();
	rb->SetKinematicTarget(holdPos, rollRot);

	m_currPos = holdPos;
	m_currRot = rollRot;
}

bool MMMEngine::SnowCollider::CheckOnPlayer()
{
	return On_Player;
}


void MMMEngine::SnowCollider::SetSize(float size)
{
	m_Size = size;
	
	auto curPosition = GetTransform()->GetWorldPosition();
	curPosition.y = size/2;
	GetTransform()->SetWorldPosition(curPosition);

	auto m_col = GetComponent<SphereColliderComponent>();
	m_col->SetRadius(size / 2);

	DirectX::SimpleMath::Vector3 scale_Size = DirectX::SimpleMath::Vector3(size);
	GetTransform()->SetWorldScale(scale_Size);

	auto Trigger = TriggerCollider->GetComponent<SnowTrigger>();
	Trigger->SetTriggerSize(size);
}

void MMMEngine::SnowCollider::SetOnPlayer(bool value, ObjPtr<GameObject> player)
{
	On_Player = value;
	m_player = player;

	if (On_Player && m_player)
	{
		//플레이어 Trans
		auto pTr = m_player->GetTransform();

		//눈덩이의 Trans
		auto sTr = GetTransform();

		//설정했을때 플레이어 위치, 회전 눈덩이 위치 회전
		DirectX::SimpleMath::Vector3 playerPos = pTr->GetWorldPosition();
		DirectX::SimpleMath::Quaternion playerRot = pTr->GetWorldRotation();
		DirectX::SimpleMath::Vector3 snowPos = sTr->GetWorldPosition();
		DirectX::SimpleMath::Quaternion snowRot = sTr->GetWorldRotation();

		DirectX::SimpleMath::Quaternion inv = playerRot;
		inv.Inverse(inv);          // playerRot^-1

		m_prevPlayerPos = playerPos;
		m_prevPlayerRot = playerRot;

		m_localOffset = DirectX::SimpleMath::Vector3::Transform(snowPos - playerPos, inv);
		m_localRotOffset = inv * snowRot;

		m_hasPrevPlayerPos = true;

		m_baseRot = GetTransform()->GetWorldRotation();
		m_rollRot = DirectX::SimpleMath::Quaternion::Identity;
		m_hasPrevHoldPos = false;
	}
	else
	{
		m_hasPrevPlayerPos = false;
		m_player = nullptr;
		m_hasPrevHoldPos = false;
		m_rollAngle = 0.f;

		m_hasRollAxis = false;
		m_rollAxis = DirectX::SimpleMath::Vector3::Right;

	}
}
