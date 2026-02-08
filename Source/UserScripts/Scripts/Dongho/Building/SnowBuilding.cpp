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
	GetGameObject()->GetComponent<Battlestats>()->HP = 50;
	GetGameObject()->GetComponent<Building>()->maxHP = 50;

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
		GetGameObject()->GetComponent<Building>()->maxHP = 50;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		maxsnowpoint = 15;
	}
	if (level == 2)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 50;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		maxsnowpoint = 20;
	}
	if (level == 3)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 65;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		maxsnowpoint = 25;
	}
	if (level == 4)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 65;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		maxsnowpoint = 30;
	}
	if (level == 5)
	{
		GetGameObject()->GetComponent<Building>()->maxHP = 80;
		GetGameObject()->GetComponent<Battlestats>()->HP =
			GetGameObject()->GetComponent<Building>()->maxHP;
		maxsnowpoint = 35;
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