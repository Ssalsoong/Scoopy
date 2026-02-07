#include "Export.h"
#include "ScriptBehaviour.h"
#include "PlayerController.h"
#include "PlayerMove.h"
#include "MMMInput.h"
#include "Transform.h"
#include "SnowCollider.h"
#include "TileMap.h"
#include "SnowballManager2.h"


void MMMEngine::PlayerController::Start()
{
	MoveComponent = GetComponent<PlayerMove>();
	if (!MoveComponent.IsValid()) { std::cout << "MoveComponent not found" << std::endl; }
}

void MMMEngine::PlayerController::Update()
{
	InPutMove();
	InPutHoldSnow();
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
	if (Input::GetKey(KeyCode::W)) z += 1.f;
	if (Input::GetKey(KeyCode::S)) z -= 1.f;
	if (Input::GetKey(KeyCode::D)) x += 1.f;
	if (Input::GetKey(KeyCode::A)) x -= 1.f;

	m_InputDir = DirectX::SimpleMath::Vector3(x, 0.f, z);

	auto t = GetComponent<PlayerMove>();
	t->SetInputDir(m_InputDir);
}

void MMMEngine::PlayerController::InPutHoldSnow()
{
	if (Input::GetKeyDown(KeyCode::Space)) {
		if (m_Snows.empty())
		{
			auto myPtr = GetGameObject();
			m_SnowManager->GetComponent<SnowballManager2>()->Make_snow(myPtr);
			if (Input::GetKey(KeyCode::Space))
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
					auto snow = curSnow->GetComponent<SnowCollider>();
					if (snow)
						snow->SetOnPlayer(true, GetGameObject());
				}

				is_Snow = true;
				m_TileMap->GetComponent<TileMap>()->NoticePlayer(true);

				auto t = GetComponent<PlayerMove>();
				t->SetScoopMode(true, curSnow);
			}
		}
		else
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
				auto snow = curSnow->GetComponent<SnowCollider>();
				if (snow)
					snow->SetOnPlayer(true, GetGameObject());
			}

			is_Snow = true;
			m_TileMap->GetComponent<TileMap>()->NoticePlayer(true);

			auto t = GetComponent<PlayerMove>();
			t->SetScoopMode(true, curSnow);
		}
	}

	if (!curSnow)
	{
		if (!is_Snow)
		{
			auto t = GetComponent<PlayerMove>();
			t->SetScoopMode(false, nullptr);
			m_TileMap->GetComponent<TileMap>()->NoticePlayer(false);
			curSnow = nullptr;
		}
	}


	if (!m_Snows.empty())
	{
		if (Input::GetKeyUp(KeyCode::Space)) {
			if (curSnow.IsValid())
			{
				auto snow = curSnow->GetComponent<SnowCollider>();
				if (snow)
					snow->SetOnPlayer(false, nullptr);
			}
			m_TileMap->GetComponent<TileMap>()->NoticePlayer(false);
			is_Snow = false;
			curSnow = nullptr;
			auto t = GetComponent<PlayerMove>();
			t->SetScoopMode(false, nullptr);
		}
	}
}

void MMMEngine::PlayerController::HasSnow(bool value)
{
	is_Snow = value;
}
