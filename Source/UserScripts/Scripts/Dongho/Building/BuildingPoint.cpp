#include "Export.h"
#include "ScriptBehaviour.h"
#include "BuildingPoint.h"
#include "../Player/Player.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "../../Sunken/LevelUpManager.h"
#include "../../Mingi/UI/WorldSpaceUI.h"

void MMMEngine::BuildingPoint::Start()
{
	if(!player.IsValid())
		player = GetGameObject()->FindWithTag("Player");

	if (!player.IsValid()) {
		std::cout << "BuildingPoint::PlayerObj Not Found!!!" << std::endl;
		Destroy(GetGameObject());
	}
	else if (mPlayer = player->GetComponent<Player>(); !mPlayer.IsValid()) {
		std::cout << "BuildingPoint::PlayerScript Not Found!!!" << std::endl;
		Destroy(GetGameObject());
	}

	if (!LevelUpManager::Get()) {
		std::cout << "BuildingPoint::LevelUpManager Not Found!!" << std::endl;
		Destroy(GetGameObject());
	}

	// 게이지 셋팅
	if (auto gage = LevelUpManager::Get()->mUnivGagePrefab; gage) {
		if (mCanvas = LevelUpManager::Get()->GetCanvas(); mCanvas) {
			auto obj = Instantiate(gage);
			obj->GetTransform()->SetParent(mCanvas->GetTransform());
			obj->SetActive(true);

			if (mProgGage = obj->GetComponent<Gage>(); mProgGage) {
				mProgGage->SetEnabled(false);
			}

			if (auto wsUI = obj->GetComponent<WorldSpaceUI>(); wsUI) {
				wsUI->TargetTransform = GetTransform();
			}
		}
	}

	// 없으면 쓰지마
	if (!mProgGage.IsValid()) {
		std::cout << "BuildingPoint::Prograss Gage Not Spawned!!" << std::endl;
	}
}

void MMMEngine::BuildingPoint::Update()
{
	if (!player || !mPlayer) return;

	if (mPlayer->buildchance)
	{
		GetComponent<MeshRenderer>()->SetEnabled(true);

		if (canBuild) {
			if (mProgGage) {
				mProgGage->SetEnabled(true);

				float maxTime = mPlayer->GetBuildTime();
				float currTime = mPlayer->GetBuildElipsed();
				mProgGage->SetValue(currTime / maxTime);
			}
		}
		else {
			if (mProgGage)
				mProgGage->SetEnabled(false);
		}
	}
	else
	{
		GetComponent<MeshRenderer>()->SetEnabled(false);
		if (mProgGage)
			mProgGage->SetEnabled(false);
	}
	//CheckPlayer();
}

void MMMEngine::BuildingPoint::OnTriggerEnter(MMMEngine::TriggerInfo info)
{
	if (player.IsValid()) {
		if (auto pScript = player->GetComponent<Player>(); pScript.IsValid()) {
			pScript->misInited = false;
			pScript->ElipsedClear();
			pScript->SetBuildable(true);
		}

		if(!alreadybuilt)
			canBuild = true;
		else
			canBuild = false;
	}
}

void MMMEngine::BuildingPoint::OnTriggerExit(MMMEngine::TriggerInfo info)
{
	if (player.IsValid()) {
		if (auto pScript = player->GetComponent<Player>(); pScript.IsValid()) {
			pScript->misInited = false;
			pScript->ElipsedClear();
			pScript->SetBuildable(false);
		}

		canBuild = false;
	}
}

void MMMEngine::BuildingPoint::OnDisable()
{
	if (player.IsValid()) {
		if (auto pScript = player->GetComponent<Player>(); pScript.IsValid()) {
			pScript->misInited = false;
			pScript->ElipsedClear();

			if (pScript->isBuildable)
				pScript->SetBuildable(false);
		}
	}

	if (mProgGage)
		mProgGage->SetEnabled(false);

	canBuild = false;
}

//void MMMEngine::BuildingPoint::CheckPlayer()
//{
//	auto pos = GetTransform()->GetWorldPosition();
//	float bestD2 = checkdist * checkdist;
//	float dx = pos.x - playerpos.x;
//	float dz = pos.z - playerpos.z;
//	float d2 = dx * dx + dz * dz;
//
//	if (d2 < bestD2 && !alreadybuilt)
//	{
//		canBuild = true;
//	}
//	else
//	{
//		canBuild = false;
//	}
//}