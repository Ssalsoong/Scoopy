#include "Export.h"
#include "ScriptBehaviour.h"
#include "SnowBuilding.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Building.h"
#include "MMMTime.h"
#include "../Battlestats.h"
#include "Transform.h"
#include "../../test/PlayerController.h"
#include "../../test/SnowCollider.h"
#include "../Player/Player.h"
#include "../Snow/Snowball.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<SnowBuilding>("SnowBuilding")
		(rttr::metadata("wrapper_type_name", "ObjPtr<SnowBuilding>"))
		.property("snowpoint", &SnowBuilding::snowpoint);

	registration::class_<ObjPtr<SnowBuilding>>("ObjPtr<SnowBuilding>")
		.constructor(
			[]() {
				return Object::NewObject<SnowBuilding>();
			})
        .method("Inject", &ObjPtr<SnowBuilding>::Inject);
}

void MMMEngine::SnowBuilding::Start()
{
	player = GetGameObject()->Find("Player");
}

void MMMEngine::SnowBuilding::Update()
{
	playerpos = player->GetTransform()->GetWorldPosition();
	UpdateSnow();
	ProvideSnow();
}

void MMMEngine::SnowBuilding::UpdateSnow()
{
	chargeTimer += Time::GetDeltaTime();
	if (chargeTimer >= chargeDelay)
	{
		if(snowpoint<maxsnowpoint)
		snowpoint++;
		chargeTimer = 0.0f;
	}
}

void MMMEngine::SnowBuilding::LevelApply(int level)
{
	if (level == 1)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 70;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(70);
		chargeDelay = 2.0f;
	}
	if (level == 2)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 90;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(50);
		chargeDelay = 1.5f;
	}
	if (level == 3)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 110;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(110);
		chargeDelay = 1.0f;
	}
	if (level == 4)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 130;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(130);
		chargeDelay = 0.5f;
	}
	if (level == 5)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 150;
		GetGameObject()->GetComponent<Battlestats>()->SetHP(150);
		chargeDelay = 0.25f;
		provideDelay = 0.25f;
	}
}

void MMMEngine::SnowBuilding::ProvideSnow()
{
	if (snowpoint <= 0)
		return;
	auto pos = GetTransform()->GetWorldPosition();
	float bestD2 = snowProvidedist * snowProvidedist;
	float dx = pos.x - playerpos.x;
	float dz = pos.z - playerpos.z;
	float d2 = dx * dx + dz * dz;
	bool nowInRange = (d2 < bestD2);
	if (!nowInRange)
		return;
	auto playercontroller = player->GetComponent<PlayerController>();
	if (playercontroller->HasCurrentSnow())
	{
		auto curSnow = playercontroller->GetCurSnow();
		auto curPoint = curSnow->GetComponent<Snowball>()->GetPoint();
		auto maxpoint = player->GetComponent<Player>()->Getmaxpoint();
		if (curPoint >= maxpoint)
			return;
		provideTimer += Time::GetDeltaTime();
		if (provideTimer >= provideDelay)
		{
			curSnow->GetComponent<Snowball>()->SetPoint(curPoint + 1);
			curSnow->GetComponent<SnowCollider>()->SetScoopCount(curPoint);
			snowpoint--;
			provideTimer = 0.0f;
		}
	}
}