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


		struct TileState
		{
			bool cleared = false;     // true = ����� ����
			float accum = 0.0f;       // �̵� ����
			float respawn = 0.0f;     // ������ Ÿ�̸�

			bool inRespawn = false;
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
			float threshold = 1.5f; // Ÿ�� �ȿ��� �� ��ġ��ŭ �����̸� �ڿ�ȹ��


		Vec2 prevPos{};
		bool hasPrev = false;


		bool isHarvesting = false; // ĳ�»���
		bool wasHarvesting = false; // ���� ������



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

		//���� player��ġ ��� �Լ�
		Vec2 GetCurPosXZ() const;

		int GetIndex(int x, int y);


		void UpdateRespawn();

	};
}