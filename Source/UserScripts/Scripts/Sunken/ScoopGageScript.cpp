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

	
}

void MMMEngine::ScoopGageScript::Update()
{
	if (mPlayerController.IsValid()) {
		if (auto snowObj = mPlayerController->GetCurSnow(); snowObj.IsValid()) {
			if (auto snow = snowObj->GetComponent<Snowball>(); snow.IsValid()) {
				int snowSize = snow->point;
				int maxSize = mPlayer->maxpoint;

				//TODO 게이지 만들기 
			}
		}
	}
}
