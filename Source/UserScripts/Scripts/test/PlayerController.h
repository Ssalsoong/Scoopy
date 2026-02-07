#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <SimpleMath.h>
#include <unordered_set>


namespace MMMEngine
{
    class SnowballManager;
    class USERSCRIPTS PlayerController : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        PlayerController()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();


        void AddSnowList(ObjPtr<GameObject> obj);

        void RemoveSnowList(ObjPtr<GameObject> obj);

        void InPutMove();

        void InPutHoldSnow();

        void HasSnow(bool value);

        void AddScoop(int SnowCount);

        void SetMaxScoop(int MaxCount);
        
        void AttachNearestSnow();

        void DetachSnow();
    private:

        DirectX::SimpleMath::Vector3 m_InputDir;

        ObjPtr<Component> MoveComponent;

        std::unordered_set<ObjPtr<GameObject>> m_Snows;

        ObjPtr<GameObject> curSnow;

        bool is_Snow = false;

        int SnowScoopCount = 0;

        int MaxPlayerScoop = 10;

        bool m_holdSpace = false;

    public:
        USCRIPT_PROPERTY()
        ObjPtr<GameObject> m_TileMap;
        USCRIPT_PROPERTY()
        ObjPtr<GameObject> m_SnowManager;

        bool IsHoldingSpace() const { return m_holdSpace; }
        bool HasCurrentSnow() const { return curSnow.IsValid(); }

        bool m_pendingAttach = false;
        int m_attachDelayFrames = 0;

        int GetScoopCount() const { return SnowScoopCount; }

    };
}
