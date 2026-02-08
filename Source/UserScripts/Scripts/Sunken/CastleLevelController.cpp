#include "Export.h"
#include "ScriptBehaviour.h"
#include "CastleLevelController.h"

#include "../Dongho/Castle/Castle.h"
#include "LevelUpManager.h"

#include "InputManager.h"

void MMMEngine::CastleLevelController::UpLevel()
{
	mCurrLevel++;
	mCastle->exp = 0;
	mUpPending++;

	if (mCurrLevel >= LevelUpManager::Get()->GetMaxLevel(EXPTYPE::EXP_CASTLE))
		mReqExp = 0;
	else
		mReqExp = LevelUpManager::Get()->GetExpPoint(EXPTYPE::EXP_CASTLE, mCurrLevel);

	mHpGage->GetGameObject()->SetActive(false);
	mExpGage->GetGameObject()->SetActive(false);
}

void MMMEngine::CastleLevelController::SetUITrans(ObjPtr<RectTransform> _rectTrans, Vector2& _offset, Vector2& _mPadding)
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

	_rectTrans->SetAnchoredPosition(((canvasPos - achPos) + (_offset / distFactor)) + paddingFactor / 2.0f );

	auto defaultSize = Vector3{ 1.0f, 1.0f, 1.0f };
	_rectTrans->SetWorldScale(defaultSize / distFactor);
}

void MMMEngine::CastleLevelController::SetLVManager(int _upIndex)
{
	switch (_upIndex)
	{
	case 0:
		LevelUpManager::Get()->AddCastleLevel();
		break;
	case 1:
		LevelUpManager::Get()->AddScoopLevel();
		break;
	default:
		break;
	}
}

void MMMEngine::CastleLevelController::UpdateGuage()
{
	mCastleIcon->GetGameObject()->SetActive(false);
	mScoopIcon->GetGameObject()->SetActive(false);

	mHpGage->GetGameObject()->SetActive(true);
	mExpGage->GetGameObject()->SetActive(true);

	auto expRect = mExpGage->GetRectTransform();
	auto hpRect = mHpGage->GetRectTransform();

	SetUITrans(expRect, mGagePosOffset ,mPadding);
	SetUITrans(hpRect, mGagePosOffset, -mPadding);

	auto maxHP = mCastle->maxHP;
	auto currHP = mCastle->prevHP;

	float hpFactor = (float)currHP / (float)maxHP;
	mHpGage->SetValue(hpFactor);

	float expFactor = 0.0f;
	if (mReqExp == 0)
		expFactor = 1.0f;
	else
		expFactor = (float)mCastle->exp / (float)mReqExp;

	mExpGage->SetValue(expFactor);
}

void MMMEngine::CastleLevelController::UpdateReadyIcon()
{
	if (mReadyIcon)
		mReadyIcon->GetGameObject()->SetActive(true);
	else
		return;

	auto readyTrans = mReadyIcon->GetRectTransform();

	SetUITrans(readyTrans, mReadyPosOffset, Vector2{0.0f, 0.0f});
}

void MMMEngine::CastleLevelController::UpdateSelectIcon()
{
	if (mCastleIcon)
		mCastleIcon->GetGameObject()->SetActive(true);

	if (mScoopIcon)
		mScoopIcon->GetGameObject()->SetActive(true);

	auto castleRect = mCastleIcon->GetRectTransform();
	auto scoopRect = mScoopIcon->GetRectTransform();

	SetUITrans(castleRect, mSelectPosOffset, mSelectPadding);
	SetUITrans(scoopRect, mSelectPosOffset, -mSelectPadding);

	if (mSelectIndex == 0) {
		castleRect->SetWorldScale(1.0f, 1.0f, 1.0f);
		scoopRect->SetWorldScale(0.75f, 0.75f, 0.75f);
	}
	else {
		scoopRect->SetWorldScale(1.0f, 1.0f, 1.0f);
		castleRect->SetWorldScale(0.75f, 0.75f, 0.75f);
	}

	// 인풋 받기
	auto& input = InputManager::Get();

	// TODO::효과음 재생 부분
	if(input.GetKeyDown(KeyCode::LeftArrow)) {
		if (mSelectIndex > 0)
			--mSelectIndex;
	}
	else if (input.GetKeyDown(KeyCode::RightArrow)) {
		if (mSelectIndex < 1)
			++mSelectIndex;
	}
	else if (input.GetKeyDown(KeyCode::Space)) {
		std::cout << "CastleLVController::Selected" << std::endl;
		SetLVManager(mSelectIndex);
		mUpPending--;
	}
}

