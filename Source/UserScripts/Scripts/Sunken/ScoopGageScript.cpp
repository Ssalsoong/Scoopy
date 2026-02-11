#include "Export.h"
#include "ScriptBehaviour.h"
#include "ScoopGageScript.h"

#include "../test/PlayerController.h"
#include "../Dongho/Snow/Snowball.h"
#include "../Dongho/Player/Player.h"

void MMMEngine::ScoopGageScript::Start()
{
	if (auto player = GameObject::FindWithTag("Player"); player.IsValid()) {
		mPlayerController = player->GetComponent<PlayerController>();
		mPlayer = player->GetComponent<Player>();

		if (!mPlayerController.IsValid()) {
			std::cout << "ScoopGageScript::PlayerController Not Found !!!" << std::endl;
			Destroy(GetGameObject());
		}
		if (!mPlayer.IsValid()) {
			std::cout << "ScoopGageScript::Player Not Found !!!" << std::endl;
			Destroy(GetGameObject());
		}
	}
	else {
		std::cout << "ScoopGageScript::PlayerObject Not Found !!!" << std::endl;
		Destroy(GetGameObject());
	}

	if (!mGage.IsValid()) {
		std::cout << "ScoopGageScript::Gage Not Found !!!" << std::endl;
		Destroy(GetGameObject());
	}

	if (!mActiveImg.IsValid()) {
		std::cout << "ScoopGageScript::ActiveIMG Not Found !!!" << std::endl;
		Destroy(GetGameObject());
	}

	if (!mDeactiveImg.IsValid()) {
		std::cout << "ScoopGageScript::DeactiveIMG Not Found !!!" << std::endl;
		Destroy(GetGameObject());
	}
	
}

void MMMEngine::ScoopGageScript::Update()
{
	if (mPlayerController.IsValid()) {
		if (auto snowObj = mPlayerController->GetCurSnow(); snowObj.IsValid()) {
			if (auto snow = snowObj->GetComponent<Snowball>(); snow.IsValid()) {
				int snowSize = snow->point;
				int maxSize = mPlayer->maxpoint;

				if (mGage && maxSize > 0) {
					mGage->SetValue(static_cast<float>(snowSize) / static_cast<float>(maxSize));
				}
			}
		}
		else if (mGage) {
			mGage->SetValue(0.0f);
		}

		if (mPlayerController->IsHoldingSpace()) {
			if (mActiveImg)   mActiveImg->SetAlpha(1.0f);
			if (mDeactiveImg) mDeactiveImg->SetAlpha(0.0f);
		}
		else {
			if (mActiveImg)   mActiveImg->SetAlpha(0.0f);
			if (mDeactiveImg) mDeactiveImg->SetAlpha(1.0f);
		}
	}
}
