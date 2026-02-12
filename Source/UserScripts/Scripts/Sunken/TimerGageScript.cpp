#include "Export.h"
#include "ScriptBehaviour.h"
#include "TimerGageScript.h"
#include "../Dongho/Manager/GameManager.h"
#include "../Mingi/UI/TimerUI.h"

void MMMEngine::TimerGageScript::Start()
{
	mTimerGage = GetComponent<Gage>();

	if (!mTimerGage) {
		std::cout << "TimerGageScript::Gage Not Found!!" << std::endl;
		Destroy(SelfPtr(this));
	}

	mTimerUI = GetTransform()->GetParent()->GetComponent<TimerUI>();

	if (!mTimerUI.IsValid()) {
		std::cout << "TimerGageScript::TiemrUI Not Found!!" << std::endl;
		Destroy(SelfPtr(this));
	}
}

void MMMEngine::TimerGageScript::Update()
{
	bool stat = GameManager::instance->nowSetting;

	/*if (mPrevStat != stat) {
		if (stat) {

		}
	}*/

	if (stat) {
		if (mTimerUI->IsPlayingAnimation())
			return;
		
		float waveTime = GameManager::instance->settingfullTime;
		float currTime = GameManager::instance->settingTimer;

		float factor = currTime / waveTime;

		mTimerGage->SetValue(1.0f - factor);
	}
}
