#include "Export.h"
#include "ScriptBehaviour.h"
#include "TimerGageScript.h"
#include "../Dongho/Manager/GameManager.h"

void MMMEngine::TimerGageScript::Start()
{
	mTimerGage = GetComponent<Gage>();

	if (!mTimerGage) {
		std::cout << "TimerGageScript::Gage Not Found!!" << std::endl;
		Destroy(SelfPtr(this));
	}
}

void MMMEngine::TimerGageScript::Update()
{
	bool stat = GameManager::instance->nowSetting;

	if (mPrevStat != stat) {
		if (stat) {
			
		}
	}

	if (stat) {
		float waveTime = GameManager::instance->settingfullTime;
		float currTime = GameManager::instance->settingTimer;

		float factor = currTime / waveTime;

		mTimerGage->SetValue(1.0f - factor);
	}
}
