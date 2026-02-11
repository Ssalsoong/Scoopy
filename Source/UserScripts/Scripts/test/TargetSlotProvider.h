#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <vector>
#include <SimpleMath.h>

namespace MMMEngine
{
    class GameObject;

    class USERSCRIPTS TargetSlotProvider : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        TargetSlotProvider()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_PROPERTY()
        float baseRadius = 1.2f;
        USCRIPT_PROPERTY()
        float ringSpacing = 0.6f;
        USCRIPT_PROPERTY()
        float slotSpacing = 0.8f;
        USCRIPT_PROPERTY()
        float yOffset = 0.0f;
        USCRIPT_PROPERTY()
        int maxRings = 4;

        bool RequestSlot(ObjPtr<GameObject> enemy, int& outRing, int& outIndex);
        void ReleaseSlot(int ring, int index, ObjPtr<GameObject> enemy);
        bool GetSlotWorldPos(int ring, int index, DirectX::SimpleMath::Vector3& outPos);

    private:
        struct Slot
        {
            ObjPtr<GameObject> occupant = nullptr;
        };

        struct Ring
        {
            float radius = 0.0f;
            std::vector<Slot> slots;
        };

        std::vector<Ring> m_rings;

        bool FindExisting(ObjPtr<GameObject> enemy, int& outRing, int& outIndex);
        int GetRingCapacity(float radius) const;
        void EnsureRing(int ringIndex);
    };
}
