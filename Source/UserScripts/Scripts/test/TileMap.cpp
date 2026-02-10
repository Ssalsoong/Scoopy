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

	if (!P_trans.IsValid()) { std::cout << u8"transform 발견못함" << std::endl; };
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
	return tiles[idx].cleared;
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
	if (!isHarvesting)
		return;

	Vec2 cur = GetCurPosXZ();
	TileCheck(cur);
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

			if (0 <= t_index && t_index < (int)boxlist.size() && boxlist[t_index].IsValid())
				boxlist[t_index]->SetActive(true);

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