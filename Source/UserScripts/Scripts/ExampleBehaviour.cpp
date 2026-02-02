#include "Export.h"
#include "ScriptBehaviour.h"
#include "ExampleBehaviour.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "MMMInput.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<ExampleBehaviour>("ExampleBehaviour")
		(rttr::metadata("wrapper_type_name", "ObjPtr<ExampleBehaviour>"))
		.property("IsCustomBool",&ExampleBehaviour::basespeed)
		.property("IsScoop", &ExampleBehaviour::is_Scoop)
		;


	registration::class_<ObjPtr<ExampleBehaviour>>("ObjPtr<ExampleBehaviour>")
		.constructor(
			[]() {
				return Object::NewObject<ExampleBehaviour>();
			})
        .method("Inject", &ObjPtr<ExampleBehaviour>::Inject);
}

void MMMEngine::ExampleBehaviour::Start()
{

}

void MMMEngine::ExampleBehaviour::Update()
{
	float x = 0.f, z = 0.f;
	if (Input::GetKey(KeyCode::W)) z += 1.f;
	if (Input::GetKey(KeyCode::S)) z -= 1.f;
	if (Input::GetKey(KeyCode::D)) x += 1.f;
	if (Input::GetKey(KeyCode::A)) x -= 1.f;

	m_InputDir = Vector3(x, 0.f, z);
}

void MMMEngine::ExampleBehaviour::FixedUpdate()
{
	auto rb = GetComponent<RigidBodyComponent>();


    Vector3 desiredVel = ComputeDesiredVelocity();

    rb->SetLinearVelocity(desiredVel);

    ApplyYawFromVelocity(desiredVel);
}


void MMMEngine::ExampleBehaviour::OnCollisionEnter(MMMEngine::CollisionInfo info)
{
	std::cout << "Enter" << std::endl;
	/*m_HasWall = false;*/
}

Vector3 MMMEngine::ExampleBehaviour::ComputeDesiredVelocity()
{
	Vector3 dir = m_InputDir;
	if (dir.LengthSquared() > 0.f)
	{
		dir.Normalize();
		return dir * basespeed;
	}
	return Vector3(0, 0, 0);
}


void MMMEngine::ExampleBehaviour::ApplyYawFromVelocity(const Vector3& v)
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

float MMMEngine::ExampleBehaviour::WrapPi(float a)
{
	const float pi = 3.1415926535f;
	while (a > pi) a -= 2.f * pi;
	while (a < -pi) a += 2.f * pi;
	return a;
}
