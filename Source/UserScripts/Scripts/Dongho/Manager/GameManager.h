#pragma once
#include "ScriptBehaviour.h"
#include "MMMApplication.h"
#include "UserScriptsCommon.h"
#include "rttr/type"
#include "StaticMesh.h"
#include "SimpleMath.h"
#include <array>

namespace MMMEngine { class TimerUI; }

using DirectX::SimpleMath::Vector3;
namespace MMMEngine {
	class Player;
	class Castle;
	class USERSCRIPTS GameManager : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
			RTTR_REGISTRATION_FRIEND

			USCRIPT_PROPERTY()
			ObjPtr<TimerUI> mTimerUI;
	public:
		GameManager()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
		USCRIPT_MESSAGE()
		void Awake();
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
		USCRIPT_PROPERTY()
		int mMaxWave = 10;
		int levelsum = 0;

		ObjPtr<GameObject> player;
		ObjPtr<GameObject> castle;
		ObjPtr<Player> playercomp;
		ObjPtr<Castle> castlecomp;



		static ObjPtr<GameManager> instance;
	};
}
