#include "Export.h"
#include "ScriptBehaviour.h"
#include "PlayerMove.h"
#include "MMMInput.h"
#include "TileMap.h"
#include "RigidBodyComponent.h"
#include "PlayerController.h"
#include "../Sunken/PlayerAnimController.h"

void MMMEngine::PlayerMove::Start()
{
	//T = GameObject::Find("TileMap")->GetComponent<TileMap>();
	if (auto go = GameObject::Find("TileMap"); go.IsValid())
	{
		T = go->GetComponent<TileMap>();
	}

	m_rigid = GetComponent<RigidBodyComponent>();
	// 애니메이션 컨트롤러 저장
	mPAController = GetComponent<PlayerAnimController>();
	if (!mPAController) {
		std::cout << "PlayerMove::PlayerAnimController Not Found !!!" << std::endl;
	}
	else {
		mPAController->SetDefaultSpeed(DefaultSpeed);
	}
}


void MMMEngine::PlayerMove::FixedUpdate()
{
	auto rb = GetComponent<RigidBodyComponent>();
	Vector3 desiredVel = ComputeDesiredVelocity();
	rb->SetLinearVelocity(desiredVel);

	if (m_LookTarget)
	{
		m_LookTarget = false;

		Vector3 fwd = rb->Px_GetForward(); // 또는 Transform forward
		fwd.y = 0.f;

		if (fwd.LengthSquared() > 1e-6f)
		{
			fwd.Normalize();
			float targetYaw = std::atan2(fwd.x, fwd.z);
			rb->SnapRotation(Quaternion::CreateFromAxisAngle(Vector3::Up, targetYaw));
		}
		return;
	}

	ApplyYawFromVelocity(desiredVel);


	// 애니메이션 설정
	if (mPAController) {
		mPAController->SetMoveSpeed(desiredVel.Length());
	}
}



Vector3 MMMEngine::PlayerMove::ComputeDesiredVelocity()
{
	Vector3 input = m_InputDir;
	if (input.LengthSquared() <= 0.f) return Vector3(0, 0, 0);
	input.Normalize();

	Vector3 dir(input.x, 0.f, input.z);

	if (dir.LengthSquared() > 1e-6f) dir.Normalize();
	float speed = ComputeSpeed();
	return dir * speed * Time::GetFixedDeltaTime();
}


float MMMEngine::PlayerMove::ComputeSpeed()
{
	int scoop = 0;
	if (auto pc = GetComponent<PlayerController>(); pc.IsValid())
		scoop = pc->GetScoopCount();

	if (is_Scoop)
	{
		float T_OnSnowSpeed = OnSnowSpeed;

		float MinusSpeed = (25.f / float(Snow_MaxScoop));

		float slowed = T_OnSnowSpeed - (float(scoop) * MinusSpeed);
		if (slowed < MinSpeed) slowed = MinSpeed;
		return (slowed + m_FinalBuff); // 스쿱 상태면 타일 영향 무시
	}

	// 스쿱 아닐 때만 타일 속도 적용
	float speed = DefaultSpeed;
	if (T.IsValid())
	{
		auto pos = GetTransform()->GetWorldPosition();
		speed = T->IsTileClearedAt(pos.x, pos.z) ? DefaultSpeed : OnSnowSpeed;
	}

	CurSpeed = speed;

	return (speed + m_FinalBuff);
}

float MMMEngine::PlayerMove::GetCurSpeed()
{
	return CurSpeed;
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

void MMMEngine::PlayerMove::SetScoopMode(bool value, ObjPtr<GameObject> target)
{
	if (is_Scoop == value && Snow == target) return; // 중복 호출 방지


	Snow = target;
	is_Scoop = value;
	isSlow = value;
	m_LookTarget = value;
}

void MMMEngine::PlayerMove::SetInputDir(DirectX::SimpleMath::Vector3 vec)
{
	m_InputDir = vec;
}

void MMMEngine::PlayerMove::SetSnowMaxScoop(int maxValue)
{
	Snow_MaxScoop = maxValue;
}


void MMMEngine::PlayerMove::AddBuffSource(const void* src, float value)
{
	m_BuffSources[src] = value;
	RecalcBuff();
}

void MMMEngine::PlayerMove::RemoveBuffSource(const void* src)
{
	m_BuffSources.erase(src);
	RecalcBuff();
}

void MMMEngine::PlayerMove::UpdateBuffSource(const void* src, float value)
{
	auto it = m_BuffSources.find(src);
	if (it != m_BuffSources.end())
	{
		it->second = value;
		RecalcBuff();
	}
}

void MMMEngine::PlayerMove::RecalcBuff()
{
	float maxBuff = 0.f;

	for (auto& [k, v] : m_BuffSources)
	{
		if (v > maxBuff)
			maxBuff = v;
	}
	m_FinalBuff = maxBuff;
}

