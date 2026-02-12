#include "Export.h"
#include "ScriptBehaviour.h"
#include "../Building/BuffBuilding.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "../Building/Building.h"
#include "../../test/PlayerMove.h"
#include "Transform.h"
#include "../Battlestats.h"
#include "../Player/Player.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<BuffBuilding>("BuffBuilding")
		(rttr::metadata("wrapper_type_name", "ObjPtr<BuffBuilding>"))
		.property("speedbuff", &BuffBuilding::speedbuff);

	registration::class_<ObjPtr<BuffBuilding>>("ObjPtr<BuffBuilding>")
		.constructor(
			[]() {
				return Object::NewObject<BuffBuilding>();
			})
        .method("Inject", &ObjPtr<BuffBuilding>::Inject);
}

void MMMEngine::BuffBuilding::Start()
{
	player = GetGameObject()->Find("Player");
}

void MMMEngine::BuffBuilding::Update()
{
	if (!player) return;
	playerpos = player->GetTransform()->GetWorldPosition();
	GiveBuff();
}

void MMMEngine::BuffBuilding::GiveBuff()
{
	auto pos = GetTransform()->GetWorldPosition();
	float bestD2 = buffdist * buffdist;
	float dx = pos.x - playerpos.x;
	float dz = pos.z - playerpos.z;
	float d2 = dx * dx + dz * dz;
	bool nowInRange = (d2 < bestD2);
	if (auto playermove = player->GetComponent<PlayerMove>();playermove.IsValid()) {
		if (auto playercomp = player->GetComponent<Player>(); playercomp.IsValid())
		{
			if (nowInRange && !prevInRange)
			{
				playermove->AddBuffSource(this, speedbuff);
				playercomp->AddAttackBuffSource(this, attackbuff);
			}
			else if (!nowInRange && prevInRange)
			{
				playermove->RemoveBuffSource(this);
				playercomp->AddAttackBuffSource(this, attackbuff);
			}
			prevInRange = nowInRange;
		}
	}
}

void MMMEngine::BuffBuilding::LevelApply(int level)
{
	if (level == 1)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 70;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(70);
		speedbuff = 10.f;
		attackbuff = 5;
		buffdist = 2.0f;
	}
	if (level == 2)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 90;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(90);
		speedbuff = 20.f;
		attackbuff = 7;
		buffdist = 2.5f;
	}
	if (level == 3)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 110;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(110);
		speedbuff = 30.f;
		attackbuff = 9;
		buffdist = 3.0f;
	}
	if (level == 4)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 130;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(130);
		speedbuff = 40.f;
		attackbuff = 11;
		buffdist = 3.5f;
	}
	if (level == 5)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 150;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(150);
		speedbuff = 50.f;
		attackbuff = 15;
		buffdist = 4.0f;
	}
}