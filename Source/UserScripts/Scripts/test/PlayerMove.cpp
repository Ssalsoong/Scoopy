#include "Export.h"
#include "ScriptBehaviour.h"
#include "PlayerMove.h"
#include "MMMInput.h"
#include "TileMap.h"



void MMMEngine::PlayerMove::Start()
{
	//T = GameObject::Find("TileMap")->GetComponent<TileMap>();
	if (auto go = GameObject::Find("TileMap"); go.IsValid())
	{
		T = go->GetComponent<TileMap>();
	}
}

void MMMEngine::PlayerMove::Update()
{
	float x = 0.f, z = 0.f;
	if (Input::GetKey(KeyCode::W)) z += 1.f;
	if (Input::GetKey(KeyCode::S)) z -= 1.f;
	if (Input::GetKey(KeyCode::D)) x += 1.f;
	if (Input::GetKey(KeyCode::A)) x -= 1.f;

	if (Input::GetKeyDown(KeyCode::O)) {
		std::cout << "key down O" << std::endl;
		if (T.IsValid())	T->NoticePlayer(true);
		else std::cout << "Tile Not Found" << std::endl;
	}

	if (Input::GetKeyDown(KeyCode::P)) {
		std::cout << "key down P" << std::endl;
		if (T.IsValid()) T->NoticePlayer(false);
		else std::cout << "Tile Not Found" << std::endl;
	}
	m_InputDir = Vector3(x, 0.f, z);
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
			Vector3 myPos = GetTransform()->GetWorldPosition();          // 네 API로 교체
			Vector3 to = Snow->GetTransform()->GetWorldPosition() - myPos;
			to.y = 0.f;

			if (to.LengthSquared() > 1e-6f)
			{
				to.Normalize();
				float targetYaw = std::atan2(to.x, to.z);
				rb->SnapRotation(Quaternion::CreateFromAxisAngle(Vector3::Up, targetYaw));
				return;
			}
		}
		return;
	}

	ApplyYawFromVelocity(desiredVel);
}



Vector3 MMMEngine::PlayerMove::ComputeDesiredVelocity()
{
	Vector3 dir = m_InputDir;
	if (dir.LengthSquared() > 0.f)
	{
		dir.Normalize();
		return dir * basespeed;
	}
	return Vector3(0, 0, 0);

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
		dt = TimeManager::Get().GetDeltaTime();
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
	m_LookTarget = value;
	is_Scoop = value;
	isSlow = value;
}
