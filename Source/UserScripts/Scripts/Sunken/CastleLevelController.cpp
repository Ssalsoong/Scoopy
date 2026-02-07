#include "Export.h"
#include "ScriptBehaviour.h"
#include "CastleLevelController.h"

#include "../Dongho/Castle/Castle.h"
#include "LevelUpManager.h"

void MMMEngine::CastleLevelController::Start()
{
	if (!LevelUpManager::Get().IsValid()) {
		std::cout << "CastleLVController::No LVManager!!!" << std::endl;
		Destroy(SelfPtr(this));
	}

	if (!mCastle.IsValid()) {
		// TODO::Parent Object 찾기 시도
		std::cout << "CastleLVController::No Castle!!!" << std::endl;
		Destroy(SelfPtr(this));
	}

	if (!mExpGage.IsValid()) {
		std::cout << "CastleLVController::No EXPGage!!!" << std::endl;
	}

	if (!mHpGage.IsValid()) {
		std::cout << "CastleLVController::No HpGage!!!" << std::endl;
	}
}


void MMMEngine::CastleLevelController::Update()
{
	if (isActive) {
		auto tp = GetTransform();
		Vector2 canvasPos = LevelUpManager::Get()->GetCanvasPos(tp->GetWorldPosition());

		auto maxHP = mCastle->maxHP;
		auto currHP = mCastle->prevHP;

		float guageFactor = (float)maxHP / (float)currHP;

	}
}

void MMMEngine::CastleLevelController::OnTriggerEnter(MMMEngine::CollisionInfo info)
{
	if (info.other->GetTag() == "Player") {
		isActive = true;
	}
}

void MMMEngine::CastleLevelController::OnTriggerExit(MMMEngine::CollisionInfo info)
{
	if (info.other->GetTag() == "Player") {
		isActive = false;
	}
}
