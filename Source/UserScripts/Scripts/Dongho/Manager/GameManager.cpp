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

#include "../../Mingi/UI/TimerUI.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<GameManager>("GameManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<GameManager>"))
		.property("mTimerUI", &GameManager::mTimerUI);

	registration::class_<ObjPtr<GameManager>>("ObjPtr<GameManager>")
		.constructor(
			[]() {
				return Object::NewObject<GameManager>();
			}).method("Inject", &ObjPtr<GameManager>::Inject);
}

MMMEngine::ObjPtr<MMMEngine::GameManager> MMMEngine::GameManager::instance = nullptr;

void MMMEngine::GameManager::Awake()
{
	instance = GetGameObject()->GetComponent<GameManager>();
}

void MMMEngine::GameManager::Start()
{
	player = GetGameObject()->Find("Player");
	castle = GetGameObject()->Find("Castle");
	playercomp = player->GetComponent<Player>();
	castlecomp = castle->GetComponent<Castle>();

	if (!mTimerUI) {
		std::cout << "GameManager::TimerUI Not Found!!!" << std::endl;
	}
	else {
		mTimerUI->SetMaxWaveNum(mMaxWave);
		mTimerUI->SetWaveCount(0);
	}
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
			playercomp->Setbuildchance(false);
		}
	}
	else
	{
		if (!EnemySpawner::instance->WaveSpawn(wave))
		{
			if (wave == mMaxWave) {
				GameWin = true;
				return;
			}
			nowSetting = true;
			wave += 1;
			BuildingManager::instance->BuildingReturn();
			EnemySpawner::instance->EnemyUpgrade();
			playercomp->Setbuildchance(true);
			player->GetComponent<Battlestats>()->SetHP(playercomp->GetmaxHP());
			castle->GetComponent<Battlestats>()->SetHP(castlecomp->GetmaxHP());
			if (castlecomp->waveexp)
			{
				int BuildingCount = BuildingManager::instance->GetBuildingCount();
				castlecomp->Getexp(BuildingCount * 10);
			}
		}
	}

	static bool prevSetting = nowSetting;

	if (prevSetting != nowSetting) {
		prevSetting = nowSetting;
		if (!prevSetting) {
			if (mTimerUI.IsValid()) {
				mTimerUI->ShowNextWave();
			}
		}
	}
}