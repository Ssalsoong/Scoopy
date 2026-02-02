#include "Export.h"
#include "ScriptBehaviour.h"
#include "Castleball.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Castle.h"
#include "MMMTime.h"
#include "Transform.h"
#include "../Enemy/Enemy.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<Castleball>("Castleball")
        (rttr::metadata("wrapper_type_name", "ObjPtr<Castleball>"));

	registration::class_<ObjPtr<Castleball>>("ObjPtr<Castleball>")
		.constructor(
			[]() {
				return Object::NewObject<Castleball>();
			})
        .method("Inject", &ObjPtr<Castleball>::Inject);
}

void MMMEngine::Castleball::Start()
{

}

void MMMEngine::Castleball::Update()
{
	// 타겟이 없거나 죽었거나 비활성화면 회수
	if (!target)
	{
		owner->GetComponent<Castle>()->ReturnBall(GetGameObject());
		GetTransform()->SetLocalPosition(0.f, 0.f, 0.f);
		GetGameObject()->SetActive(false);
		return;
	}
	targetpos = target->GetTransform()->GetWorldPosition();
	atk = owner->GetComponent<Castle>()->atk;
	auto pos = GetTransform()->GetWorldPosition();

	// 방향/거리
	auto to = targetpos - pos;
	to.y = 0.0f;

	float dist2 = to.LengthSquared();
	if (dist2 < 1e-8f) dist2 = 0.0f;

	float dist = sqrtf(dist2);

	// 이동 (관통 방지: 남은 거리 이상 못 가게 clamp)
	float dt = Time::GetDeltaTime();
	float step = speed * dt;
	if (step > dist) step = dist;

	if (dist > 1e-6f)
	{
		auto dir = to / dist;
		pos += dir * step;
		GetTransform()->SetWorldPosition(pos);
	}

	// 이동 후 히트 판정
	auto left = targetpos - pos;
	left.y = 0.0f;

	if (left.LengthSquared() <= hitRadius * hitRadius)
	{
		// 데미지(컴포넌트로 판별)
		target->GetComponent<Enemy>()->GetDamage(atk);

		owner->GetComponent<Castle>()->ReturnBall(GetGameObject());
		target = nullptr;
		GetTransform()->SetLocalPosition(0.f, 0.f, 0.f);
		GetGameObject()->SetActive(false);
		return;
	}
}