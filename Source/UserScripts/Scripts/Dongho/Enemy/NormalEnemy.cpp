#include "NormalEnemy.h"
#include "Enemy.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<NormalEnemy>("NormalEnemy")
		(rttr::metadata("wrapper_type_name", "ObjPtr<NormalEnemy>"));

	registration::class_<ObjPtr<NormalEnemy>>("ObjPtr<NormalEnemy>")
		.constructor(
			[]() {
				return Object::NewObject<NormalEnemy>();
			}).method("Inject", &ObjPtr<NormalEnemy>::Inject);
}

void MMMEngine::NormalEnemy::Start()
{

}

void MMMEngine::NormalEnemy::Update()
{

}

void MMMEngine::NormalEnemy::ApplyStats()
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