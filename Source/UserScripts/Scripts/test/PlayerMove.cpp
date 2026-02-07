#include "Export.h"
#include "ScriptBehaviour.h"
#include "PlayerMove.h"
#include "MMMInput.h"
#include "TileMap.h"
#include "RigidBodyComponent.h"



void MMMEngine::PlayerMove::Start()
{
	//T = GameObject::Find("TileMap")->GetComponent<TileMap>();
	if (auto go = GameObject::Find("TileMap"); go.IsValid())
	{
		T = go->GetComponent<TileMap>();
	}

	m_rigid = GetComponent<RigidBodyComponent>();
}


void MMMEngine::PlayerMove::FixedUpdate()
{
	auto rb = GetComponent<RigidBodyComponent>();
	Vector3 desiredVel = ComputeDesiredVelocity();
	rb->SetLinearVelocity(desiredVel);

	if (m_LookTarget)
	{
		m_LookTarget = false;

		if (Snow.IsValid())
		{
			Vector3 to = Snow->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition();
			to.y = 0.f;
			if (to.LengthSquared() > 1e-6f)
			{
				to.Normalize();
				float targetYaw = std::atan2(to.x, to.z);
				rb->SnapRotation(Quaternion::CreateFromAxisAngle(Vector3::Up, targetYaw));
			}
		}
		return; // 이 프레임은 회전만 처리
	}

	ApplyYawFromVelocity(desiredVel);

}



Vector3 MMMEngine::PlayerMove::ComputeDesiredVelocity()
{
	Vector3 input = m_InputDir;
	if (input.LengthSquared() <= 0.f) return Vector3(0, 0, 0);
	input.Normalize();

	Vector3 dir(input.x, 0.f, input.z);

	if (dir.LengthSquared() > 1e-6f) dir.Normalize();
	return dir * basespeed * Time::GetDeltaTime();
}


void MMMEngine::PlayerMove::ApplyYawFromVelocity(const Vector3& v)
{
	Vector3 f = v;
	f.y = 0.f;

	if (f.LengthSquared() < 1e-6f) return;
	f.Normalize();

	auto rb = GetComponent<RigidBodyComponent>();

	float targetYaw = std::atan2(f.x, f.z);
	// 좌표계에 맞춰 atan2 부호/축만 조정
	if (!is_Scoop)
	{

		Quaternion q = Quaternion::CreateFromAxisAngle(Vector3::Up, targetYaw);


		rb->SnapRotation(q); // 혹은 부드럽게 보간해서 SnapRotation
	}
	else
	{
		//dt = TimeManager::Get().GetDeltaTime();
		dt = TimeManager::Get().GetFixedDeltaTime();
		curYaw = rb->Px_GetYaw();
		//forward = rb->Px_GetForward();

		float delta = WrapPi(targetYaw - curYaw);
		float maxStep = turnSpeed * dt;
		delta = std::clamp(delta, -maxStep, maxStep);

		rb->SnapRotation(
			Quaternion::CreateFromAxisAngle(Vector3::Up, curYaw + delta)
		);
	}
}

float MMMEngine::PlayerMove::WrapPi(float a)
{
	const float pi = 3.1415926535f;
	while (a > pi) a -= 2.f * pi;
	while (a < -pi) a += 2.f * pi;
	return a;
}

void MMMEngine::PlayerMove::SetScoopMode(bool value , ObjPtr<GameObject> target)
{
	Snow = target;
	is_Scoop = value;
	isSlow = value;
	m_LookTarget = value;
	
	if(value) m_rigid->SetInterpolationMode(RigidBodyComponent::InterpolationMode::None);
	else m_rigid->SetInterpolationMode(RigidBodyComponent::InterpolationMode::Interpolate);
}

void MMMEngine::PlayerMove::SetInputDir(DirectX::SimpleMath::Vector3 vec)
{
	m_InputDir = vec;
}
