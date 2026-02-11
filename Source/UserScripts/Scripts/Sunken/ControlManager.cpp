#include "Export.h"
#include "ScriptBehaviour.h"
#include "ControlManager.h"

MMMEngine::ObjPtr<MMMEngine::ControlManager> MMMEngine::ControlManager::instance;

void MMMEngine::ControlManager::RemoveKey(std::vector<KeyCode>& _keycodes, int _idx)
{
	if (_keycodes.empty())
		return;

	if (_keycodes.size() == 1) {
		_keycodes.clear();
		return;
	}
		
	_keycodes[_idx] = _keycodes.back();
	_keycodes.pop_back();
}

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
	auto& input = InputManager::Get();

	for (auto& layer : mControlArray) {
		if(layer < 0)
			continue;

		auto it = mControlMap.find(layer);
		if (it != mControlMap.end()) {
			for (int i = static_cast<int>(it->second.size()) - 1; i >= 0; i--) {
				if (input.GetKeyUp(it->second[i])) {
					RemoveKey(it->second, i);
				}
			}
		}
	}
}

bool MMMEngine::ControlManager::GetKeyDown(KeyCode _keycode, int _layer)
{
	auto& input = InputManager::Get();
	int idx = static_cast<int>(_keycode);

	if (input.GetKeyDown(_keycode)) {
		// 아직 아무도 이 키를 잡지 않았거나, 더 높은 레이어라면 갱신
		if (mControlArray[idx] < 0 || _layer >= mControlArray[idx]) {
			mControlArray[idx] = _layer;
			mControlMap[_layer].push_back(_keycode);
			return true;
		}
	}
	return false;

}

bool MMMEngine::ControlManager::GetKeyUp(KeyCode _keycode, int _layer)
{
	auto& input = InputManager::Get();
	int idx = static_cast<int>(_keycode);

	if (input.GetKeyUp(_keycode)) {
		// 해당 레이어가 현재 소유 중인 경우만 해제
		if (mControlArray[idx] == _layer) {
			mControlArray[idx] = -1;

			auto it = mControlMap.find(_layer);
			if (it != mControlMap.end()) {
				for (int i = 0; i < it->second.size(); i++) {
					if (it->second[i] == _keycode) {
						RemoveKey(it->second, i);
						break;
					}
				}
			}
			return true;
		}
	}
	return false;
}

bool MMMEngine::ControlManager::GetKey(KeyCode _keycode, int _layer)
{
	auto& input = InputManager::Get();
	int idx = static_cast<int>(_keycode);

	if (input.GetKey(_keycode)) {
		// 아직 아무도 이 키를 잡지 않았거나, 더 높은 레이어라면 갱신
		if (mControlArray[idx] < 0 || _layer >= mControlArray[idx]) {
			mControlArray[idx] = _layer;
			mControlMap[_layer].push_back(_keycode);
			return true;
		}
		// 이미 같은 레이어가 소유 중이면 true
		else if (mControlArray[idx] == _layer) {
			return true;
		}
	}
	return false;
}

//bool MMMEngine::ControlManager::GetKeyDown(KeyCode _keycode, ObjPtr<GameObject> _obj, bool _isForce /*= false*/)
//{
//	auto& input = InputManager::Get();
//	int idx = static_cast<int>(_keycode);
//
//	if (input.GetKeyDown(_keycode)) {
//		if (!mControlArray[idx].IsValid() || _isForce) {
//			mControlArray[idx] = _obj;
//			return true;
//		}
//		else if (mControlArray[idx] == _obj) {
//			return true;
//		}
//	}
//
//	return false;
//}
//
//bool MMMEngine::ControlManager::GetKeyUp(KeyCode _keycode, ObjPtr<GameObject> _obj, bool _isForce /*= false*/)
//{
//	auto& input = InputManager::Get();
//	int idx = static_cast<int>(_keycode);
//
//	if (input.GetKeyUp(_keycode)) {
//		if (!mControlArray[idx].IsValid() || _isForce) {
//			mControlArray[idx] = _obj;
//			return true;
//		}
//		else if (mControlArray[idx] == _obj) {
//			return true;
//		}
//	}
//
//	return false;
//}
//
//bool MMMEngine::ControlManager::GetKey(KeyCode _keycode, ObjPtr<GameObject> _obj, bool _isForce /*= false*/)
//{
//	auto& input = InputManager::Get();
//	int idx = static_cast<int>(_keycode);
//
//	if (input.GetKey(_keycode)) {
//		if (!mControlArray[idx].IsValid() || _isForce) {
//			mControlArray[idx] = _obj;
//			return true;
//		}
//		else if (mControlArray[idx] == _obj) {
//			return true;
//		}
//	}
//
//	return false;
//}