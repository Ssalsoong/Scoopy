#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <CoreComponents.h>

namespace MMMEngine
{
    class Castle;
    class USERSCRIPTS CastleLevelController : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

            bool isActive = false;

        USCRIPT_PROPERTY()
        ObjPtr<Castle> mCastle;
        ObjPtr<Gage> mExpGage;
        ObjPtr<Gage> mHpGage;

    public:
        CastleLevelController()
        {
        }

        

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        USCRIPT_MESSAGE()
            void OnTriggerEnter(MMMEngine::CollisionInfo info);

        USCRIPT_MESSAGE()
            void OnTriggerExit(MMMEngine::CollisionInfo info);
    };
}
