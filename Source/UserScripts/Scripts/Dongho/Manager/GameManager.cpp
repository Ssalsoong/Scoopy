#include "GameManager.h"
#include "../Player/Player.h"
#include "../Castle/Castle.h"
#include "MMMTime.h"
#include "Transform.h"
#include "EnemySpawner.h"
#include "MeshRenderer.h"
#include "StaticMesh.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "BuildingManager.h"
#include "../Battlestats.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<GameManager>("GameManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<GameManager>"));

	registration::class_<ObjPtr<GameManager>>("ObjPtr<GameManager>")
		.constructor(
			[]() {
				return Object::NewObject<GameManager>();
			}).method("Inject", &ObjPtr<GameManager>::Inject);
}

MMMEngine::ObjPtr<MMMEngine::GameManager> MMMEngine::GameManager::instance = nullptr;

void MMMEngine::GameManager::Start()
{
	player = GetGameObject()->Find("Player");
	castle = GetGameObject()->Find("Castle");
	playercomp = player->GetComponent<Player>();
	castlecomp = castle->GetComponent<Castle>();
	instance = GetGameObject()->GetComponent<GameManager>();
}

void MMMEngine::GameManager::Update()
{
	if (GameWin)
	{

	}

	if (GameOver)
	{

	}

	if (nowSetting)
	{
		settingTimer += Time::GetDeltaTime();
		if (settingTimer >= settingfullTime)
		{
			EnemySpawner::instance->WaveSetting(wave);
			nowSetting = false;
			settingTimer = 0.0f;
			playercomp->buildchance = false;
		}
	}
	else
	{
		if (!EnemySpawner::instance->WaveSpawn(wave))
		{
			if (wave == 10) {
				GameWin = true;
				return;
			}
			nowSetting = true;
			wave += 1;
			BuildingManager::instance->BuildingReturn();
			EnemySpawner::instance->EnemyUpgrade();
			playercomp->buildchance = true;
			player->GetComponent<Battlestats>()->HP = playercomp->maxHP;
			castle->GetComponent<Battlestats>()->HP = castlecomp->maxHP;
		}
	}
}