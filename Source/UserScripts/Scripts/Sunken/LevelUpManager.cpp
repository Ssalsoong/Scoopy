#include "Export.h"
#include "ScriptBehaviour.h"
#include "LevelUpManager.h"
#include "CastleLevelController.h"
#include "../Dongho/Castle/Castle.h"
#include "../Dongho/Player/Player.h"
#include "BuildingLevelController.h"
#include "LevelUpBubble.h"

MMMEngine::ObjPtr<MMMEngine::LevelUpManager> MMMEngine::LevelUpManager::instance;

void MMMEngine::LevelUpManager::Awake()
{
	if (LevelUpManager::Get())
		Destroy(SelfPtr(this));
	if (!instance)
		instance = SelfPtr(this);

	if (!CheckValid(mCanvas, "Canvas") ||
		!CheckValid(mCastle, "Castle") ||
		!CheckValid(mExpGage, "ExpGage") ||
		!CheckValid(mHpGage, "HpGage") ||
		!CheckValid(mReadyIcon, "ReadyIcon") ||
		!CheckValid(mCastleIcon, "CastleIcon") ||
		!CheckValid(mScoopIcon, "ScoopIcon") ||
		!CheckValid(mHPIcon, "HPIcon") ||
		!CheckValid(mBuffIcon, "BuffIcon") ||
		!CheckValid(mDeBuffIcon, "DeBuffIcon") ||
		!CheckValid(mSnowIcon, "SnowIcon") ||
		!CheckValid(mLevelUpBubble, "LevelUpBubble") ||
		!CheckValid(mSpeechBubbleIcon, "SpeechBubbleIcon") ||
		!CheckValid(mHeadlineText, "HeadlineText") || 
		!CheckValid(mScriptText, "ScriptText"))
	{
		Destroy(SelfPtr(this));
	}

	if (!mReadyPrefab || !mCountPrefab || !mGagePrefab)
	{
		std::cerr << "LevelManager::Prefab Not Found!!" << std::endl;
		Destroy(SelfPtr(this));
	}
}

void MMMEngine::LevelUpManager::Start()
{
	mHeadlineMap[EXPTYPE::EXP_BUILD].push_back(L"체력건물");
	mHeadlineMap[EXPTYPE::EXP_BUILD].push_back(L"버프건물");
	mHeadlineMap[EXPTYPE::EXP_BUILD].push_back(L"디버프건물");
	mHeadlineMap[EXPTYPE::EXP_BUILD].push_back(L"수급건물");

	mScriptMap[EXPTYPE::EXP_BUILD].push_back(L"건물의 체력이\n상승합니다.");
	mScriptMap[EXPTYPE::EXP_BUILD].push_back(L"근처에 있으면\n이속이 빨라집니다.");
	mScriptMap[EXPTYPE::EXP_BUILD].push_back(L"근처의 적의\n이속이 느려집니다.");
	mScriptMap[EXPTYPE::EXP_BUILD].push_back(L"일정 시간마다\n눈을 저장합니다.");

	mHeadlineMap[EXPTYPE::EXP_CASTLE].push_back(L"성 강화");
	mHeadlineMap[EXPTYPE::EXP_CASTLE].push_back(L"스쿱 강화");

	mScriptMap[EXPTYPE::EXP_CASTLE].push_back(L"공격력 +2");
	mScriptMap[EXPTYPE::EXP_CASTLE].push_back(L"눈덩이 최대 획득 수 +2\n스쿱 공격력 증가 + 1");
}

void MMMEngine::LevelUpManager::Update()
{
}

Vector2 MMMEngine::LevelUpManager::GetCanvasPos(const Vector3& _worldPos)
{
	auto cam = Camera::GetMainCamera();
	Vector2 screenPos = cam->WorldToScreenPoint(_worldPos);
	Vector2 canvasPos = mCanvas->ScreenToCanvas(screenPos);

	return canvasPos;
}

MMMEngine::ObjPtr<MMMEngine::Castle> MMMEngine::LevelUpManager::GetCastle()
{
	return mCastle;
}

