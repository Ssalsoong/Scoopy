#include "Export.h"
#include "ScriptBehaviour.h"
#include "LevelUpManager.h"
#include "CastleLevelController.h"
#include "../Dongho/Castle/Castle.h"
#include "../Dongho/Player/Player.h"

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
		!CheckValid(mSnowIcon, "SnowIcon"))
	{
		Destroy(SelfPtr(this));
	}

	if (!mReadyPrefab || !mGagePrefab)
	{
		std::cerr << "LevelManager::Prefab Not Found!!" << std::endl;
		Destroy(SelfPtr(this));
	}
}

void MMMEngine::LevelUpManager::Start()
{
	
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
