#include "Export.h"
#include "ScriptBehaviour.h"
#include "LevelUpManager.h"
#include "CastleLevelController.h"

MMMEngine::ObjPtr<MMMEngine::LevelUpManager> MMMEngine::LevelUpManager::instance;

void MMMEngine::LevelUpManager::Awake()
{
	if (LevelUpManager::Get())
		Destroy(SelfPtr(this));
	if (!instance)
		instance = SelfPtr(this);

	//Instantiate(ResourceManager::Get().Load<Prefab>(L"Assets/Prefab/TestPrefab.prefab"));

	if (!mCanvas.IsValid()) {
		std::cout << "LevelManager::Canvas Not Found!!" << std::endl;
		GetGameObject()->SetActive(false);
	}

	if (!mCastleController.IsValid()) {
		std::cout << "LevelManager::Castle Not Found!!" << std::endl;
		GetGameObject()->SetActive(false);
	}

	/*if (!mPlayer.IsValid()) {
		std::cout << "LevelManager::Player Not Found!!" << std::endl;
		GetGameObject()->SetActive(false);
	}*/
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
