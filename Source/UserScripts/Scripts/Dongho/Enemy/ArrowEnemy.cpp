#include "Export.h"
#include "ScriptBehaviour.h"
#include "ArrowEnemy.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "MeshRenderer.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "Arrow.h"
#include "../Battlestats.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<ArrowEnemy>("ArrowEnemy")
        (rttr::metadata("wrapper_type_name", "ObjPtr<ArrowEnemy>"));

	registration::class_<ObjPtr<ArrowEnemy>>("ObjPtr<ArrowEnemy>")
		.constructor(
			[]() {
				return Object::NewObject<ArrowEnemy>();
			})
        .method("Inject", &ObjPtr<ArrowEnemy>::Inject);
}

void MMMEngine::ArrowEnemy::Start()
{
	pre_arrow = ResourceManager::Get().Load<Prefab>(L"Assets/Prefab/Arrow.Prefab");
	for (int i = 0; i < 5; ++i)
	{
		auto obj = Instantiate(pre_arrow);
		obj->GetTransform()->SetParent(GetTransform());
		obj->GetTransform()->SetLocalPosition(0.f, 0.2f, 0.f);
		obj->GetComponent<Arrow>()->SetOwner(GetGameObject());
		obj->SetActive(false);
		Arrows.push(obj);
	}
}
void MMMEngine::ArrowEnemy::Update()
{
}

void MMMEngine::ArrowEnemy::ArrowAttack(ObjPtr<GameObject> target, int atk)
{
	if (Arrows.empty())
		return;
	
	auto obj = Arrows.front();
	Arrows.pop();
	if (!obj)
		return;
	if (auto arrow = obj->GetComponent<Arrow>(); arrow.IsValid())
	{
		arrow->SetTarget(target);
		arrow->SetAtk(atk);
	}

	obj->SetActive(true);
}

void MMMEngine::ArrowEnemy::LookAt(const DirectX::SimpleMath::Vector3& target)
{
	auto pos = GetTransform()->GetWorldPosition();
	auto dir = target - pos;
	dir.y = 0.0f;

	float len2 = dir.LengthSquared();
	if (len2 < 1e-8f) return;

	dir.Normalize();

	float yaw = atan2f(dir.x, dir.z);
	auto rot = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, 0, 0);
	GetTransform()->SetWorldRotation(rot);
}


void MMMEngine::ArrowEnemy::ReturnArrow(ObjPtr<GameObject> obj)
{
	Arrows.push(obj);
}