#include "Export.h"
#include "ScriptBehaviour.h"
#include "ArrowEnemy.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Enemy.h"
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
	arrowmesh = ResourceManager::Get().Load<StaticMesh>(L"Assets/DefaultMesh/Sphere_StaticMesh.staticmesh");
	for (int i = 0; i < 5; ++i)
	{
		auto obj = NewObject<GameObject>();
		obj->SetName("Arrow");
		obj->SetTag("Arrow");
		obj->GetTransform()->SetParent(GetTransform());
		obj->AddComponent<Arrow>();
		obj->GetComponent<Arrow>()->SetOwner(GetGameObject());
		obj->AddComponent<MeshRenderer>();
		obj->GetComponent<MeshRenderer>()->SetMesh(arrowmesh);
		obj->GetTransform()->SetLocalPosition(0.f, 0.f, 0.f);
		obj->GetTransform()->SetWorldScale(0.1f,0.1f,0.1f);
		obj->SetActive(false);
		Arrows.push(obj);
	}
}
void MMMEngine::ArrowEnemy::Update()
{
}

void MMMEngine::ArrowEnemy::ApplyStats()
{
	if (!GetGameObject()->GetComponent<Enemy>())
		return;
	auto Enemycomp = GetGameObject()->GetComponent<Enemy>();
	GetComponent<Battlestats>()->HP = HP;
	Enemycomp->atk = atk;
	Enemycomp->velocity = velocity;
	Enemycomp->attackDelay = attackDelay;
	Enemycomp->battledist = battledist;
	Enemycomp->checkdist = checkdist;
}

void MMMEngine::ArrowEnemy::ArrowAttack(ObjPtr<GameObject> target)
{
	if (Arrows.empty())
		return;
	auto obj = Arrows.front();
	Arrows.pop();
	if (!obj)
		return;
	obj->GetComponent<Arrow>()->SetTarget(target);
	obj->SetActive(true);
}

void MMMEngine::ArrowEnemy::ReturnArrow(ObjPtr<GameObject> obj)
{
	Arrows.push(obj);
}