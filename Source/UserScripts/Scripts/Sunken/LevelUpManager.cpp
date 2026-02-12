#include "Export.h"
#include "ScriptBehaviour.h"
#include "LevelUpManager.h"
#include "CastleLevelController.h"
#include "../Dongho/Castle/Castle.h"
#include "../Dongho/Player/Player.h"
#include "BuildingLevelController.h"
#include "LevelUpBubble.h"

MMMEngine::ObjPtr<MMMEngine::LevelUpManager> MMMEngine::LevelUpManager::instance;

bool MMMEngine::LevelUpManager::ValidateReferences()
{
	bool ok = true;

	auto ReqObj = [&](auto& ref, const char* name) {
		if (!CheckValid(ref, name)) ok = false;
		};

	auto OptObj = [&](auto& ref, const char* name) {
		if (!ref) {
			std::cout << "LevelUpManager::Optional missing: " << name << std::endl;
		}
		};

	auto ReqRes = [&](auto& res, const char* name) {
		if (!res) {
			std::cerr << "LevelUpManager::Missing Resource: " << name << std::endl;
			ok = false;
		}
		};

	// 필수 루트
	ReqObj(mCanvas, "Canvas");
	ReqObj(mCastle, "Castle");
	ReqObj(mLevelUpBubble, "LevelUpBubble");

	// UI - 게이지/상태
	ReqObj(mExpGage, "ExpGage");
	OptObj(mHpGage, "HpGage");
	ReqObj(mReadyIcon, "ReadyIcon");

	// 캐슬 관련
	ReqObj(mCastleIcon, "CastleIcon");
	ReqObj(mScoopIcon, "ScoopIcon");
	ReqObj(mCastleRangeIcon, "CastleRangeIcon");
	ReqObj(mCastleDoubleIcon, "CastleDoubleIcon");
	ReqObj(mCastleExpIcon, "CastleExpIcon");
	ReqObj(mCastleShieldIcon, "CastleShieldIcon");
	ReqObj(mScoopRangeIcon, "ScoopRangeIcon");
	ReqObj(mScoopCritIcon, "ScoopCritIcon");
	ReqObj(mScoopReflectIcon, "ScoopReflectIcon");

	// 빌딩 관련
	ReqObj(mHPIcon, "HPIcon");
	ReqObj(mBuffIcon, "BuffIcon");
	ReqObj(mDeBuffIcon, "DeBuffIcon");
	ReqObj(mSnowIcon, "SnowIcon");

	// 버블/텍스트
	ReqObj(mSpeechBubbleIcon, "SpeechBubbleIcon");
	ReqObj(mHeadlineText, "HeadlineText");
	ReqObj(mScriptText, "ScriptText");

	// 프리팹
	ReqRes(mReadyPrefab, "ReadyPrefab");
	ReqRes(mCountPrefab, "CountPrefab");
	ReqRes(mHPGagePrefab, "GagePrefab");

	return ok;
}

void MMMEngine::LevelUpManager::Awake()
{
	if (LevelUpManager::Get())
		Destroy(SelfPtr(this));
	if (!instance)
		instance = SelfPtr(this);

	if (!ValidateReferences())
	{
		Destroy(GetGameObject());
		return;
	}
}

