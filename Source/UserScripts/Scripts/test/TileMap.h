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
        static constexpr int GRID_W = 20;
        static constexpr int GRID_H = 20;
        
        enum class TilePhase
        {
            Idle,
            Harvesting,
            Cleared,
            Regenerating
        };

        struct TileState
        {
            TilePhase phase = TilePhase::Idle;

            float accum = 0.f;       // 이동 누적
            float fade = 0.f;        //디더링용 변수 ( 진행도라고 생각하시면됨 )
            float respawn = 0.f;     // 리스폰 타이머
        };

        struct RespawnEntry {
            int ix, iz;
        };

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
        float threshold = 0.75f; // 타일 안에서 이 수치만큼 움직이면 자원획득

        USCRIPT_PROPERTY()
        float recoverSpeed = 1.0f;        // 떠났을 때 복구 속도

        USCRIPT_PROPERTY()
        float regenFadeSpeed = 1.0f;      // 리젠 시 등장 속도

        USCRIPT_PROPERTY()
        float accumRegenSpeed = 1.0f;     // 복수 수치
   
        Vec2 prevPos{};
        bool hasPrev = false;


        bool isHarvesting = false; // 캐는상태

        //bool wasHarvesting = false; // 이전 프레임

        

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();


        USCRIPT_PROPERTY()
        ResPtr<Prefab> box;


        int width = 30;
        int offset = 15;

        //int index = 0;
        std::vector<ObjPtr<GameObject>> boxlist;
        std::vector<TileState> tiles;

        std::vector<RespawnEntry> CheckTiles;
        
        USCRIPT_PROPERTY()
        float RESPAWN_TIME = 15.0f;

        //bool  gained[GRID_W][GRID_H]{};
        //float accum[GRID_W][GRID_H]{};

    private:
        int OneTimeGetValue = 1;

    public:
        void SetOneTimeValue(int value);
        void NoticePlayer(bool value);

        bool IsTileClearedAt(float x, float z);

    private:
        float DistXZ(const Vec2& a, const Vec2& b);

        bool InBounds(int ix, int iz);

        void WorldToTile(float x, float z, float originX, float originZ, float tileSize, int& outIx, int& outIz);

        void EnterState(const Vec2& startPos);

        void TileCheck(const Vec2& pos);

        //현재 player위치 얻는 함수
        Vec2 GetCurPosXZ() const;

        int GetIndex(int x, int y);

        void UpdateRespawn();

        void UpdateAccumDecay(int currentTileIdx);
    };
}
