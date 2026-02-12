#pragma once
#include "ScriptBehaviour.h"
#include "MMMApplication.h"
#include "UserScriptsCommon.h"
#include "rttr/type"
#include "StaticMesh.h"
#include "SimpleMath.h"
#include <array>

namespace MMMEngine { class GameOverSequencer; }

namespace MMMEngine { class PauseUI; }

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
		
		float enemySpawnTimer = 0.0f;
		float settingTimer = 0.0f;

		bool GameWin = false;
		bool GameOver = false;
		bool mOverSet = false;

		bool nowSetting = true;
		bool isPausing = false;
		int wave = 1;
		int levelsum = 0;

		USCRIPT_PROPERTY()
			int mMaxWave = 10;

		USCRIPT_PROPERTY()
			float enemySpawnDelay = 0.5f;

		USCRIPT_PROPERTY()
			float settingfullTime = 30.0f;

		ObjPtr<GameObject> player;
		ObjPtr<GameObject> castle;
		ObjPtr<Player> playercomp;
		ObjPtr<Castle> castlecomp;
		ObjPtr<TimerUI> mTimerUI;
		ObjPtr<GameOverSequencer> mOverSequencer;

		USCRIPT_PROPERTY()
		ObjPtr<PauseUI> mPauseUI;

		static ObjPtr<GameManager> instance;
	};
}
