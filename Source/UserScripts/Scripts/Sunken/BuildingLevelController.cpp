#include "Export.h"
#include "ScriptBehaviour.h"
#include "BuildingLevelController.h"
#include "LevelUpManager.h"

#include "../Dongho/Battlestats.h"

void MMMEngine::BuildingLevelController::Start()
{
	if (!LevelUpManager::Get()) {
		std::cout << "BuildingLVController::LevelUpManager Not Exist !!" << std::endl;
		Destroy(SelfPtr(this));
	}

	if (!mBuilding) {
		std::cout << "BuildingLVController::Building Not Exist !!" << std::endl;
		Destroy(SelfPtr(this));
	}

	if (!mBattleStat) {
		std::cout << "BuildingLVController::BattleStat Not Exist !!" << std::endl;
		Destroy(SelfPtr(this));
	}

	mCanvas = LevelUpManager::Get()->GetCanvas();
	mReqExp = LevelUpManager::Get()->GetExpPoint(EXPTYPE::EXP_BUILD, mCurrLevel);
	mHpGage = LevelUpManager::Get()->mHpGage;
	mExpGage = LevelUpManager::Get()->mExpGage;

	if (!mCanvas.IsValid()) {
		std::cout << "BuildingLVController::No Canvas!!!" << std::endl;
		Destroy(SelfPtr(this));
	}

	if (!mExpGage.IsValid()) {
		std::cout << "BuildingLVController::No EXPGage!!!" << std::endl;
		GetGameObject()->SetActive(false);
	}
	else
		mExpGage->GetGameObject()->SetActive(false);

	if (!mHpGage.IsValid()) {
		std::cout << "BuildingLVController::No HpGage!!!" << std::endl;
		GetGameObject()->SetActive(false);
	}
	else
		mHpGage->GetGameObject()->SetActive(false);
}

void MMMEngine::BuildingLevelController::LevelUp() {
	mCurrLevel++;
	mBuilding->exp = 0;

	if (mCurrLevel >= LevelUpManager::Get()->GetMaxLevel(EXPTYPE::EXP_BUILD))
		mReqExp = 0;
	else
		mReqExp = LevelUpManager::Get()->GetExpPoint(EXPTYPE::EXP_BUILD, mCurrLevel);

	//TODO::BuildingManager한테 LevelUp 신호 보내기
}

void MMMEngine::BuildingLevelController::SetUITrans(ObjPtr<RectTransform> _rectTrans, Vector2& _offset, Vector2& _mPadding)
{
	auto tp = GetTransform();
	auto wPos = tp->GetWorldPosition();

	Vector2 canvasSize = mCanvas->GetCanvasSize();
	Vector2 canvasPos = LevelUpManager::Get()->GetCanvasPos(wPos);
	Vector2 achPos = _rectTrans->GetAnchorMin();
	float camDistance = Vector3::Distance(Camera::GetMainCamera()->GetTransform()->GetWorldPosition(), wPos);
	achPos *= canvasSize;

	float distFactor = camDistance * mDistanceFactor;
	auto paddingFactor = _mPadding / distFactor;

	_rectTrans->SetAnchoredPosition(((canvasPos - achPos) + (_offset / distFactor)) + paddingFactor / 2.0f);

	auto defaultSize = Vector3{ 1.0f, 1.0f, 1.0f };
	_rectTrans->SetWorldScale(defaultSize / distFactor);
}

void MMMEngine::BuildingLevelController::UpdateGuage()
{
	auto expRect = mExpGage->GetRectTransform();
	auto hpRect = mHpGage->GetRectTransform();

	SetUITrans(expRect, mGagePosOffset, mPadding);
	SetUITrans(hpRect, mGagePosOffset, -mPadding);

	auto maxHP = mBuilding->maxHP;
	auto currHP = mBattleStat->HP;

	float hpFactor = (float)currHP / (float)maxHP;
	mHpGage->SetValue(hpFactor);

	float expFactor = 0.0f;
	if (mReqExp == 0)
		expFactor = 1.0f;
	else
		expFactor = (float)mBuilding->exp / (float)mReqExp;

	mExpGage->SetValue(expFactor);
}

void MMMEngine::BuildingLevelController::Update()
{
	if (isActive) {
		UpdateGuage();
	}

	auto currEXP = mBuilding->exp;
	if (currEXP >= mReqExp && mCurrLevel < LevelUpManager::Get()->GetMaxLevel(EXPTYPE::EXP_BUILD))
		LevelUp();
}

void MMMEngine::BuildingLevelController::OnTriggerEnter(MMMEngine::CollisionInfo info)
{
	if (info.other->GetTag() == "Player") {
		mHpGage->GetGameObject()->SetActive(true);
		mExpGage->GetGameObject()->SetActive(true);
		isActive = true;
	}
}

void MMMEngine::BuildingLevelController::OnTriggerExit(MMMEngine::CollisionInfo info)
{
	if (info.other->GetTag() == "Player") {
		mHpGage->GetGameObject()->SetActive(false);
		mExpGage->GetGameObject()->SetActive(false);
		isActive = false;
	}
}
