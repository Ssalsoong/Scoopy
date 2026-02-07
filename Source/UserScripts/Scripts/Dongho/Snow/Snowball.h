#pragma once
#include "ScriptBehaviour.h"
#include "SimpleMath.h"
#include "UserScriptsCommon.h"
#include "rttr/type"

namespace MMMEngine {
	class Transform;
	class Player;
	class USERSCRIPTS Snowball : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
		RTTR_REGISTRATION_FRIEND
	public:
		Snowball()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
		void Start();
		void Update();
		void EatSnow(ObjPtr<GameObject> other);
		void PointUp();
		int GetPoint() const { return point; }

		ObjPtr<GameObject>m_player;
		int point = 1;
	};
}
