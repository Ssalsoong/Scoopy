#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "Transform.h"
#include "Prefab.h"

namespace MMMEngine
{
    class Transform;

    class USERSCRIPTS TileMap : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    private:
        static constexpr int GRID_W = 30;
        static constexpr int GRID_H = 30;
    public:
        TileMap()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_PROPERTY()
        ObjPtr<Transform> P_trans = nullptr;

        struct Vec2 { float x, z; };


        float tileSize = 1.0f;
        float originX = 0.0f;
        float originZ = 0.0f;

        USCRIPT_PROPERTY()
        float threshold = 2.0f; // 타일 안에서 이 수치만큼 움직이면 자원획득

        bool  gained[GRID_W][GRID_H]{};
        float accum[GRID_W][GRID_H]{};

        Vec2 prevPos{};
        bool hasPrev = false;


        bool isHarvesting = false; // 캐는상태
        bool wasHarvesting = false; // 이전 프레임

        void NoticePlayer(bool value);

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        USCRIPT_PROPERTY()
        ResPtr<Prefab> box;


        int width = 30;
        int offset = 15;

        int index = 0;
        std::vector<ObjPtr<GameObject>> boxlist;

        void SetOneTimeValue(int value);
    private:
        int OneTimeGetValue = 1;


    public:
        float DistXZ(const Vec2& a, const Vec2& b);

        bool InBounds(int ix, int iz);

        void WorldToTile(float x, float z, float originX, float originZ, float tileSize, int& outIx, int& outIz);

        void EnterState(const Vec2& startPos);

        bool TileCheck(const Vec2& pos);

        //현재 player위치 얻는 함수
        Vec2 GetCurPosXZ() const;

        void ResetTile();

        bool IsTileClearedAt(float x, float z);


    };
}
