#pragma once
#include "ScriptBehaviour.h"
#include "MMMApplication.h"
#include "UserScriptsCommon.h"
#include "rttr/type"
#include "StaticMesh.h"
#include "SimpleMath.h"
#include <array>

using DirectX::SimpleMath::Vector3;
namespace MMMEngine {
	class Player;
	class Castle;
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
		USCRIPT_MESSAGE()
		void Start();
		USCRIPT_MESSAGE()
		void Update();
		float settingfullTime = 30.0f;
		float enemySpawnDelay = 0.5f;
		float enemySpawnTimer = 0.0f;
		float settingTimer = 0.0f;

		bool GameWin = false;
		bool GameOver = false;
		bool nowSetting = true;
		int wave = 1;
		int levelsum = 0;

		ObjPtr<GameObject> player;
		ObjPtr<GameObject> castle;
		ObjPtr<Player> playercomp;
		ObjPtr<Castle> castlecomp;
		static ObjPtr<GameManager> instance;
	};
}
