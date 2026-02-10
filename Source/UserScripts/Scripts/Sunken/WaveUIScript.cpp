#include "Export.h"
#include "ScriptBehaviour.h"
#include "WaveUIScript.h"
#include "../Dongho/Manager/GameManager.h"

void MMMEngine::WaveUIScript::Start()
{
	mWaveBackground = GetComponent<Image>();
	if (!mWaveBackground) {
		std::cout << "WaveUIScript::WaveBackground Not Found!!" << std::endl;
		Destroy(SelfPtr(this));
	}

	// 가져오기 시도
	if (!mWaveText) {
		for (int i = 0; i < GetTransform()->GetChildCount(); i++) {
			mWaveText = GetTransform()->GetChild(i)->GetComponent<Text>();
			if (mWaveText)
				break;
		}
	}

	if (!mWaveText) {
		std::cout << "WaveUIScript::WaveBackground Not Found!!" << std::endl;
		Destroy(SelfPtr(this));
	}
}

void MMMEngine::WaveUIScript::Update()
{
	bool stat = GameManager::instance->nowSetting;

	if (mPrevStat != stat) {
		if (stat) {
			mWaveBackground->SetEnabled(true);
			mWaveText->SetEnabled(true);

			int currWave = GameManager::instance->wave;
			int maxWave = GameManager::instance->mMaxWave;

			std::wstring waveText = std::to_wstring(currWave) + L" / " + std::to_wstring(maxWave);
			mWaveText->SetText(waveText);
		}
		else {
			mWaveBackground->SetEnabled(false);
			mWaveText->SetEnabled(false);
		}
		mPrevStat = stat;
	}
}
