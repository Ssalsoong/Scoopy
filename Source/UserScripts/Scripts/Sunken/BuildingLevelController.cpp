#include "Export.h"
#include "ScriptBehaviour.h"
#include "BuildingLevelController.h"
#include "LevelUpManager.h"

#include "../Dongho/Battlestats.h"
#include "../Dongho/Manager/BuildingManager.h"
#include "../Dongho/Building/HPBuilding.h"
#include "../Dongho/Building/BuffBuilding.h"
#include "../Dongho/Building/DebuffBuilding.h"
#include "../Dongho/Building/SnowBuilding.h"
#include "InputManager.h"

void MMMEngine::BuildingLevelController::SetLevelSelection(int _idx)
{
	SetLVManager(_idx);
	mUpPending--;
	isReady = false;
}

void MMMEngine::BuildingLevelController::Start()
{
	if (!LevelUpManager::Get()) {
		std::cout << "BuildingLVController::LevelUpManager Not Exist !!" << std::endl;
		Destroy(GetGameObject());
	}

	if (!BuildingManager::instance) {
		std::cout << "BuildingLVController::BuildingManager Not Exist !!" << std::endl;
		Destroy(GetGameObject());
	}

	if (!mBuilding) {
		mBuilding = GetTransform()->GetParent()->GetGameObject()->GetComponent<Building>();
		if (!mBuilding) {
			std::cout << "BuildingLVController::Building Not Exist !!" << std::endl;
			Destroy(GetGameObject());
		}
	}

	if (!mBattleStat) {
		mBattleStat = GetTransform()->GetParent()->GetGameObject()->GetComponent<Battlestats>();
		if (!mBattleStat) {
			std::cout << "BuildingLVController::BattleStat Not Exist !!" << std::endl;
			Destroy(GetGameObject());
		}
	}

	if (!mPlayer) {
		mPlayer = GameObject::FindWithTag("Player");
		if (!mPlayer) {
			std::cout << "BuildingLVController::Player Not Exist !!" << std::endl;
			Destroy(GetGameObject());
		}
	}

	mCanvas = LevelUpManager::Get()->GetCanvas();
	mReqExp = LevelUpManager::Get()->GetExpPoint(EXPTYPE::EXP_BUILD, mCurrLevel);
	mHpGage = LevelUpManager::Get()->mHpGage;
	mExpGage = LevelUpManager::Get()->mExpGage;
	mHPIcon = LevelUpManager::Get()->mHPIcon;
	mBuffIcon = LevelUpManager::Get()->mBuffIcon;
	mDeBuffIcon = LevelUpManager::Get()->mDeBuffIcon;
	mSnowIcon = LevelUpManager::Get()->mSnowIcon;
	mReadyIcon = Instantiate(LevelUpManager::Get()->mReadyPrefab)->GetComponent<Image>();
	mCountIcon = Instantiate(LevelUpManager::Get()->mCountPrefab)->GetComponent<Image>();

	mCountIcon->GetTransform()->SetParent(mCanvas->GetTransform());
	mCountIcon->GetGameObject()->SetActive(false);

	if (!mCanvas.IsValid()) {
		std::cout << "BuildingLVController::No Canvas!!!" << std::endl;
		Destroy(GetGameObject());
	}

	if (!mExpGage.IsValid()) {
		std::cout << "BuildingLVController::No EXPGage!!!" << std::endl;
		GetGameObject()->SetActive(false);
	}

	if (!mHpGage.IsValid()) {
		std::cout << "BuildingLVController::No HpGage!!!" << std::endl;
		GetGameObject()->SetActive(false);
	}

	if (!mReadyIcon.IsValid()) {
		std::cout << "BuildingLVController::No ReadyImage!!!" << std::endl;
		GetGameObject()->SetActive(false);
	}

	if (GetComponent<HPBuilding>())
		mType = BUILDTYPE::HEALTH;
	else if (GetComponent<BuffBuilding>())
		mType = BUILDTYPE::BUFF;
	else if (GetComponent<DebuffBuilding>())
		mType = BUILDTYPE::DEBUFF;
	else if (GetComponent<SnowBuilding>())
		mType = BUILDTYPE::SNOW;
	else
		mType = BUILDTYPE::NORMAL;

	// 생성한 오브젝트들 canvas 상속
	mReadyIcon->GetTransform()->SetParent(mCanvas->GetTransform(), false);
}

