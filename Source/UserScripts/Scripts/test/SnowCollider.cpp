#include "Export.h"
#include "ScriptBehaviour.h"
#include "SnowCollider.h"
#include "Transform.h"
#include "MMMTime.h"
#include "SnowTrigger.h"
#include "SnowballManager2.h"

const float EPS2 = 1e-6f;

DirectX::SimpleMath::Quaternion MMMEngine::SnowCollider::ComputeRollingRotation(DirectX::SimpleMath::Vector3& delta, float dt) const
{
	float dist = delta.Length();
	if (dist < 1e-6f) return Quaternion::Identity;

	Vector3 dir = delta / dist;
	Vector3 axis = Vector3::Up.Cross(dir);
	if (axis.LengthSquared() < 1e-6f) return Quaternion::Identity;
	axis.Normalize();

	float safeSize = (m_Size > 1e-4f) ? m_Size : 1e-4f;
	float angle = dist / (safeSize * 2.0f);

	Quaternion dq = Quaternion::CreateFromAxisAngle(axis, angle);

	float t = 1.0f - expf(-m_Rolesmooth * dt);
	return Quaternion::Slerp(Quaternion::Identity, dq, t);

}


//눈이 합쳐지거나 성에 넣어서 사라질때 호출할 함수
void MMMEngine::SnowCollider::SnowDestory()
{
	auto my_ptr = GetGameObject();
	SnowManager->GetComponent<SnowballManager2>()->RemoveFromList(my_ptr);
	TriggerCollider->GetComponent<SnowTrigger>()->DestoryTrigger();
}



void MMMEngine::SnowCollider::Start()
{	
	auto selfPtr = GetGameObject();
	auto childTrans = GetTransform()->Find("TriggerCollider");
	if (childTrans.IsValid()) { 
		TriggerCollider = childTrans->GetGameObject();
		TriggerCollider->GetComponent<SnowTrigger>()->SetParentPtr(selfPtr);
	}

	SetSize(1.0f);
}

void MMMEngine::SnowCollider::FixedUpdate()
{
	if (!On_Player || !m_player) return;

    float dt = TimeManager::Get().GetFixedDeltaTime();
    if (dt <= 0.f) return;

    auto pTr = m_player->GetTransform();
    Vector3 playerPos = pTr->GetWorldPosition();
    Quaternion playerRot = pTr->GetWorldRotation();

    // 플레이어 전방 벡터
    Vector3 forward = Vector3::Transform(Vector3::Backward, playerRot);

	m_holdDistance = (m_Size / 2) + 0.2f;
	m_holdHeight = m_Size / 2;

	// 홀드 포지션
    Vector3 holdPos = playerPos + forward * m_holdDistance + Vector3(0, m_holdHeight, 0);

    if (auto rb = GetComponent<RigidBodyComponent>(); rb.IsValid())
    {
        // 회전은 플레이어 정면으로 맞춤 (필요 없으면 curRot 유지 가능)
        rb->SetKinematicTarget(holdPos, playerRot);
    }
}


void MMMEngine::SnowCollider::SetSize(float size)
{
	m_Size = size;
	
	auto curPosition = GetTransform()->GetWorldPosition();
	curPosition.y = size/2;
	GetTransform()->SetWorldPosition(curPosition);

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
		Vector3 playerPos = pTr->GetWorldPosition();
		Quaternion playerRot = pTr->GetWorldRotation();
		Vector3 snowPos = sTr->GetWorldPosition();
		Quaternion snowRot = sTr->GetWorldRotation();

		Quaternion inv = playerRot;
		inv.Inverse(inv);          // playerRot^-1

		m_prevPlayerPos = playerPos;
		m_prevPlayerRot = playerRot;

		m_localOffset = Vector3::Transform(snowPos - playerPos, inv);
		m_localRotOffset = inv * snowRot;

		m_hasPrevPlayerPos = true;
	}
	else
	{
		m_hasPrevPlayerPos = false;
		m_player = nullptr;
	}
}
