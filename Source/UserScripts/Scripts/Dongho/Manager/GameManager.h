#pragma once
#include "ScriptBehaviour.h"
#include "MMMApplication.h"
#include "UserScriptsCommon.h"
#include "rttr/type"
#include "StaticMesh.h"

namespace MMMEngine {
	class USERSCRIPTS GameManager : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
		RTTR_REGISTRATION_FRIEND
	public:
		GameManager()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
		void Start();
		void Update();
		float settingfullTime = 30.0f;
		float NorenemySpawnDelay = 10.0f;
		float ArenemySpawnDelay = 15.0f;
		float settingTimer = 0.0f;
		float NorenemySpawnTimer = 0.0f;
		float ArenemySpawnTimer = 0.0f;
		bool GameOver = false;
		bool nowSetting = true;
		ObjPtr<GameObject> player;
		ObjPtr<GameObject> castle;
		ResPtr<StaticMesh> playermesh;
		ResPtr<StaticMesh> castlemesh;
		static ObjPtr<GameManager> instance;
	};
}
