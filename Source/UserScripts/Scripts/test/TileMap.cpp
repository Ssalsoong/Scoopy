#include "Export.h"
#include "ScriptBehaviour.h"
#include "TileMap.h"
#include "PlayerController.h"
#include "MMMTime.h"


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

	if (auto p = P_trans->GetComponent<PlayerController>(); p.IsValid())
	{
		if (p->HasCurrentSnow() && p->IsScoopFull())
		{
			prevPos = pos;
			return;
		}
	}

	int ix, iz;
	WorldToTile(pos.x, pos.z, originX, originZ, tileSize, ix, iz);


	if (InBounds(ix, iz))
	{
		int idx = GetIndex(ix, iz);
		TileState& tile = tiles[idx];

		if (!tile.cleared)
		{
			tile.accum += d;

			if (tile.accum >= threshold)
			{
				tile.cleared = true;
				tile.respawn = 0.0f;

				if (boxlist[idx].IsValid())
					boxlist[idx]->SetActive(false);

				if (!tile.inRespawn)
				{
					tile.inRespawn = true;
					CheckTiles.emplace_back(RespawnEntry{ ix, iz });

				}

				if (auto p = P_trans->GetComponent<PlayerController>(); p.IsValid())
				{
					p->AddScoop(OneTimeGetValue);
				}
			}
		}
	}

	prevPos = pos;
}

MMMEngine::TileMap::Vec2 MMMEngine::TileMap::GetCurPosXZ() const
{
	Vec2 pose{};
	if (!P_trans) return pose;

<<<<<<< Updated upstream
	if (!P_trans.IsValid()) { std::cout << u8"transform ¹ß°ß¸øÇÔ" << std::endl; };
	auto world_Position = P_trans->GetWorldPosition();
	pose.x = world_Position.x;
	pose.z = world_Position.z;
	return pose;
=======
    if (!P_trans.IsValid()) { std::cout << u8"transform ï¿½ß°ß¸ï¿½ï¿½ï¿½" << std::endl;};
    auto world_Position = P_trans->GetWorldPosition();
    pose.x = world_Position.x;
    pose.z = world_Position.z;
    return pose;
>>>>>>> Stashed changes
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
	return tiles[idx].cleared;
}

void MMMEngine::TileMap::NoticePlayer(bool value)
{
	isHarvesting = value;

<<<<<<< Updated upstream
	if (isHarvesting)
	{
		//¸ð¼Ç ½ÃÀÛ: prevPos¸¦ ÇöÀç À§Ä¡·Î ¸ÂÃß°í hasPrev È°¼ºÈ­
		Vec2 cur = GetCurPosXZ();
		EnterState(cur);      // prevPos=cur, hasPrev=true
	}
	else
	{
		// ¸ð¼Ç Á¾·á
		hasPrev = false;
	}
=======
    if (isHarvesting)
    {
        //ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½: prevPosï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ä¡ï¿½ï¿½ ï¿½ï¿½ï¿½ß°ï¿½ hasPrev È°ï¿½ï¿½È­
        Vec2 cur = GetCurPosXZ();
        EnterState(cur);      // prevPos=cur, hasPrev=true
    }
    else
    {
        // ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
        hasPrev = false;
    }
>>>>>>> Stashed changes
}

void MMMEngine::TileMap::Start()
{


	hasPrev = false;
	isHarvesting = false;

	originX = -15.0f;
	originZ = -15.0f;

	tiles.resize(GRID_W * GRID_H);
	boxlist.reserve(GRID_W * GRID_H);
	CheckTiles.reserve(GRID_W * GRID_H);

<<<<<<< Updated upstream
	if (auto go = GameObject::Find("Player"); go.IsValid())
	{
		//ÀÌ°Å playerÀÓ ÀÌ¸§ ¼öÁ¤¸øÇßÀ½
		P_trans = go->GetTransform();
	}
=======
    if (auto go = GameObject::Find("Player"); go.IsValid())
    {
        //ï¿½Ì°ï¿½ playerï¿½ï¿½ ï¿½Ì¸ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        P_trans = go->GetTransform();
    }
>>>>>>> Stashed changes


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
	if (!isHarvesting)
		return;

<<<<<<< Updated upstream
	Vec2 cur = GetCurPosXZ();
	TileCheck(cur);
=======
        TileCheck(cur); // harvesting ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
    }

    UpdateAccumDecay(curIdx);
>>>>>>> Stashed changes
}



void MMMEngine::TileMap::UpdateRespawn()
{
	if (CheckTiles.empty()) return;
	float dt = Time::GetDeltaTime();

	for (size_t i = 0; i < CheckTiles.size();)
	{
		auto& e = CheckTiles[i];
		auto t_index = GetIndex(e.ix, e.iz);
		TileState& e_tile = tiles[t_index];

		if (e_tile.respawn >= RESPAWN_TIME)
		{
			e_tile.accum = 0.f;
			e_tile.respawn = 0.f;
			e_tile.cleared = false;
			e_tile.inRespawn = false;

<<<<<<< Updated upstream
			if (0 <= t_index && t_index < (int)boxlist.size() && boxlist[t_index].IsValid())
				boxlist[t_index]->SetActive(true);
=======
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
            continue;   // ï¿½ï¿½ï¿½ï¿½ ï¿½Ã·ï¿½ï¿½Ì¾î°¡ ï¿½ï¿½ï¿½ ï¿½Ö´ï¿½ Å¸ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½

        TileState& tile = tiles[i];

        if (!(tile.phase == TileMap::TilePhase::Cleared) && tile.accum > 0.f)
        {
            tile.accum -= dt * accumRegenSpeed;

            if (tile.accum < 0.f)
                tile.accum = 0.f;

            // ï¿½ï¿½ï¿½â¼­ fadeï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
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
>>>>>>> Stashed changes

			CheckTiles[i] = CheckTiles.back();
			CheckTiles.pop_back();
			continue;
		}
		else
		{
			e_tile.respawn += dt;
		}
		++i;
	}
}