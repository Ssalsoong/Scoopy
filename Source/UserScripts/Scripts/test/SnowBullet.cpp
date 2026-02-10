#include "Export.h"
#include "ScriptBehaviour.h"
#include "SnowBullet.h"
#include "Transform.h"
#include "EnemyMove.h"
#include "SphereColliderComponent.h"

void MMMEngine::SnowBullet::Start()
{
	startPos = GetTransform()->GetWorldPosition();
	m_t = 0.f;
}

DirectX::SimpleMath::Vector3 MMMEngine::SnowBullet::Bezier2(const DirectX::SimpleMath::Vector3& p0, const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& p2, float t)
{
	float u = 1.f - t;
	return p0 * (u * u) + p1 * (2.f * u * t) + p2 * (t * t);
}

DirectX::SimpleMath::Vector3 MMMEngine::SnowBullet::Bezier2Deriv(const DirectX::SimpleMath::Vector3& p0, const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& p2, float t)
{
	return (p1 - p0) * (2.f * (1.f - t)) + (p2 - p1) * (2.f * t);
}

void MMMEngine::SnowBullet::FixedUpdate()
{
	if (RunFixedUpdate)
	{
		float dt = TimeManager::Get().GetFixedDeltaTime();
		if (dt <= 0.f) return;

		DirectX::SimpleMath::Vector3 p0 = startPos;
		if (target.IsValid()) p2 = target->GetTransform()->GetWorldPosition();
		DirectX::SimpleMath::Vector3 p1 = (p0 + p2) * 0.5f + DirectX::SimpleMath::Vector3(0, peakHeight, 0);

		DirectX::SimpleMath::Vector3 deriv = Bezier2Deriv(p0, p1, p2, m_t);
		float derivLen = deriv.Length();
		if (derivLen > 1e-5f)
		{
			float dtParam = (m_speed * dt) / derivLen;
			m_t = std::min(1.f, m_t + dtParam);
		}


		//m_spinAngle += m_spinSpeed * dt;

		//auto spin = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(
		//	DirectX::SimpleMath::Vector3::UnitX,  // x축 회전
		//	m_spinAngle
		//);


		float dAngle = m_spinSpeed * dt;

		auto delta = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(
			DirectX::SimpleMath::Vector3::UnitX, dAngle
		);


		DirectX::SimpleMath::Vector3 desiredPos = Bezier2(p0, p1, p2, m_t);
		DirectX::SimpleMath::Quaternion curRot = GetTransform()->GetWorldRotation();

		//auto desiredRot = spin * curRot;
		auto desiredRot = delta * curRot;
		desiredRot.Normalize();

		GetComponent<RigidBodyComponent>()->SetKinematicTarget(desiredPos, desiredRot);
	}

	//타겟이 도중에 파괴되었을때 용도
	if (!target.IsValid())
	{
		auto trans = GetTransform()->GetWorldPosition();
		if (trans == p2)
		{
			ReSet();
		}
	}
}

void MMMEngine::SnowBullet::OnTriggerEnter(MMMEngine::CollisionInfo info)
{
	if (info.other == target) ReSet();
}

void MMMEngine::SnowBullet::ReSet()
{
	RunFixedUpdate = false;
	GetTransform()->SetWorldPosition(startPos);
	GetGameObject()->SetActive(false);

	m_spinAngle = 0.f;
}

void MMMEngine::SnowBullet::SetSnowTarget(ObjPtr<GameObject> Enemy)
{
	target = Enemy;
}

void MMMEngine::SnowBullet::StartBullet(DirectX::SimpleMath::Vector3 Start_pos, float size, float speed, ObjPtr<GameObject> enemy)
{
	auto m_Trans = GetTransform();
	auto m_Rigid = GetComponent<RigidBodyComponent>();

	m_t = 0.f;
	auto sizevec = DirectX::SimpleMath::Vector3{ size };
	m_Trans->SetWorldScale(sizevec);
	m_Trans->SetWorldPosition(Start_pos);
	startPos = Start_pos;
	m_speed = speed;
	RunFixedUpdate = true;
	target = enemy;
	p2 = target->GetTransform()->GetWorldPosition();

	if (target.IsValid())
		p2 = target->GetTransform()->GetWorldPosition();

	if (m_Rigid.IsValid())
	{
		auto rot = m_Trans->GetWorldRotation();
		m_Rigid->SetKinematicTarget(Start_pos, rot);
		m_Rigid->WakeUp();
	}
}