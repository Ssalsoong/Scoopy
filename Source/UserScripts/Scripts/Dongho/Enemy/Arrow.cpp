#include "Export.h"
#include "ScriptBehaviour.h"
#include "../Enemy/Arrow.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "../Enemy/ArrowEnemy.h"
#include "../Enemy/Enemy.h"
#include "Transform.h"
#include "MMMTime.h"
#include "../Player/Player.h"
#include "../Castle/Castle.h"
#include "../Building/Building.h"
#include "../Manager/BattleManager.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<Arrow>("Arrow")
        (rttr::metadata("wrapper_type_name", "ObjPtr<Arrow>"));

	registration::class_<ObjPtr<Arrow>>("ObjPtr<Arrow>")
		.constructor(
			[]() {
				return Object::NewObject<Arrow>();
			})
        .method("Inject", &ObjPtr<Arrow>::Inject);
}

void MMMEngine::Arrow::Start()
{

}

void MMMEngine::Arrow::Update()
{
	// 타겟이 없거나 죽었거나 비활성화면 회수
	if (!target)
	{
		owner->GetComponent<ArrowEnemy>()->ReturnArrow(GetGameObject());
		GetTransform()->SetLocalPosition(0.f, 0.f, 0.f);
		GetGameObject()->SetActive(false);
		return;
	}
	targetpos = target->GetTransform()->GetWorldPosition();
	atk = owner->GetComponent<Enemy>()->atk;
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
		BattleManager::instance->Attack(target, atk);

		owner->GetComponent<ArrowEnemy>()->ReturnArrow(GetGameObject());
		target = nullptr;
		GetTransform()->SetLocalPosition(0.f, 0.f, 0.f);
		GetGameObject()->SetActive(false);
		return;
	}
}