void MMMEngine::LevelUpManager::Start()
{
	// 빌딩 일반강화
	mHeadlineMap[EXPTYPE::EXP_BUILD].push_back(L"체력건물");
	mHeadlineMap[EXPTYPE::EXP_BUILD].push_back(L"버프건물");
	mHeadlineMap[EXPTYPE::EXP_BUILD].push_back(L"디버프건물");
	mHeadlineMap[EXPTYPE::EXP_BUILD].push_back(L"눈저장건물");

	mScriptMap[EXPTYPE::EXP_BUILD].push_back(L"건물의 체력이\n상승합니다.");
	mScriptMap[EXPTYPE::EXP_BUILD].push_back(L"근처에 있으면\n이속이 빨라집니다.");
	mScriptMap[EXPTYPE::EXP_BUILD].push_back(L"근처의 적의\n이속이 느려집니다.");
	mScriptMap[EXPTYPE::EXP_BUILD].push_back(L"일정 시간마다\n눈을 저장합니다.");

	// 성 일반강화
	mHeadlineMap[EXPTYPE::EXP_CASTLE].push_back(L"성 강화");
	mHeadlineMap[EXPTYPE::EXP_CASTLE].push_back(L"스쿱 강화");

	mScriptMap[EXPTYPE::EXP_CASTLE].push_back(L"공격력 +3");
	mScriptMap[EXPTYPE::EXP_CASTLE].push_back(L"눈덩이 최대 획득 수 +2\n스쿱 공격력 증가 + 1");
	
	// 스쿱 특수강화
	mHeadlineMap[EXPTYPE::EXP_SCOOPFIRST].push_back(L"카본 스쿱");
	mHeadlineMap[EXPTYPE::EXP_SCOOPFIRST].push_back(L"텅스텐 스쿱");

	mScriptMap[EXPTYPE::EXP_SCOOPFIRST].push_back(L"공격속도 2.5초 감소\n공격 범위 40도 증가");
	mScriptMap[EXPTYPE::EXP_SCOOPFIRST].push_back(L"50% 확률로 2배 데미지\n(몬스터 개별 적용)");
	
	mHeadlineMap[EXPTYPE::EXP_SCOOPSECOND].push_back(L"단단한 갑옷");
	mHeadlineMap[EXPTYPE::EXP_SCOOPSECOND].push_back(L"튼튼한 체력");

	mScriptMap[EXPTYPE::EXP_SCOOPSECOND].push_back(L"받은 데미지 50%반사");
	mScriptMap[EXPTYPE::EXP_SCOOPSECOND].push_back(L"최대 체력 50 증가");

	// 캐슬 특수강화
	mHeadlineMap[EXPTYPE::EXP_CASTLEFIRST].push_back(L"경험치 부스트");
	mHeadlineMap[EXPTYPE::EXP_CASTLEFIRST].push_back(L"조준경");

	mScriptMap[EXPTYPE::EXP_CASTLEFIRST].push_back(L"웨이브가 끝나면\n설치한 건물의 수 만큼\n경험치 10 획득");
	mScriptMap[EXPTYPE::EXP_CASTLEFIRST].push_back(L"성과 설치한 건물의\n사거리 2 증가");

	mHeadlineMap[EXPTYPE::EXP_CASTLESECOND].push_back(L"마법 보호막");
	mHeadlineMap[EXPTYPE::EXP_CASTLESECOND].push_back(L"두 배");

	mScriptMap[EXPTYPE::EXP_CASTLESECOND].push_back(L"피격 시 5초 동안 보호막 생성\n(웨이브 당 1번 발동) ");
	mScriptMap[EXPTYPE::EXP_CASTLESECOND].push_back(L"성 추가 공격 + 1\n(소모량 증가하지 않음)");
}

