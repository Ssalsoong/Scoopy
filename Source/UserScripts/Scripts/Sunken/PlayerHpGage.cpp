#include "Export.h"
#include "ScriptBehaviour.h"
#include "PlayerHpGage.h"

#include "../Dongho/Player/Player.h"

void MMMEngine::PlayerHpGage::Start()
{
	if (!mPlayer) {
		mPlayer = GameObject::FindWithTag("Player")->GetComponent<Player>();
		if (!mPlayer) {
			std::cout << "PlayerHpGage::Player Not Found!!!" << std::endl;
			Destroy(SelfPtr(this));
		}
	}

	mGage = GetComponent<Gage>();
	if (!mGage) {
		std::cout << "PlayerHpGage::Gage Not Found!!!" << std::endl;
		Destroy(SelfPtr(this));
	}
}

void MMMEngine::PlayerHpGage::Update()
{
	int maxHP = mPlayer->GetmaxHP();
	int currHP = mPlayer->prevHP;

	float gageFactor = (float)currHP / (float)maxHP;

	mGage->SetValue(gageFactor);
}