void MMMEngine::CastleLevelController::Start()
{
	if (!LevelUpManager::Get().IsValid()) {
		std::cout << "CastleLVController::No LVManager!!!" << std::endl;
		Destroy(SelfPtr(this));
	}

	mCanvas = LevelUpManager::Get()->GetCanvas();
	mCastle = LevelUpManager::Get()->GetCastle();
	mReqExp = LevelUpManager::Get()->GetExpPoint(EXPTYPE::EXP_CASTLE, mCurrLevel);
	mHpGage = LevelUpManager::Get()->mHpGage;
	mExpGage = LevelUpManager::Get()->mExpGage;
	mReadyIcon = LevelUpManager::Get()->mReadyIcon;
	mCastleIcon = LevelUpManager::Get()->mCastleIcon;
	mScoopIcon = LevelUpManager::Get()->mScoopIcon;

	if (!mCastle.IsValid()) {
		std::cout << "CastleLVController::No Castle!!!" << std::endl;
		Destroy(SelfPtr(this));
	}

	if (!mExpGage.IsValid()) {
		std::cout << "CastleLVController::No EXPGage!!!" << std::endl;
		GetGameObject()->SetActive(false);
	}
	else
		mExpGage->GetGameObject()->SetActive(false);

	if (!mHpGage.IsValid()) {
		std::cout << "CastleLVController::No HpGage!!!" << std::endl;
		GetGameObject()->SetActive(false);
	}
	else
		mHpGage->GetGameObject()->SetActive(false);

	if (!mReadyIcon.IsValid()) {
		std::cout << "CastleLVController::No ReadyIcon!!!" << std::endl;
		GetGameObject()->SetActive(false);
	}
	else
		mReadyIcon->GetGameObject()->SetActive(false);

	if (!mCastleIcon.IsValid()) {
		std::cout << "CastleLVController::No CastleIcon!!!" << std::endl;
		GetGameObject()->SetActive(false);
	}
	else
		mCastleIcon->GetGameObject()->SetActive(false);

	if (!mScoopIcon.IsValid()) {
		std::cout << "CastleLVController::No mScoopIcon!!!" << std::endl;
		GetGameObject()->SetActive(false);
	}
	else
		mScoopIcon->GetGameObject()->SetActive(false);
}


void MMMEngine::CastleLevelController::Update()
{
	if (isActive) {
		if (mUpPending > 0) {
			UpdateSelectIcon();
		}
		else {
			UpdateGuage();
		}
	}
	else if (mUpPending > 0) {
		UpdateReadyIcon();
	}

	auto currEXP = mCastle->exp;
	if (currEXP >= mReqExp && mCurrLevel < LevelUpManager::Get()->GetMaxLevel(EXPTYPE::EXP_CASTLE))
		UpLevel();
}

void MMMEngine::CastleLevelController::OnTriggerEnter(MMMEngine::CollisionInfo info)
{
	if (info.other->GetTag() == "Player") {
		mReadyIcon->GetGameObject()->SetActive(false);
		isActive = true;
	}
}

void MMMEngine::CastleLevelController::OnTriggerExit(MMMEngine::CollisionInfo info)
{
	if (info.other->GetTag() == "Player") {
		mHpGage->GetGameObject()->SetActive(false);
		mExpGage->GetGameObject()->SetActive(false);
		mCastleIcon->GetGameObject()->SetActive(false);
		mScoopIcon->GetGameObject()->SetActive(false);
		isActive = false;
	}
}
