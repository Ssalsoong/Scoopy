#include "Export.h"
#include "ScriptBehaviour.h"
#include "TileMap.h"
#include "PlayerController.h"
#include "MMMTime.h"
#include "MeshRenderer.h";


void MMMEngine::TileMap::SetOneTimeValue(int value)
{
    OneTimeGetValue = value;
}

float MMMEngine::TileMap::DistXZ(const Vec2& a, const Vec2& b)
{
    float dx = b.x - a.x;
    float dz = b.z - a.z;
    return std::sqrt(dx * dx + dz * dz);
}

bool MMMEngine::TileMap::InBounds(int ix, int iz)
{
    return (0 <= ix && ix < GRID_W && 0 <= iz && iz < GRID_H);
}

void MMMEngine::TileMap::WorldToTile(float x, float z, float originX, float originZ, float tileSize, int& outIx, int& outIz)
{
    outIx = (int)std::floor((x - originX) / tileSize);
    outIz = (int)std::floor((z - originZ) / tileSize);
}

void MMMEngine::TileMap::EnterState(const Vec2& startPos)
{
    prevPos = startPos;
    hasPrev = true;
}

void MMMEngine::TileMap::TileCheck(const Vec2& pos)
{
    if (!hasPrev)
    {
        EnterState(pos);
        return;
    }

    float d = DistXZ(prevPos, pos);

    int ix, iz;
    WorldToTile(pos.x, pos.z, originX, originZ, tileSize, ix, iz);

    if (auto p = P_trans->GetComponent<PlayerController>(); p.IsValid())
    {
        if (p->HasCurrentSnow() && p->IsScoopFull())
        {
            prevPos = pos;
            return;
        }
    }

    if (InBounds(ix, iz))
    {
        int idx = GetIndex(ix, iz);
        TileState& tile = tiles[idx];

        if (tile.phase != TilePhase::Cleared)
        {
            tile.phase = TilePhase::Harvesting;

            tile.accum += d;

            float t = tile.accum / threshold;
            tile.fade = std::clamp(t, 0.f, 1.f);

            if (idx < boxlist.size() && boxlist[idx].IsValid())
            {
                auto renderer = boxlist[idx]->GetComponent<MeshRenderer>();
                if (renderer)
                {
                    renderer->SetDitherAlpha(1 - tile.fade);
                }
            }

            if (tile.fade >= 1.f)
            {
                tile.phase = TilePhase::Cleared;
                tile.respawn = 0.f;

                if (P_trans.IsValid())
                {
                    auto playerGo = P_trans->GetGameObject();
                    if (playerGo.IsValid())
                    {
                        if (auto pc = playerGo->GetComponent<PlayerController>(); pc.IsValid())
                            pc->AddScoop(1);
                    }
                }


                if (boxlist[idx].IsValid())
                    boxlist[idx]->SetActive(false);

                CheckTiles.emplace_back(RespawnEntry{ ix, iz });
            }
        }
    }

    prevPos = pos;
}

MMMEngine::TileMap::Vec2 MMMEngine::TileMap::GetCurPosXZ() const
{
    Vec2 pose{};
    if (!P_trans) return pose;

    if (!P_trans.IsValid()) { std::cout << u8"transform 발견못함" << std::endl;};
    auto world_Position = P_trans->GetWorldPosition();
    pose.x = world_Position.x;
    pose.z = world_Position.z;
    return pose;
}

int MMMEngine::TileMap::GetIndex(int x, int y)
{
    return y * GRID_W + x;
}


bool MMMEngine::TileMap::IsTileClearedAt(float x, float z)
{
    int ix, iz;
    WorldToTile(x, z, originX, originZ, tileSize, ix, iz);
    if (!InBounds(ix, iz)) return false;
    int idx = GetIndex(ix, iz);
    return (tiles[idx].phase == TilePhase::Cleared);
}


void MMMEngine::TileMap::NoticePlayer(bool value)
{
    isHarvesting = value;

    if (isHarvesting)
    {
        //모션 시작: prevPos를 현재 위치로 맞추고 hasPrev 활성화
        Vec2 cur = GetCurPosXZ();
        EnterState(cur);      // prevPos=cur, hasPrev=true
    }
    else
    {
        // 모션 종료
        hasPrev = false;
    }
}

void MMMEngine::TileMap::Start()
{


    hasPrev = false;
    isHarvesting = false;

    originX = -10.0f;
    originZ = -10.0f;

    tiles.resize(GRID_W * GRID_H);
    boxlist.reserve(GRID_W * GRID_H);
    CheckTiles.reserve(GRID_W * GRID_H);

    if (auto go = GameObject::Find("Player"); go.IsValid())
    {
        //이거 player임 이름 수정못했음
        P_trans = go->GetTransform();
    }


    if (box)
    {
        for (int iz = 0; iz < GRID_H; ++iz)
        {
            for (int ix = 0; ix < GRID_W; ++ix)
            {
                auto obj = Instantiate(box);

                float wx = originX + (ix + 0.5f) * tileSize;
                float wz = originZ + (iz + 0.5f) * tileSize;

                obj->GetTransform()->SetWorldPosition({ wx, 0.0f, wz });
                boxlist.push_back(obj);
            }
        }
    }
}

void MMMEngine::TileMap::Update()
{
    UpdateRespawn();
    int curIdx = -1;
    if (isHarvesting)
    {
        Vec2 cur = GetCurPosXZ();
        int ix, iz;
        WorldToTile(cur.x, cur.z, originX, originZ, tileSize, ix, iz);
        if (InBounds(ix, iz))
            curIdx = GetIndex(ix, iz);

        TileCheck(cur); // harvesting 중일 때만 증가
    }

    UpdateAccumDecay(curIdx);
}


void MMMEngine::TileMap::UpdateRespawn()
{
    if (CheckTiles.empty()) return;

    float dt = Time::GetDeltaTime();

    for (size_t i = 0; i < CheckTiles.size();)
    {
        auto& e = CheckTiles[i];
        int idx = GetIndex(e.ix, e.iz);
        TileState& tile = tiles[idx];

        tile.respawn += dt;

        if (tile.respawn >= RESPAWN_TIME)
        {
            tile.accum = threshold;
            tile.fade = 1.f;
            tile.phase = TilePhase::Regenerating;

            if (boxlist[idx].IsValid())
                boxlist[idx]->SetActive(true);

            CheckTiles[i] = CheckTiles.back();
            CheckTiles.pop_back();
            continue;
        }
        ++i;
    }
}

void MMMEngine::TileMap::UpdateAccumDecay(int currentTileIdx)
{
    float dt = Time::GetDeltaTime();

    for (int i = 0; i < tiles.size(); ++i)
    {
        if (i == currentTileIdx)
            continue;   // 지금 플레이어가 밟고 있는 타일은 제외

        TileState& tile = tiles[i];

        if (!(tile.phase == TileMap::TilePhase::Cleared) && tile.accum > 0.f)
        {
            tile.accum -= dt * accumRegenSpeed;

            if (tile.accum < 0.f)
                tile.accum = 0.f;

            // 여기서 fade도 같이 갱신 가능
            tile.fade = tile.accum / threshold;
            tile.fade = std::clamp(tile.fade, 0.f, 1.f);

            if (i < boxlist.size() && boxlist[i].IsValid())
            {
                auto renderer = boxlist[i]->GetComponent<MeshRenderer>();
                if (renderer)
                {
                    renderer->SetDitherAlpha(1 - tile.fade);
                }
            }
        }
    }
}

