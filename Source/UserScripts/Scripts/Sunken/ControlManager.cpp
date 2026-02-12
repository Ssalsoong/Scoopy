#include "Export.h"
#include "ScriptBehaviour.h"
#include "ControlManager.h"
#include "MMMInput.h"

MMMEngine::ObjPtr<MMMEngine::ControlManager> MMMEngine::ControlManager::instance;

void MMMEngine::ControlManager::Awake()
{
	if (!ControlManager::Get().IsValid())
		instance = SelfPtr(this);
	else
		Destroy(SelfPtr(this));

	mControlArray.fill(-1);
}

void MMMEngine::ControlManager::LateUpdate()
{
	for (auto& [layer, keys] : mControlMap) {
		for (auto it = keys.begin(); it != keys.end(); ) {
			if (Input::GetKeyUp(*it)) {
				int idx = static_cast<int>(*it);
				mControlArray[idx] = -1;
				it = keys.erase(it); // set에서 안전하게 제거
			}
			else {
				++it;
			}
		}
	}
}


bool MMMEngine::ControlManager::GetKeyDown(KeyCode _keycode, int _layer)
{
	// 최소 레이어보다 낮으면 무시
	if (_layer < minLayer)
		return false;

	auto& input = InputManager::Get();
	int idx = static_cast<int>(_keycode);

	if (input.GetKeyDown(_keycode)) {
		// 아직 아무도 이 키를 잡지 않았거나, 더 높은 레이어라면 갱신
		if (mControlArray[idx] < 0 || _layer >= mControlArray[idx]) {
			mControlArray[idx] = _layer;
			mControlMap[_layer].insert(_keycode); // 중복 방지
			return true;
		}
	}
	return false;
}

bool MMMEngine::ControlManager::GetKeyUp(KeyCode _keycode, int _layer)
{
	// 최소 레이어보다 낮으면 무시
	if (_layer < minLayer)
		return false;

	auto& input = InputManager::Get();
	int idx = static_cast<int>(_keycode);

	if (input.GetKeyUp(_keycode)) {
		// 해당 레이어가 현재 소유 중인 경우만 해제
		if (mControlArray[idx] == _layer) {
			mControlArray[idx] = -1;

			auto it = mControlMap.find(_layer);
			if (it != mControlMap.end()) {
				it->second.erase(_keycode); // set에서 제거
			}
			return true;
		}
	}
	return false;
}

bool MMMEngine::ControlManager::GetKey(KeyCode _keycode, int _layer)
{
	// 최소 레이어보다 낮으면 무시
	if (_layer < minLayer)
		return false;

	auto& input = InputManager::Get();
	int idx = static_cast<int>(_keycode);

	if (input.GetKey(_keycode)) {
		// 아직 아무도 이 키를 잡지 않았거나, 더 높은 레이어라면 갱신
		if (mControlArray[idx] < 0 || _layer >= mControlArray[idx]) {
			mControlArray[idx] = _layer;
			mControlMap[_layer].insert(_keycode); // 중복 방지
			return true;
		}
		// 이미 같은 레이어가 소유 중이면 true
		else if (mControlArray[idx] == _layer) {
			return true;
		}
	}
	return false;
}

void MMMEngine::ControlManager::SetMinLayer(int _layer)
{
	mInputLayers.insert(_layer);
	minLayer = *std::prev(mInputLayers.end());
}

void MMMEngine::ControlManager::ReleaseMinLayer()
{
	if (!mInputLayers.empty()) {
		auto it = std::prev(mInputLayers.end());

		// 가장 높은 레이어 제거
		mInputLayers.erase(it);

		// 남아있는 레이어 중 가장 높은 값으로 minLayer 갱신
		if (!mInputLayers.empty()) {
			minLayer = *std::prev(mInputLayers.end());
		}
		else {
			minLayer = -1; // 아무 레이어도 없을 때 기본값
		}
	}
}
