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

	if (!LevelUpManager::Get()) {
		std::cout << "BuildingPoint::LevelUpManager Not Found!!" << std::endl;
		Destroy(GetGameObject());
	}

	// 게이지 셋팅
	if (auto gage = LevelUpManager::Get()->mUnivGagePrefab; gage) {
		if (mCanvas = LevelUpManager::Get()->GetCanvas(); mCanvas) {
			auto obj = Instantiate(gage);
			obj->GetTransform()->SetParent(mCanvas->GetTransform());
			mProgGage = obj->GetComponent<Gage>();

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
	if (!player) return;
	//playerpos = player->GetTransform()->GetWorldPosition();
	if (player->GetComponent<Player>()->buildchance)
	{
		GetComponent<MeshRenderer>()->SetEnabled(true);
	}
	else
	{
		GetComponent<MeshRenderer>()->SetEnabled(false);
	}
	//CheckPlayer();
}

void MMMEngine::BuildingPoint::OnTriggerEnter(MMMEngine::TriggerInfo info)
{
	if (player.IsValid()) {
		if (auto pScript = player->GetComponent<Player>(); pScript.IsValid()) {
			pScript->SetBuildable(true);
		}

		if(!alreadybuilt)
			canBuild = true;
		else
			canBuild = false;
	}

	if (mProgGage && mProgGage->GetGameObject())
		mProgGage->GetGameObject()->SetActive(true);
}

void MMMEngine::BuildingPoint::OnTriggerExit(MMMEngine::TriggerInfo info)
{
	if (player.IsValid()) {
		if (auto pScript = player->GetComponent<Player>(); pScript.IsValid()) {
			pScript->SetBuildable(false);
		}
		canBuild = false;
	}

	if (mProgGage && mProgGage->GetGameObject())
		mProgGage->GetGameObject()->SetActive(false);
}

void MMMEngine::BuildingPoint::OnDisable()
{
	if (player.IsValid()) {
		if (auto pScript = player->GetComponent<Player>(); pScript.IsValid()) {
			if (pScript->isBuildable)
				pScript->SetBuildable(false);
		}
	}

	if (mProgGage && mProgGage->GetGameObject())
		mProgGage->GetGameObject()->SetActive(false);

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