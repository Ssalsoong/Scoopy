#include "Export.h"
#include "ScriptBehaviour.h"
#include "Buildingball.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Building.h"
#include "Transform.h"
#include "MMMTime.h"
#include "../Manager/BattleManager.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<Buildingball>("Buildingball")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Buildingball>"));

	registration::class_<ObjPtr<Buildingball>>("ObjPtr<Buildingball>")
		.constructor(
			[]() {
				return Object::NewObject<Buildingball>();
			})
		.method("Inject", &ObjPtr<Buildingball>::Inject);
}

void MMMEngine::Buildingball::Start()
{

}

void MMMEngine::Buildingball::Update()
{
	//// 타겟이 없거나 죽었거나 비활성화면 회수
	//if (!target)
	//{
	//	owner->GetComponent<Building>()->ReturnBall(GetGameObject());
	//	auto ownerpos = owner->GetTransform()->GetWorldPosition();
	//	GetTransform()->SetWorldPosition(ownerpos);
	//	GetGameObject()->SetActive(false);
	//	return;
	//}
	//targetpos = target->GetTransform()->GetWorldPosition();
	//auto pos = GetTransform()->GetWorldPosition();


	// 이동 후 히트 판정
	//auto left = targetpos - pos;
	//left.y = 0.0f;

	//if (left.LengthSquared() <= hitRadius * hitRadius)
	//{
	//	// 데미지(컴포넌트로 판별)
	//	BattleManager::instance->Attack(owner, target, atk);

	//	owner->GetComponent<Building>()->ReturnBall(GetGameObject());
	//	target = nullptr;
	//	auto ownerpos = owner->GetTransform()->GetWorldPosition();
	//	GetTransform()->SetWorldPosition(ownerpos);
	//	GetGameObject()->SetActive(false);
	//	return;
	//}
}

void MMMEngine::Buildingball::Returnball()
{
	owner->GetComponent<Building>()->ReturnBall(GetGameObject());
	auto ownerpos = owner->GetTransform()->GetWorldPosition();
	GetTransform()->SetWorldPosition(ownerpos);
}

void MMMEngine::Buildingball::Attacktarget()
{
	BattleManager::instance->Attack(owner, target, atk);
	owner->GetComponent<Building>()->ReturnBall(GetGameObject());
	target = nullptr;
	auto ownerpos = owner->GetTransform()->GetWorldPosition();
	GetTransform()->SetWorldPosition(ownerpos);
}