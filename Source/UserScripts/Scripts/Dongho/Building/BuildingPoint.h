#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "SimpleMath.h"
#include "CoreComponents.h"

namespace MMMEngine
{
    class USERSCRIPTS BuildingPoint : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

        ObjPtr<Gage> mProgGage;
        ObjPtr<Canvas> mCanvas;
    public:
        BuildingPoint()
        {
        REGISTER_BEHAVIOUR_MESSAGE(OnDisable);
        REGISTER_BEHAVIOUR_MESSAGE(OnTriggerEnter);
        REGISTER_BEHAVIOUR_MESSAGE(OnTriggerExit);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
        //float checkdist = 2.f; // 원래는 uproperty 있었음

        USCRIPT_PROPERTY()
        ObjPtr<GameObject> player;

		USCRIPT_PROPERTY()
	    ResPtr<Prefab> mUniversalGage;

        bool canBuild = false;
        bool GetcanBuild() { return canBuild; }
        bool alreadybuilt = false;
        void Setalreadybuilt(bool value) { alreadybuilt = value; }
        
        //DirectX::SimpleMath::Vector3 playerpos;
        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

		USCRIPT_MESSAGE()
			void OnTriggerEnter(MMMEngine::TriggerInfo info);

		USCRIPT_MESSAGE()
			void OnTriggerExit(MMMEngine::TriggerInfo info);

		USCRIPT_MESSAGE()
			void OnDisable();

        //void CheckPlayer();
    };
}
