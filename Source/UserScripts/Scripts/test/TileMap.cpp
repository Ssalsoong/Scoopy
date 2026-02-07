#include "Export.h"
#include "ScriptBehaviour.h"
#include "TileMap.h"
#include "PlayerController.h"


int GetIndex(int x, int y)
{
    //return (y + 10) * 21 + (x + 10);
    return y * 30 + x;
}

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

bool MMMEngine::TileMap::TileCheck(const Vec2& pos)
{
    if (!hasPrev)
    {
        prevPos = pos;
        hasPrev = true;
        return false;
    }

    float d = DistXZ(prevPos, pos);

    int ix, iz;
    WorldToTile(pos.x, pos.z, originX, originZ, tileSize, ix, iz);


    bool harvestedNow = false;

    if (InBounds(ix, iz) && !gained[ix][iz])
    {
        accum[ix][iz] += d;

        if (accum[ix][iz] >= threshold)
        {
            gained[ix][iz] = true;

            int idx = GetIndex(ix, iz);
            if (0 <= idx && idx < (int)boxlist.size() && boxlist[idx].IsValid())
                boxlist[idx]->SetActive(false);
            auto p_Con = P_trans->GetComponent<PlayerController>();
            if (p_Con.IsValid())
            {
                p_Con->AddScoop(OneTimeGetValue);
            }
        }
    }

    prevPos = pos;
    return harvestedNow;
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

void MMMEngine::TileMap::ResetTile()
{
    for (int iz = 0; iz < GRID_H; ++iz)
    {
        for (int ix = 0; ix < GRID_W; ++ix)
        {
            gained[ix][iz] = true;
            accum[ix][iz] = 0.0f;
            int idx = GetIndex(ix, iz);
            if (0 <= idx && idx < (int)boxlist.size() && boxlist[idx].IsValid())
                boxlist[idx]->SetActive(true);
        }
    }
}

bool MMMEngine::TileMap::IsTileClearedAt(float x, float z)
{
    int ix, iz;
    WorldToTile(x, z, originX, originZ, tileSize, ix, iz);
    if (!InBounds(ix, iz)) return false;
    return gained[ix][iz];
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

    originX = -15.0f;
    originZ = -15.0f;

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
    if (!isHarvesting)
        return;

    Vec2 cur = GetCurPosXZ();
    TileCheck(cur);
}