void MMMEngine::LevelUpManager::AddCastleLevel(int _val)
{
	if (_val > 0) {
		mCastleLevel += _val;
		for (int i = 0; i < _val; ++i) {
			mCastle->LevelUp();
		}
	}
	else if (_val < 0) {
		mCastleLevel += _val;
		for (int i = 0; i < -_val; ++i) {
			//mCastle->LevelDown(); // LevelDown 구현
		}
	}
}

void MMMEngine::LevelUpManager::AddScoopLevel(int _val)
{
	if (_val > 0) {
		mScoopLevel += _val;
		for (int i = 0; i < _val; ++i) {
			mPlayer->LevelUp();
		}
	}
	else if (_val < 0) {
		mScoopLevel += _val;
		for (int i = 0; i < -_val; ++i) {
			//mPlayer->LevelDown(); // LevelDown 구현
		}
	}
}

int MMMEngine::LevelUpManager::GetExpPoint(EXPTYPE _type, int _level)
{
	if (_level < 0) {
		std::cout << "LevelUpManager::GetExp::Wrong Index Number!!" << std::endl;
		return -1;
	}

	switch (_type)
	{
	case MMMEngine::EXP_CASTLE:
	{
		if (_level < (int)mCastleExp.size())
			return mCastleExp[_level];
		return 0;
		break;
	}
	case MMMEngine::EXP_BUILD:
	{
		if (_level < (int)mBuildingExp.size())
			return mBuildingExp[_level];
		return 0;
		break;
	}
	case MMMEngine::EXP_END:
		break;
	default:
		break;
	}

	std::cout << "LevelUpManager::GetExp::Wrong Index or Type!!" << std::endl;
	return -1;
}

int MMMEngine::LevelUpManager::GetMaxLevel(EXPTYPE _type)
{
	switch (_type)
	{
	case MMMEngine::EXP_CASTLE:
	{
		return mCastleExp.size();
		break;
	}
	case MMMEngine::EXP_BUILD:
	{
		return mBuildingExp.size();
		break;
	}
	case MMMEngine::EXP_END:
		break;
	default:
		break;
	}

	std::cout << "LevelUpManager::GetExp::Wrong Type!!" << std::endl;
	return 0;
}

void MMMEngine::LevelUpManager::SetBubble(EXPTYPE _type, ObjPtr<GameObject> _target, std::vector<ObjPtr<Image>>& _icons)
{
	mPendingType = _type;
	mLevelPendingObj = _target;

	mLevelUpBubble->SetActive(true);
	mLevelUpBubble->SetIcons(_icons);

	// SetHeadline과 SetScript는 버블 안에서 해결
}

void MMMEngine::LevelUpManager::RemoveBubble()
{
	mPendingType = EXPTYPE::EXP_END;
	mLevelPendingObj.Reset();
	mLevelUpBubble->SetActive(false);
}

void MMMEngine::LevelUpManager::SetSelection(int _selectionIdx)
{
	switch (mPendingType)
	{
	case MMMEngine::EXP_CASTLE:
	{
		auto controller = mLevelPendingObj->GetComponent<CastleLevelController>();
		
		if (controller) {
			controller->SetLevelSelection(_selectionIdx);
			return;
		}
		break;
	}
	case MMMEngine::EXP_BUILD:
	{
		auto controller = mLevelPendingObj->GetComponent<BuildingLevelController>();
		if (controller) {
			controller->SetLevelSelection(_selectionIdx);
			return;
		}
		break;
	}
	case MMMEngine::EXP_END:
		[[fallthrough]];
	default:
		break;
	}

	std::cout << "LevelUpManager::SetSelection::Wrong Type!!" << std::endl;
	return;
}

const std::wstring& MMMEngine::LevelUpManager::GetHeadline(int _idx)
{
	auto it = mHeadlineMap.find(mPendingType);

	if (it != mHeadlineMap.end()) {
		if (_idx < it->second.size()) {
			return it->second[_idx];
		}
	}

	std::cout << "LevelUpManager::No Bubble Headline!!" << std::endl;
	return L"";
}

const std::wstring& MMMEngine::LevelUpManager::GetScripts(int _idx)
{
	auto it = mScriptMap.find(mPendingType);

	if (it != mScriptMap.end()) {
		if (_idx < it->second.size()) {
			return it->second[_idx];
		}
	}

	std::cout << "LevelUpManager::No Bubble Script!!" << std::endl;
	return L"";
}