void MMMEngine::BuildingLevelController::LevelUp() {
	mUpPending--;
	mCurrLevel++;
	mBuilding->exp = 0;
	mPrevActive = false;

	if (mCurrLevel >= LevelUpManager::Get()->GetMaxLevel(EXPTYPE::EXP_BUILD))
		mReqExp = 0;
	else
		mReqExp = LevelUpManager::Get()->GetExpPoint(EXPTYPE::EXP_BUILD, mCurrLevel);


	//TODO::BuildingManager한테 LevelUp 신호 보내기
	auto obj = mBuilding->GetGameObject();
	switch (mType)
	{
	case MMMEngine::BuildingLevelController::NORMAL:
		isReady = true;
		break;
	case MMMEngine::BuildingLevelController::HEALTH:
		BuildingManager::instance->LevelUpHP(obj);
		break;
	case MMMEngine::BuildingLevelController::BUFF:
		BuildingManager::instance->LevelUpBuff(obj);
		break;
	case MMMEngine::BuildingLevelController::DEBUFF:
		BuildingManager::instance->LevelUpDeBuff(obj);
		break;
	case MMMEngine::BuildingLevelController::SNOW:
		BuildingManager::instance->LevelUpSnow(obj);
		break;
	case MMMEngine::BuildingLevelController::END:
		[[fallthrough]];
	default:
		std::cout << "BuildLVController::Wrong Type !!" << std::endl;
		break;
	}

	mReqExp = LevelUpManager::Get()->GetExpPoint(EXPTYPE::EXP_BUILD, mCurrLevel);
}

void MMMEngine::BuildingLevelController::UpdateReadyIcon()
{
	mReadyIcon->GetGameObject()->SetActive(true);
	auto readyTrans = mReadyIcon->GetRectTransform();

	SetUITrans(readyTrans, mReadyPosOffset, Vector2{ 0.0f, 0.0f });
}

//void MMMEngine::BuildingLevelController::UpdateSelectIcon()
//{
//	SetActiveIcon();
//
//	std::vector<ObjPtr<RectTransform>> icons = { 
//		mHPIcon->GetRectTransform(), 
//		mBuffIcon->GetRectTransform(),
//		mDeBuffIcon->GetRectTransform(),
//		mSnowIcon->GetRectTransform() };
//	float w = mHPIcon->GetRectTransform()->GetWidth();
//	Vector2 p = mSelectPadding;
//	int N = (int)icons.size();
//
//	float totalWidth = N * w + (N - 1) * p.x;
//	float startX = -totalWidth / 2.0f + w / 2.0f;
//
//	for (int i = 0; i < N; ++i) {
//		Vector2 pos = mSelectPosOffset + Vector2{ startX + i * (w + p.x), i * p.y };
//		SetUITrans(icons[i], mSelectPosOffset, pos);
//	}
//
//	// 인풋 받기
//	auto& input = InputManager::Get();
//	
//	Vector3 selectSize(1.0f, 1.0f, 1.0f);
//	Vector3 deSelectSize(0.8f, 0.8f, 0.8f);
//	// 이미지 크기 조절하기
//	switch (mSelectIndex)
//	{
//	case 0:
//	{
//		icons[0]->SetWorldScale(selectSize);
//		icons[1]->SetWorldScale(deSelectSize);
//		icons[2]->SetWorldScale(deSelectSize);
//		icons[3]->SetWorldScale(deSelectSize);
//		break;
//	}
//	case 1:
//	{
//		icons[0]->SetWorldScale(deSelectSize);
//		icons[1]->SetWorldScale(selectSize);
//		icons[2]->SetWorldScale(deSelectSize);
//		icons[3]->SetWorldScale(deSelectSize);
//		break;
//	}
//	case 2:
//	{
//		icons[0]->SetWorldScale(deSelectSize);
//		icons[1]->SetWorldScale(deSelectSize);
//		icons[2]->SetWorldScale(selectSize);
//		icons[3]->SetWorldScale(deSelectSize);
//		break;
//	}
//	case 3:
//	{
//		icons[0]->SetWorldScale(deSelectSize);
//		icons[1]->SetWorldScale(deSelectSize);
//		icons[2]->SetWorldScale(deSelectSize);
//		icons[3]->SetWorldScale(selectSize);
//		break;
//	}
//	default:
//		break;
//	}
//
//	// 컨트롤 받기
//	ControlSelection();
//}

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

	_rectTrans->SetAnchoredPosition(((canvasPos - achPos) + (_offset / distFactor)) + paddingFactor);

	auto defaultSize = Vector3{ 1.0f, 1.0f, 1.0f };
	_rectTrans->SetWorldScale(defaultSize / distFactor);
}

//void MMMEngine::BuildingLevelController::ControlSelection()
//{
//	// TODO::효과음 재생 부분
//	auto& input = InputManager::Get();
//
//	if (input.GetKeyDown(KeyCode::LeftArrow)) {
//		if (mSelectIndex > 0)
//			--mSelectIndex;
//	}
//	else if (input.GetKeyDown(KeyCode::RightArrow)) {
//		if (mSelectIndex < 3)
//			++mSelectIndex;
//	}
//	else if (input.GetKeyDown(KeyCode::Space)) {
//		//std::cout << "BuildingLVController::Selected" << std::endl;
//		SetLVManager(mSelectIndex);
//		isReady = false;
//	}
//}

