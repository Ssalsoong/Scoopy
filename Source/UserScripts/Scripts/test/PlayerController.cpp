#include "Export.h"
#include "ScriptBehaviour.h"
#include "PlayerController.h"
#include "PlayerMove.h"
#include "MMMInput.h"
#include "Transform.h"
#include "SnowCollider.h"
#include "TileMap.h"
#include "SnowballManager2.h"

#include "../Sunken/PlayerAnimController.h"
#include "../Sunken/ControlManager.h"

#include "../Mingi/Manager/SoundManager.h"

void MMMEngine::PlayerController::Start()
{
	MoveComponent = GetComponent<PlayerMove>();
	if (!MoveComponent.IsValid()) { std::cout << "MoveComponent not found" << std::endl; }
	
	m_Anime = GetComponent<PlayerAnimController>();
	if (!m_Anime.IsValid()) {
		std::cout << "PlayerController::PAcontroller not Found !!" << std::endl;
	}

	mInput = ControlManager::Get();
	if (!mInput.IsValid()) {
		std::cout << "PlayerController::ControlManager not Found !!" << std::endl;
	}
}

void MMMEngine::PlayerController::Update()
{
	InPutMove();
	InPutHoldSnow();
	CheckParticleEnable();
}

void MMMEngine::PlayerController::AddSnowList(ObjPtr<GameObject> obj)
{
	m_Snows.insert(obj);
}

void MMMEngine::PlayerController::RemoveSnowList(ObjPtr<GameObject> obj)
{
	m_Snows.erase(obj);
}

void MMMEngine::PlayerController::InPutMove()
{
	float x = 0.f, z = 0.f;

	if (mInput->GetKey(KeyCode::UpArrow, mInputLayer)) z += 1.f;
	if (mInput->GetKey(KeyCode::DownArrow, mInputLayer)) z -= 1.f;
	if (mInput->GetKey(KeyCode::RightArrow, mInputLayer)) x += 1.f;
	if (mInput->GetKey(KeyCode::LeftArrow, mInputLayer)) x -= 1.f;

	m_InputDir = DirectX::SimpleMath::Vector3(x, 0.f, z);

	auto t = GetComponent<PlayerMove>();
	t->SetInputDir(m_InputDir);
}

void MMMEngine::PlayerController::InPutHoldSnow()
{
	static bool prevSpaceFlag = false;
	bool spaceFlag = mInput->GetKey(KeyCode::Space, mInputLayer);

	if (prevSpaceFlag != spaceFlag) {
		prevSpaceFlag = spaceFlag;

		if (prevSpaceFlag) {
			m_holdSpace = true;

			// 애니메이션 재생
			m_Anime->mScooping = true;

			if (!m_Snows.empty())
			{
				AttachNearestSnow();
			}
			else
			{
				if (m_TileMap.IsValid())
					m_TileMap->GetComponent<TileMap>()->NoticePlayer(true);

				if (auto mv = GetComponent<PlayerMove>(); mv.IsValid()) {
					mv->SetScoopMode(true, nullptr);

				}

			}
		}
		else {
			m_holdSpace = false;

			// 애니메이션 재생
			m_Anime->mScooping = false;

			m_pendingAttach = false;
			m_attachDelayFrames = 0;
			SnowScoopCount = 0;
			DetachSnow();
			return;
		}
	}
	
	if (m_holdSpace && !curSnow.IsValid() && !m_Snows.empty())
	{
		AttachNearestSnow();
	}

	if (m_pendingAttach)
	{
		if (m_attachDelayFrames > 0)
		{
			--m_attachDelayFrames;
		}
		else
		{
			m_pendingAttach = false;
			if (m_holdSpace && !curSnow.IsValid() && !m_Snows.empty())
				AttachNearestSnow();
		}
	}
}

