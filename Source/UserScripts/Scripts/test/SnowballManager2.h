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
	class USERSCRIPTS SnowballManager2 : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
			RTTR_REGISTRATION_FRIEND

	public:
		SnowballManager2()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
		USCRIPT_MESSAGE()
		void Start();
		USCRIPT_MESSAGE()
		void Update();


		USCRIPT_PROPERTY()
		ObjPtr<GameObject> m_Player;

		USCRIPT_PROPERTY()
		ResPtr<Prefab> Pre_Snow;

		std::vector<ObjPtr<GameObject>> Snows;

		void Make_snow(ObjPtr<GameObject> player);

		void RemoveFromList(ObjPtr<GameObject> obj);

		void AssembleSnow();
		USCRIPT_PROPERTY()
		ObjPtr<GameObject> m_Castle;
		void ConsumeToCastle();

		void ConsumeToBuilding();
	};
}