void MMMEngine::BuildingLevelController::SetLVManager(int _idx)
{
	auto obj = mBuilding->GetGameObject();

	switch (_idx)
	{
	case 0:
		mType = BUILDTYPE::HEALTH;
		BuildingManager::instance->LevelUpHP(obj);
		break;
	case 1:
		mType = BUILDTYPE::BUFF;
		BuildingManager::instance->LevelUpBuff(obj);
		break;
	case 2:
		mType = BUILDTYPE::DEBUFF;
		BuildingManager::instance->LevelUpDeBuff(obj);
		break;
	case 3:
		mType = BUILDTYPE::SNOW;
		BuildingManager::instance->LevelUpSnow(obj);
		break;
	default:
		std::cout << "SetLVManager::Wrong LevelUp Index !!!" << std::endl;
		break;
	}

	mReadyIcon->GetGameObject()->SetActive(false);
	mHPIcon->GetGameObject()->SetActive(false);
	mBuffIcon->GetGameObject()->SetActive(false);
	mDeBuffIcon->GetGameObject()->SetActive(false);
	mSnowIcon->GetGameObject()->SetActive(false);
}

void MMMEngine::BuildingLevelController::SetActiveIcon()
{
	if (isReady && isActive) {
		mReadyIcon->GetGameObject()->SetActive(false);
		mHPIcon->GetGameObject()->SetActive(true);
		mBuffIcon->GetGameObject()->SetActive(true);
		mDeBuffIcon->GetGameObject()->SetActive(true);
		mSnowIcon->GetGameObject()->SetActive(true);
	}
	else {
		mReadyIcon->GetGameObject()->SetActive(true);
		mHPIcon->GetGameObject()->SetActive(false);
		mBuffIcon->GetGameObject()->SetActive(false);
		mDeBuffIcon->GetGameObject()->SetActive(false);
		mSnowIcon->GetGameObject()->SetActive(false);
	}

}

void MMMEngine::BuildingLevelController::UpdateGuage()
{
	auto expRect = mExpGage->GetRectTransform();
	auto hpRect = mHpGage->GetRectTransform();
	auto countRect = mCountIcon->GetRectTransform();

	SetUITrans(expRect, mGagePosOffset, mPadding);
	SetUITrans(hpRect, mGagePosOffset, -mPadding);
	SetUITrans(countRect, mGagePosOffset, mCountPosOffset);

	auto maxHP = mBuilding->maxHP;
	auto currHP = mBattleStat->HP;

	float hpFactor = (float)currHP / (float)maxHP;
	mHpGage->SetValue(hpFactor);

	float expFactor = 0.0f;
	if (mReqExp == 0)
		expFactor = 1.0f;
	else
		expFactor = (float)mBuilding->exp / (float)mReqExp;

	// 현재 눈 갯수 출력
	auto text = mCountIcon->GetTransform()->GetChild(0)->GetComponent<Text>();
	int snowCount = mBuilding->point;
	text->SetText(std::to_wstring(snowCount));

	mExpGage->SetValue(expFactor);
}

void MMMEngine::BuildingLevelController::Update()
{
	if (isActive)
		UpdateGuage();
	else if(isReady)
		UpdateReadyIcon();

	/*if (isReady && !isActive) {
		UpdateReadyIcon();
	}
	else if (isReady && isActive) {
		UpdateSelectIcon();
	}*/

	if (isReady) {
		if (mPrevActive != isActive) {
			mPrevActive = isActive;
			if (isActive) {
				mReadyIcon->GetGameObject()->SetActive(false);
				std::vector<ObjPtr<Image>> icons{ mHPIcon , mBuffIcon, mDeBuffIcon, mSnowIcon };
				auto object = GetGameObject();
				LevelUpManager::Get()->SetBubble(EXP_BUILD, object, icons);
			}
			else {
				LevelUpManager::Get()->RemoveBubble();
			}
		}
	}

	auto currEXP = mBuilding->exp;
	if (currEXP >= mReqExp && mCurrLevel < LevelUpManager::Get()->GetMaxLevel(EXPTYPE::EXP_BUILD))
		mUpPending++;

	while (!isReady && mUpPending > 0)
		LevelUp();
}

void MMMEngine::BuildingLevelController::OnTriggerEnter(MMMEngine::TriggerInfo info)
{
	if (info.other->GetTag() == "Player") {
		mHpGage->GetGameObject()->SetActive(true);
		mExpGage->GetGameObject()->SetActive(true);
		mCountIcon->GetGameObject()->SetActive(true);
		isActive = true;
	}
}

void MMMEngine::BuildingLevelController::OnTriggerExit(MMMEngine::TriggerInfo info)
{
	if (info.other->GetTag() == "Player") {
		mHpGage->GetGameObject()->SetActive(false);
		mExpGage->GetGameObject()->SetActive(false);
		mCountIcon->GetGameObject()->SetActive(false);
		isActive = false;
	}
}
