#include "Snowball.h"
#include "MMMTime.h"
#include "../Player/Player.h"
#include "Transform.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<Snowball>("Snowball")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Snowball>"));

	registration::class_<ObjPtr<Snowball>>("ObjPtr<Snowball>")
		.constructor(
			[]() {
				return Object::NewObject<Snowball>();
			}).method("Inject", &ObjPtr<Snowball>::Inject);
}

void MMMEngine::Snowball::Start()
{

}

void MMMEngine::Snowball::Update()
{
	if (IsCarried())
	{
		RollSnow();
	}
	GetTransform()->SetWorldScale(scale, scale, scale);
}

void MMMEngine::Snowball::RollSnow()
{
	// 스케일 계산 (포인트 기반)
	point = std::min(point, carrier->info.maxpoint);
	scale = minscale + scaleup * point;
	float r = baseRadius * scale;
	float distance = r * k;
	GetTransform()->SetLocalPosition(DirectX::SimpleMath::Vector3::Backward * distance);
	auto worldPos = GetTransform()->GetWorldPosition();
	if (!hasPrev) { prevWorldPos = worldPos; hasPrev = true; return; }
	
	auto delta = worldPos - prevWorldPos;
	delta.y = 0.0f;

	float ds = delta.Length();
	if (ds > 1e-5f && r > 1e-5f)
	{
		auto dir = delta / ds;

		auto axis = DirectX::SimpleMath::Vector3::Up.Cross(dir);
		if (axis.LengthSquared() > 1e-8f) axis.Normalize();

		float dTheta = ds / r;

		auto deltaQ = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, dTheta);

		auto currQ = GetTransform()->GetWorldRotation();
		auto nextQ = deltaQ * currQ;
		GetTransform()->SetWorldRotation(nextQ);
	}
	prevWorldPos = worldPos;
}

void MMMEngine::Snowball::EatSnow(ObjPtr<GameObject> other)
{
	if (!other || other == GetGameObject()) return;
	auto snowcomp = other->GetComponent<Snowball>();
	if (snowcomp == nullptr)
		return;
	point += snowcomp->GetPoint();
	point = std::min(point, carrier->info.maxpoint);
	Destroy(other);
}
