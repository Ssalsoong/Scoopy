#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class MarkUIInfo;
    class Transform;
    class USERSCRIPTS MarkWaveController : public ScriptBehaviour
    {
    private:

        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        MarkWaveController()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(OnDestroy);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_PROPERTY()
            ObjPtr<MarkUIInfo> Warrior1;
        USCRIPT_PROPERTY()
            ObjPtr<MarkUIInfo> Warrior2;
        USCRIPT_PROPERTY()
            ObjPtr<MarkUIInfo> Warrior3;


        USCRIPT_PROPERTY()
            ObjPtr<MarkUIInfo> Assassin1;
        USCRIPT_PROPERTY()     
            ObjPtr<MarkUIInfo> Assassin2;
        USCRIPT_PROPERTY()     
            ObjPtr<MarkUIInfo> Assassin3;

        USCRIPT_PROPERTY()
            ObjPtr<MarkUIInfo> Ranger1;
        USCRIPT_PROPERTY()     
            ObjPtr<MarkUIInfo> Ranger2;
        USCRIPT_PROPERTY()     
            ObjPtr<MarkUIInfo> Ranger3;

        USCRIPT_PROPERTY()
            ObjPtr<Transform> num1_DW_TR;

        USCRIPT_PROPERTY()
            ObjPtr<Transform> num1_L_TR;

        USCRIPT_PROPERTY()
            ObjPtr<Transform> num2_TR;


        USCRIPT_PROPERTY()
            ObjPtr<Transform> num3_R_TR;


        USCRIPT_PROPERTY()
            ObjPtr<Transform> num3_DW_TR;


        USCRIPT_PROPERTY()
            ObjPtr<Transform> num4_TR;


        USCRIPT_PROPERTY()
            ObjPtr<Transform> num6_TR;


        USCRIPT_PROPERTY()
            ObjPtr<Transform> num7_L_TR;

        USCRIPT_PROPERTY()
            ObjPtr<Transform> num7_UP_TR;


        USCRIPT_PROPERTY()
            ObjPtr<Transform> num8_TR;


        USCRIPT_PROPERTY()
            ObjPtr<Transform> num9_R_TR;

        USCRIPT_PROPERTY()
            ObjPtr<Transform> num9_UP_TR;

        static ObjPtr<MarkWaveController> Instance;

        USCRIPT_MESSAGE()
        void Awake();

        USCRIPT_MESSAGE()
        void Update();

        USCRIPT_MESSAGE()
        void OnDestroy();

        void SetWave(int wave);
        void AllOff();
    };
}
