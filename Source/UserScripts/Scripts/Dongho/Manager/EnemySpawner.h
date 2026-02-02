#pragma once
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "rttr/type"
#include "StaticMesh.h"
#include "SimpleMath.h"

namespace MMMEngine {
	class USERSCRIPTS EnemySpawner : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
		RTTR_REGISTRATION_FRIEND
	public:
		EnemySpawner()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
		void Start();
		void Update();
		void SpawnNormalEnemy();
		void SpawnArrowEnemy();
		void SpawnThiefEnemy();
		void ReturnEnemy(ObjPtr<GameObject> obj);
		static ObjPtr<EnemySpawner> instance;
		std::queue<ObjPtr<GameObject>> NormalEnemys;
		ResPtr<StaticMesh> normalenemymesh;
		std::queue<ObjPtr<GameObject>> ArrowEnemys;
		ResPtr<StaticMesh> arrowenemymesh;
		std::queue<ObjPtr<GameObject>> ThiefEnemys;
		ResPtr<StaticMesh> thiefenemymesh;
		DirectX::SimpleMath::Vector3 normalsize{ 1.f,1.f,1.f };
		DirectX::SimpleMath::Vector3 normalspawnpos{ 50.f,0.f,50.f };
		DirectX::SimpleMath::Vector3 arrowsize{ 1.f,1.f,1.f };
		DirectX::SimpleMath::Vector3 arrowspawnpos{ -50.f,0.f,50.f };
		DirectX::SimpleMath::Vector3 thiefsize{ 1.f,1.f,1.f };
		DirectX::SimpleMath::Vector3 thiefspawnpos{ -50.f,0.f,-50.f };
	};
}
