#include "NormalEnemy.h"
#include "Enemy.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "../Battlestats.h"

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
	GetComponent<Battlestats>()->HP = HP;
	Enemycomp->atk = atk;
	Enemycomp->velocity = velocity;
	Enemycomp->attackDelay = attackDelay;
	Enemycomp->battledist = battledist;
	Enemycomp->checkdist = checkdist;
}