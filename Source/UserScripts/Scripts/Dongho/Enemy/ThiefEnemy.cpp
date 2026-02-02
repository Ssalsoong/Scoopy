#include "Export.h"
#include "ScriptBehaviour.h"
#include "ThiefEnemy.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Enemy.h"

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
	Enemycomp->stats.HP = HP;
	Enemycomp->stats.atk = atk;
	Enemycomp->stats.velocity = velocity;
	Enemycomp->stats.attackDelay = attackDelay;
	Enemycomp->stats.battledist = battledist;
	Enemycomp->stats.checkdist = checkdist;
}