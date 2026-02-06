#include "Export.h"
#include "ScriptBehaviour.h"
#include "ThiefEnemy.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Enemy.h"
#include "../Battlestats.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<ThiefEnemy>("ThiefEnemy")
        (rttr::metadata("wrapper_type_name", "ObjPtr<ThiefEnemy>"));

	registration::class_<ObjPtr<ThiefEnemy>>("ObjPtr<ThiefEnemy>")
		.constructor(
			[]() {
				return Object::NewObject<ThiefEnemy>();
			})
        .method("Inject", &ObjPtr<ThiefEnemy>::Inject);
}

void MMMEngine::ThiefEnemy::Start()
{
}

void MMMEngine::ThiefEnemy::Update()
{

}

void MMMEngine::ThiefEnemy::ApplyStats()
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