void MMMEngine::LevelUpManager::Update()
{
	// 캐슬레벨 확인
	if (mCastleLevel > mCastleFirst - 1) {
		// 첫번째 스페셜
		if (mCastleSpecial == 0) {
			if (!IsBubbling()) {
				mCastleSpecial++;
				std::vector<ObjPtr<Image>> icons{ mCastleExpIcon, mCastleRangeIcon };
				SetBubble(EXPTYPE::EXP_CASTLEFIRST, GetGameObject(), icons);
			}
		}

		// 두번째 스페셜 확인
		if (mCastleLevel > mCastleSecond - 1) {
			// 두번째 스페셜
			if (mCastleSpecial == 1) {
				if (!IsBubbling()) {
					mCastleSpecial++;
					std::vector<ObjPtr<Image>> icons{ mCastleShieldIcon, mCastleDoubleIcon };
					SetBubble(EXPTYPE::EXP_CASTLESECOND, GetGameObject(), icons);
				}
			}
		}
	}

	// 스쿱레벨 확인
	if (mScoopLevel > mScoopFirst - 1) {
		// 첫번째 스페셜
		if (mScoopSpecial == 0) {
			if (!IsBubbling()) {
				mScoopSpecial++;
				std::vector<ObjPtr<Image>> icons{ mScoopRangeIcon, mScoopCritIcon };
				SetBubble(EXPTYPE::EXP_SCOOPFIRST, GetGameObject(), icons);
			}
		}

		// 두번째 스페셜 확인
		if (mScoopLevel > mScoopSecond - 1) {
			// 두번째 스페셜
			if (mScoopSpecial == 1) {
				if (!IsBubbling()) {
					mScoopSpecial++;
					std::vector<ObjPtr<Image>> icons{ mScoopReflectIcon, mHPIcon };
					SetBubble(EXPTYPE::EXP_SCOOPSECOND, GetGameObject(), icons);
				}
			}
		}
	}
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

void MMMEngine::LevelUpManager::RemoveUIPuller()
{
	mPuller = nullptr;
}

MMMEngine::ObjPtr<MMMEngine::GameObject> MMMEngine::LevelUpManager::GetUIPuller()
{
	if (mPuller.IsValid())
		return mPuller;
	return nullptr;
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
	mLevelPendingObj.Reset();
	mLevelUpBubble->SetActive(false);
	mPendingType = EXPTYPE::EXP_END;
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
	case MMMEngine::EXP_SCOOPFIRST:
	{
		if (mPlayer.IsValid()) {
			mPlayer->Level5Apply(_selectionIdx + 1);
			return;
		}
		else {
			std::cout << "LevelUpManager::SetSelection::Player Not Found !!!" << std::endl;
		}
		return;
		break;
	}
	case MMMEngine::EXP_SCOOPSECOND:
	{
		if (mPlayer.IsValid()) {
			mPlayer->Level10Apply(_selectionIdx + 1);
			return;
		}
		else {
			std::cout << "LevelUpManager::SetSelection::Player Not Found !!!" << std::endl;
		}
		break;
	}
	case MMMEngine::EXP_CASTLEFIRST:
	{
		if (mCastle.IsValid()) {
			mCastle->Level5Apply(_selectionIdx + 1);
			return;
		}
		else {
			std::cout << "LevelUpManager::SetSelection::Castle Not Found !!!" << std::endl;
		}
		break;
	}
	case MMMEngine::EXP_CASTLESECOND:
	{
		if (mCastle.IsValid()) {
			mCastle->Level10Apply(_selectionIdx + 1);
			return;
		}
		else {
			std::cout << "LevelUpManager::SetSelection::Castle Not Found !!!" << std::endl;
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

MMMEngine::ObjPtr<MMMEngine::GameObject> MMMEngine::LevelUpManager::GetBubbleTarget()
{
	return mLevelPendingObj;
}

bool MMMEngine::LevelUpManager::IsBubbling()
{
	if (mPendingType == EXPTYPE::EXP_END && !mLevelUpBubble->IsAnimating())
		return false;
	
	return true;
}

const std::wstring MMMEngine::LevelUpManager::GetHeadline(int _idx)
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

const std::wstring MMMEngine::LevelUpManager::GetScripts(int _idx)
{
	auto it = mScriptMap.find(mPendingType);

	std::wstring levelScript;
	switch (mPendingType)
	{
	case MMMEngine::EXP_CASTLE:
	{
		levelScript = L"LEVEL : ";

		if (_idx == 0)
			levelScript += std::to_wstring(mCastleLevel + 1);
		else
			levelScript += std::to_wstring(mScoopLevel + 1);
		levelScript += L"\n\n";
		break;
	}
	case MMMEngine::EXP_END:
		break;
	default:
		levelScript = L"\n\n\n";
		break;
	}

	if (it != mScriptMap.end()) {
		if (_idx < it->second.size()) {
			return levelScript + it->second[_idx];
		}
	}

	std::cout << "LevelUpManager::No Bubble Script!!" << std::endl;
	return L"";
}
