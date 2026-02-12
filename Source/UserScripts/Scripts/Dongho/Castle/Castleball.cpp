#include "Export.h"
#include "ScriptBehaviour.h"
#include "Castleball.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Castle.h"
#include "MMMTime.h"
#include "Transform.h"
#include "../Manager/BattleManager.h"
#include "../../Mingi/ExplosionPool.h"

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
		auto ownerpos = owner->GetTransform()->GetWorldPosition();
		GetTransform()->SetWorldPosition(ownerpos);
		GetGameObject()->SetActive(false);
		return;
	}
	targetpos = target->GetTransform()->GetWorldPosition();
	auto pos = GetTransform()->GetWorldPosition();


	// 이동 후 히트 판정
	auto left = targetpos - pos;
	left.y = 0.0f;

	if (left.LengthSquared() <= hitRadius * hitRadius)
	{
		// 데미지(컴포넌트로 판별)
		BattleManager::instance->Attack(owner, target, atk);

		owner->GetComponent<Castle>()->ReturnBall(GetGameObject());
		target = nullptr;
		auto ownerpos = owner->GetTransform()->GetWorldPosition();

		if (ExplosionPool::Instance.IsValid())
			ExplosionPool::Instance->Spawn(GetTransform()->GetWorldPosition());

		GetTransform()->SetWorldPosition(ownerpos);
		GetGameObject()->SetActive(false);
		return;
	}
}