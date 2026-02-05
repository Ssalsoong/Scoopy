#include "Export.h"
#include "ScriptBehaviour.h"
#include "SnowBullet.h"
#include "Transform.h"
#include "EnemyMove.h"

void MMMEngine::SnowBullet::Start()
{
	startPos = GetTransform()->GetWorldPosition();
    F_Pos = GetTransform()->GetWorldPosition();
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
    float dt = TimeManager::Get().GetFixedDeltaTime();
    if (dt <= 0.f) return;

    if (!target) return;

    DirectX::SimpleMath::Vector3 p0 = startPos;
    DirectX::SimpleMath::Vector3 p2 = target->GetTransform()->GetWorldPosition();
    DirectX::SimpleMath::Vector3 p1 = (p0 + p2) * 0.5f + DirectX::SimpleMath::Vector3(0, peakHeight, 0);

    DirectX::SimpleMath::Vector3 deriv = Bezier2Deriv(p0, p1, p2, m_t);
    float derivLen = deriv.Length();
    if (derivLen > 1e-5f)
    {
        float dtParam = (speed * dt) / derivLen;
        m_t = std::min(1.f, m_t + dtParam);
    }

    DirectX::SimpleMath::Vector3 desiredPos = Bezier2(p0, p1, p2, m_t);
    DirectX::SimpleMath::Quaternion curRot = GetTransform()->GetWorldRotation();
    GetComponent<RigidBodyComponent>()->SetKinematicTarget(desiredPos, curRot);
}

void MMMEngine::SnowBullet::OnTriggerEnter(MMMEngine::CollisionInfo info)
{
    ReSet();
}

void MMMEngine::SnowBullet::ReSet()
{
    GetGameObject()->SetActive(false);
}

void MMMEngine::SnowBullet::SetSnowTarget(ObjPtr<GameObject> Enemy)
{
    target = Enemy;
}
