#pragma once
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "rttr/type"
#include "StaticMesh.h"
#include "SimpleMath.h"
#include <array>

using DirectX::SimpleMath::Vector3;
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
		void SpawnNormalEnemy(const DirectX::SimpleMath::Vector3& pos);
		void SpawnArrowEnemy(const DirectX::SimpleMath::Vector3& pos);
		void SpawnThiefEnemy(const DirectX::SimpleMath::Vector3& pos);
		void EnemyDeath(ObjPtr<GameObject> obj);
		void WaveSetting(int wave);
		bool WaveSpawn(int wave);
		float spawnTimer = 0.0f;
		float spawnDelay = 0.5f;
		static constexpr int SpawnPointCount = 12;
		std::array<Vector3, SpawnPointCount> spawnPos =
		{
			Vector3{-14.5f,0.f,-20.5f},
			Vector3{ 0.f,  0.f,-20.5f},
			Vector3{14.5f, 0.f,-20.5f},
			Vector3{ 20.5f, 0.f, -14.5f },
			Vector3{ 20.5f, 0.f, 0.f},
			Vector3{ 20.5f, 0.f, 14.5f },
			Vector3{ 14.5f, 0.f, 20.5f },
			Vector3{ 0.f, 0.f, 20.5f },
			Vector3{ -14.5f, 0.f, 20.5f },
			Vector3{ -20.5f,  0.f, 14.5f },
			Vector3{ -20.5f,  0.f, 0.f},
			Vector3{ -20.5f,  0.f, -14.5f }
		};
		static constexpr int WaveCount = 10;
		std::array<std::array<int, SpawnPointCount>, WaveCount > waveTable =
		{
			std::array<int, SpawnPointCount>{0,5,0,0,0,0,0,0,0,0,0,0},
			std::array<int, SpawnPointCount>{0,0,0,0,0,0,0,0,0,0,10,0},
			std::array<int, SpawnPointCount>{0,10,0,0,10,0,0,0,0,0,0,0},
			std::array<int, SpawnPointCount>{0,0,0,0,0,0,0,0,0,0,10,0},
			std::array<int, SpawnPointCount>{0,0,0,0,0,0,10,10,0,0,0,0},
			std::array<int, SpawnPointCount>{0,0,0,0,0,15,0,0,0,0,10,0},
			std::array<int, SpawnPointCount>{10,15,10,0,0,0,0,0,0,0,0,0},
			std::array<int, SpawnPointCount>{0,0,0,10,10,0,0,0,0,0,10,10},
			std::array<int, SpawnPointCount>{0,10,0,0,15,0,10,15,0,0,0,0},
			std::array<int, SpawnPointCount>{0,10,0,0,10,0,0,20,10,0,20,0}
		};
		std::array<int, SpawnPointCount>leftCount{};
		int aliveCount = 0;
		static ObjPtr<EnemySpawner> instance;
		std::queue<ObjPtr<GameObject>> NormalEnemys;
		ResPtr<StaticMesh> normalenemymesh;
		std::queue<ObjPtr<GameObject>> ArrowEnemys;
		ResPtr<StaticMesh> arrowenemymesh;
		std::queue<ObjPtr<GameObject>> ThiefEnemys;
		ResPtr<StaticMesh> thiefenemymesh;
		DirectX::SimpleMath::Vector3 normalsize{ 1.f,1.f,1.f };
		DirectX::SimpleMath::Vector3 arrowsize{ 1.f,1.f,1.f };
		DirectX::SimpleMath::Vector3 thiefsize{ 1.f,1.f,1.f };
	};
}
