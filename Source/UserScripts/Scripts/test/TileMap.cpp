#include "Export.h"
#include "ScriptBehaviour.h"
#include "TileMap.h"


int GetIndex(int x, int y)
{
    //return (y + 10) * 21 + (x + 10);
    return y * 21 + x;
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

    //std::cout << u8"현재 x 좌표 " << ix << u8" 현재 z 좌표 : " << iz << std::endl;

    bool harvestedNow = false;

    if (InBounds(ix, iz) && !gained[ix][iz])
    {
        accum[ix][iz] += d;

        if (accum[ix][iz] >= threshold)
        {
            gained[ix][iz] = true;
            harvestedNow = true;

            //std::cout << u8"x 좌표 : " << ix << u8"z 좌표 : " << iz << u8" 타일 활성화됨" << std::endl;
            // 사운드나 특수효과가 필요하면 여기서 호출함수 넣으면됨
            

            if (ix >= 0 && ix <= 20 && iz >= 0 && iz <= 20)
            {
                int idx = GetIndex(ix, iz);


                if (0 <= idx && idx < (int)boxlist.size() && boxlist[idx].IsValid())
                {
                    boxlist[idx]->SetActive(false);
                    std::cout << "tile=(" << ix << "," << iz << ") idx=" << idx << "\n";
                    std::cout << idx << std::endl;
                }
            }
        }
    }

    prevPos = pos;
    return harvestedNow;
}

MMMEngine::TileMap::Vec2 MMMEngine::TileMap::GetCurPosXZ() const
{
    Vec2 pose{};
    if (!trans) return pose;

    if (!trans.IsValid()) { std::cout << u8"trans 발견못함" << std::endl;};
    auto world_Position = trans->GetWorldPosition();
    pose.x = world_Position.x;
    pose.z = world_Position.z;
    return pose;
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
        // 모션 종료: 다음 시작 때 점프 누적 방지
        hasPrev = false;
    }
}

void MMMEngine::TileMap::Start()
{
    hasPrev = false;
    isHarvesting = false;

    originX = -10.0f;
    originZ = -10.0f;

    if (auto go = GameObject::Find("Player"); go.IsValid())
    {
        trans = go->GetTransform();
    }


    if (box)
    {
        for (float y = -10; y <= 10; y++)
        {
            for (float x = -10; x <= 10; x++)
            {
                auto obj = Instantiate(box);
                obj->GetTransform()->SetWorldPosition({ x + 0.5f,  0 , y + 0.5f });
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
