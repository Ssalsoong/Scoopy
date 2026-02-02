#pragma once
#include "ScriptBehaviour.h"
#include "SimpleMath.h"
#include "UserScriptsCommon.h"
#include "rttr/type"
#include "SimpleMath.h"

namespace MMMEngine {
	class Transform;
	class Player;
	class USERSCRIPTS Snowball : public ScriptBehaviour
	{
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
		int GetPoint() const { return point; };
		float GetScale() const { return scale; };
		Player* carrier = nullptr;
		bool IsCarried() const { return carrier != nullptr; }
	private:
		void RollSnow();
		float scale = 0.05f;
		float minscale = 0.05f;
		float scaleup = 0.05f;
		int point = 1;
		float baseRadius = 0.5f; // Sphere mesh 기본 반지름
		float k = 2.0f;          // 눈이 플레이어에 얼마나 붙을지 (0.7~1.4 튜닝)
		DirectX::SimpleMath::Vector3 prevWorldPos{};
		bool hasPrev = false;
	};
}