void MMMEngine::PlayerController::HasSnow(bool value)
{
	is_Snow = value;
	if (is_Snow)
		return;

	// snow destroyed or removed
	if (curSnow.IsValid())
	{
		if (auto sc = curSnow->GetComponent<SnowCollider>(); sc)
			sc->SetOnPlayer(false, nullptr);
	}

	curSnow = nullptr;
	SnowScoopCount = 0;
	m_pendingAttach = false;
	m_attachDelayFrames = 0;

	// keep harvesting if the player is still holding space
	if (m_holdSpace)
	{
		if (m_TileMap.IsValid())
			m_TileMap->GetComponent<TileMap>()->NoticePlayer(true);

		if (auto mv = GetComponent<PlayerMove>(); mv.IsValid())
			mv->SetScoopMode(true, nullptr);
	}
	else
	{
		if (m_TileMap.IsValid())
			m_TileMap->GetComponent<TileMap>()->NoticePlayer(false);

		if (auto mv = GetComponent<PlayerMove>(); mv.IsValid())
			mv->SetScoopMode(false, nullptr);
	}
}

void MMMEngine::PlayerController::AddScoop(int SnowCount)
{
	if (!m_holdSpace) return;

	// 눈 들고 있으면 거기에만 증가
	if (curSnow.IsValid())
	{
		auto sc = curSnow->GetComponent<SnowCollider>();
		if (sc)
		{
			int newCount = std::min(MaxPlayerScoop, sc->GetScoopCount() + SnowCount);
			sc->SetScoopCount(newCount);
			SnowScoopCount = newCount; // 캐시
		}
		return;
	}

	// 눈이 없으면 pending만 증가
	int prev = SnowScoopCount;
	SnowScoopCount = std::min(MaxPlayerScoop, SnowScoopCount + SnowCount);

	if (prev == 0 && SnowScoopCount == 1)
	{
		auto mgr = m_SnowManager->GetComponent<SnowballManager2>();
		mgr->Make_snow(GetGameObject());
		m_pendingAttach = true;
		m_attachDelayFrames = 1;
	}
}

void MMMEngine::PlayerController::SetMaxScoop(int MaxCount)
{
	MaxPlayerScoop = MaxCount;
}

void MMMEngine::PlayerController::AttachNearestSnow()
{
	curSnow = nullptr;
	float bestDist = FLT_MAX;

	for (auto& s : m_Snows)
	{
		if (!s.IsValid()) continue;

		float d = (s->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition()).LengthSquared();
		if (d < bestDist)
		{
			bestDist = d;
			curSnow = s;
		}
	}

	if (curSnow.IsValid())
	{
		if (auto snow = curSnow->GetComponent<SnowCollider>(); snow)
		{
			snow->SetOnPlayer(true, GetGameObject());
			SnowScoopCount = std::min(snow->GetScoopCount(), MaxPlayerScoop);
			SoundManager::Instance->PlaySFX2D("GetSnowball", SelfPtr(this), 1.0f);
		}
	}

	is_Snow = true;
	m_TileMap->GetComponent<TileMap>()->NoticePlayer(true);

	auto t = GetComponent<PlayerMove>();
	t->SetScoopMode(true, curSnow);
}

void MMMEngine::PlayerController::CheckParticleEnable()
{
	auto tr = GetTransform();

	m_particleEnable = m_InputDir.LengthSquared() > 0 &&
		SnowScoopCount < MaxPlayerScoop &&
		m_holdSpace &&
		m_TileMap.IsValid() &&
		!m_TileMap->GetComponent<TileMap>()->IsTileClearedAt(tr->GetWorldPosition().x, tr->GetWorldPosition().z);
}

void MMMEngine::PlayerController::DetachSnow()
{
	if (curSnow.IsValid())
	{
		if (auto sc = curSnow->GetComponent<SnowCollider>(); sc)
			sc->SetOnPlayer(false, nullptr);
	}

	is_Snow = false;
	curSnow = nullptr;

	if (m_TileMap.IsValid())
		m_TileMap->GetComponent<TileMap>()->NoticePlayer(false);

	if (auto mv = GetComponent<PlayerMove>(); mv.IsValid())
		mv->SetScoopMode(false, nullptr);
}

bool MMMEngine::PlayerController::IsParticleEnable()
{
	return m_particleEnable;
}
