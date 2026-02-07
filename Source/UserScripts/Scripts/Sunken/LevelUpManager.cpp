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
