#include "Export.h"
#include "ScriptBehaviour.h"
#include "CastleLevelController.h"

#include "../Dongho/Castle/Castle.h"
#include "LevelUpManager.h"

void MMMEngine::CastleLevelController::UpLevel()
{
	mCurrLevel++;
	mCastle->exp = 0;

	//TODO::레벨업 UI모드로 전환
	isUpgrade = true;
}

void MMMEngine::CastleLevelController::UpdateGuage()
{
	auto tp = GetTransform();
	auto wPos = tp->GetWorldPosition();

	auto expRect = mExpGage->GetRectTransform();
	auto hpRect = mHpGage->GetRectTransform();

	Vector2 canvasSize = mCanvas->GetCanvasSize();
	Vector2 canvasPos = LevelUpManager::Get()->GetCanvasPos(wPos);
	Vector2 achPos = expRect->GetAnchorMin();
	float camDistanceSq = Vector3::Distance(Camera::GetMainCamera()->GetTransform()->GetWorldPosition(), wPos);
	achPos *= canvasSize;
	
	float distFactor = camDistanceSq * mDistanceFactor;
	auto paddingFactor = mPadding / distFactor;

	expRect->SetAnchoredPosition(((canvasPos - achPos) + (mUIPosOffset / distFactor)) + Vector2{ 0.0f, (paddingFactor / 2.0f) });
	hpRect->SetAnchoredPosition(((canvasPos - achPos) + (mUIPosOffset / distFactor)) + Vector2{ 0.0f, -(paddingFactor / 2.0f) });

	auto defaultSize = Vector3{ 1.0f, 1.0f, 1.0f };
	expRect->SetWorldScale(defaultSize / distFactor);
	hpRect->SetWorldScale(defaultSize / distFactor);

	auto maxHP = mCastle->maxHP;
	auto currHP = mCastle->prevHP;

	float hpFactor = (float)currHP / (float)maxHP;
	mHpGage->SetValue(hpFactor);

	float expFactor = (float)mCastle->exp / (float)mReqExp;
	mExpGage->SetValue(expFactor);

	auto currEXP = mCastle->exp;
	if (currEXP >= mReqExp)
		UpLevel();
}

void MMMEngine::CastleLevelController::Start()
{
	if (!LevelUpManager::Get().IsValid()) {
		std::cout << "CastleLVController::No LVManager!!!" << std::endl;
		Destroy(SelfPtr(this));
	}

	mCanvas = LevelUpManager::Get()->GetCanvas();
	mReqExp = LevelUpManager::Get()->GetExpPoint(EXPTYPE::EXP_CASTLE, mCurrLevel);

	if (!mCastle.IsValid()) {
		std::cout << "CastleLVController::No Castle!!!" << std::endl;
		Destroy(SelfPtr(this));
	}

	if (!mExpGage.IsValid()) {
		std::cout << "CastleLVController::No EXPGage!!!" << std::endl;
	}
	else
		mExpGage->GetGameObject()->SetActive(false);

	if (!mHpGage.IsValid()) {
		std::cout << "CastleLVController::No HpGage!!!" << std::endl;
	}
	else
		mHpGage->GetGameObject()->SetActive(false);
	
}


void MMMEngine::CastleLevelController::Update()
{
	if (isActive) {
		if (isUpgrade) {

		}
		else {
			UpdateGuage();
		}
	}
}

void MMMEngine::CastleLevelController::OnTriggerEnter(MMMEngine::CollisionInfo info)
{
	if (info.other->GetTag() == "Player") {
		mHpGage->GetGameObject()->SetActive(true);
		mExpGage->GetGameObject()->SetActive(true);
		isActive = true;
	}
}

void MMMEngine::CastleLevelController::OnTriggerExit(MMMEngine::CollisionInfo info)
{
	if (info.other->GetTag() == "Player") {
		mHpGage->GetGameObject()->SetActive(false);
		mExpGage->GetGameObject()->SetActive(false);
		isActive = false;
	}
}
