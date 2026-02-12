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
#include "../../Sunken/ControlManager.h"
#include "../../Mingi/UI/PauseUI.h"
#include "../../Mingi/UI/MarkWaveController.h"
#include "../../Mingi/UI/GameOverSequencer.h"
#include "../../Mingi/UI/GameClearSequencer.h"
#include "../../Sunken/PlayerAnimController.h"
#include "../../Mingi/Manager/SoundManager.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<GameManager>("GameManager")
		(rttr::metadata("wrapper_type_name", "ObjPtr<GameManager>"))
		.property("mTimerUI", &GameManager::mTimerUI)
		.property("mPauseUI", &GameManager::mPauseUI)
		.property("mOverSequencer", &GameManager::mOverSequencer)
		.property("mClearSequencer", &GameManager::mClearSequencer)
		.property("enemySpawnDelay", &GameManager::enemySpawnDelay)
		.property("settingfullTime", &GameManager::settingfullTime);
		

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


	MarkWaveController::Instance->SetWave(wave);

	if (!mTimerUI) {
		std::cout << "GameManager::TimerUI Not Found!!!" << std::endl;
	}
	else {
		mTimerUI->SetMaxWaveNum(mMaxWave);
		mTimerUI->SetWaveCount(0);
	}

	if (!mPauseUI) {
		std::cout << "GameManager::PauseUI Not Found!!!" << std::endl;
	}

	if (!mOverSequencer) {
		std::cout << "GameManager::GameOverSeq Not Found!!!" << std::endl;
	}
}

void MMMEngine::GameManager::Update()
{
	if (GameWin)
	{
		if (!mClearSet)
		{
			mClearSet = true;

			if (mClearSequencer)
			{
				ControlManager::Get()->SetMinLayer(150);
				mClearSequencer->StartGameClear();
			}
		}
	}

	if (GameOver)
	{
		if (!mOverSet) {
			mOverSet = true;

			if (mOverSequencer) {
				ControlManager::Get()->SetMinLayer(150);
				mOverSequencer->StartGameOver();
			}

			if (auto bs = player->GetComponent<Battlestats>(); bs) {
				bs->SetHP(0);
			}

			if (auto controller = player->GetComponent<PlayerAnimController>(); controller) {
				controller->PlayDie();
			}
		}
	}

	if (nowSetting)
	{
		if (!mPlayingNormalBGM) {
			mPlayingNormalBGM = true;
			mPlayingWaveBGM = false;
			SoundManager::Instance->StopBGM();
			SoundManager::Instance->PlayBGM("TitleTheme");
		}

		settingTimer += Time::GetDeltaTime();
		if (settingTimer >= settingfullTime)
		{

			MarkWaveController::Instance->AllOff();
			EnemySpawner::instance->WaveSetting(wave);
			nowSetting = false;
			settingTimer = 0.0f;
			playercomp->Setbuildchance(false);
		}
	}
	else
	{
		if (!mPlayingWaveBGM) {
			mPlayingNormalBGM = false;
			mPlayingWaveBGM = true;
			SoundManager::Instance->StopBGM();
			SoundManager::Instance->PlayBGM("BattleTheme");
		}

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

			MarkWaveController::Instance->SetWave(wave);
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

	static bool prevSetting = false;

	if (prevSetting != nowSetting) {
		prevSetting = nowSetting;
		if (mTimerUI.IsValid()) {
			if (prevSetting)
				mTimerUI->SwitchTimer();
			else
				mTimerUI->SwitchWave();
		}
		
	}

	// Pause Key Ž��
	if (mPauseUI.IsValid()) {
		bool pauseStat = mPauseUI->IsPause();
		if (isPausing != pauseStat) {
			isPausing = pauseStat;
			if (pauseStat) {
				SoundManager::Instance->PlaySFX2D("Pause", SelfPtr(this));
				ControlManager::Get()->SetMinLayer(100);
			}
			else {
				SoundManager::Instance->PlaySFX2D("Pause", SelfPtr(this));
				ControlManager::Get()->ReleaseMinLayer();
			}	
		}
	}
}