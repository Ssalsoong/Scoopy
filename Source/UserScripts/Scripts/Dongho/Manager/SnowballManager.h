#pragma once
#include "ScriptBehaviour.h"
#include <vector>
#include <unordered_map>
#include <SimpleMath.h>
#include "rttr/type"
#include "UserScriptsCommon.h"
#include "StaticMesh.h"

namespace MMMEngine {
	class Player;
	class Castle;
	class Transform;
	class USERSCRIPTS SnowballManager : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
		RTTR_REGISTRATION_FRIEND
		void RemoveFromList(ObjPtr<GameObject> obj);
		void AssembleSnow();
		void SnowToCastle();
		void SnowToBuilding();
		struct ScoopState
		{
			bool active = false;
			float holdTime = 0.0f;
		};

		std::unordered_map<Player*, ScoopState> scoopStates;

	public:
		SnowballManager()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
		USCRIPT_MESSAGE()
		void Start();
		USCRIPT_MESSAGE()
		void Update();
		void OnScoopStart(Player& player);
		void OnScoopHold(Player& player);
		void OnScoopEnd(Player& player);
		DirectX::SimpleMath::Vector3 snowscale{0.1f, 0.1f, 0.1f};
		float snowSpawnDelay = 0.2f;
		std::vector<ObjPtr<GameObject>> Snows;
		ObjPtr<GameObject> castle;
		ObjPtr<Castle> castlecomp;
		ObjPtr<Transform> castletr;
		ResPtr<StaticMesh> snowmesh;
		DirectX::SimpleMath::Vector3 castlepos;
		static ObjPtr<SnowballManager> instance;

		USCRIPT_PROPERTY()
		ObjPtr<GameObject> m_Player;
	};
}
