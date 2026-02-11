#include "Export.h"
#include "ScriptBehaviour.h"
#include "LevelUpBubble.h"
#include "InputManager.h"
#include "ControlManager.h"

void MMMEngine::LevelUpBubble::Start()
{
	if (!LevelUpManager::Get()) {
		std::cout << "LeveUpBubble::No LevelUpManager!!" << std::endl;
		Destroy(GetGameObject());
	}

	if (!mPlayer) {
		mPlayer = GameObject::FindWithTag("Player");
		if (!mPlayer) {
			std::cout << "LeveUpBubble::No Player!!" << std::endl;
			Destroy(GetGameObject());
		}
	}

	mCanvas = LevelUpManager::Get()->GetCanvas();
	mSpeechBubbleIcon = LevelUpManager::Get()->mSpeechBubbleIcon;
	mHeadline = LevelUpManager::Get()->mHeadlineText;
	mScript = LevelUpManager::Get()->mScriptText;
}

void MMMEngine::LevelUpBubble::Update()
{
	if (isActive) {
		SetUIActive(true);

		Vector2 zero = Vector2::Zero;
		SetUITrans(mSpeechBubbleIcon->GetRectTransform(), mSpeechOffset, zero);
		SetUITrans(mHeadline->GetRectTransform(), mSpeechOffset + mHeadlineOffset, zero);
		SetUITrans(mScript->GetRectTransform(), mSpeechOffset + mScriptOffset, zero);

		SetIconTrans();
		UpdateIcon();
		UpdateControl();

		if (isDirty) {
			SetHeadlineText(LevelUpManager::Get()->GetHeadline(mSelectIdx));
			SetScriptText(LevelUpManager::Get()->GetScripts(mSelectIdx));
		}
	}
	else
	{
		SetUIActive(false);
	}
}

void MMMEngine::LevelUpBubble::SetHeadlineText(const std::wstring& _text)
{
	mHeadline->SetText(_text);
}

void MMMEngine::LevelUpBubble::SetScriptText(const std::wstring& _text)
{
	mScript->SetText(_text);
}

void MMMEngine::LevelUpBubble::SetActive(bool _val)
{
	isActive = _val;
	mSelectIdx = 0;

	if (!_val) {
		isDirty = true;
	}
}

void MMMEngine::LevelUpBubble::SetIcons(std::vector<ObjPtr<Image>>& _vec)
{
	mIcons.clear();

	if (!_vec.empty()) {
		mIcons = _vec;
	}
}

void MMMEngine::LevelUpBubble::SetUIActive(bool _val)
{
	mSpeechBubbleIcon->GetGameObject()->SetActive(_val);
	mHeadline->GetGameObject()->SetActive(_val);
	mScript->GetGameObject()->SetActive(_val);

	for (auto& icon : mIcons) {
		icon->GetGameObject()->SetActive(_val);
	}
}

void MMMEngine::LevelUpBubble::SetUITrans(ObjPtr<RectTransform> _rectTrans, Vector2& _offset, Vector2& _mPadding)
{
	auto tp = mPlayer->GetTransform();
	auto wPos = tp->GetWorldPosition();

	Vector2 canvasSize = mCanvas->GetCanvasSize();
	Vector2 canvasPos = LevelUpManager::Get()->GetCanvasPos(wPos);
	Vector2 achPos = _rectTrans->GetAnchorMin();
	float camDistance = Vector3::Distance(Camera::GetMainCamera()->GetTransform()->GetWorldPosition(), wPos);
	achPos *= canvasSize;

	float distFactor = camDistance * mDistanceFactor;

	Vector2 offset = (_offset * mUIScale) / distFactor;
	Vector2 padding = (_mPadding * mUIScale) / distFactor;

	_rectTrans->SetAnchoredPosition(
		(canvasPos - achPos) + offset + padding
	);

	Vector3 baseScale{ 1.0f, 1.0f, 1.0f };
	_rectTrans->SetWorldScale((baseScale * mUIScale) / distFactor);
}

void MMMEngine::LevelUpBubble::SetIconTrans()
{
	if (mIcons.empty()) {
		std::cout << "LevelUpBubble::Icons Empty !!!" << std::endl;
		return;
	}		

	auto& rt = mIcons[0]->GetRectTransform();
	float w = rt->GetWidth();
	Vector2 p = mIconPadding * mUIScale;
	int N = (int)mIcons.size();

	float totalWidth = N * w + (N - 1) * p.x;
	float startX = -totalWidth / 2.0f + w / 2.0f;

	for (int i = 0; i < N; ++i) {
		Vector2 pos = (mIconOffset * mUIScale) + Vector2{ startX + i * (w + p.x), i * p.y };
		SetUITrans(mIcons[i]->GetRectTransform(), mSpeechOffset + (mIconOffset * mUIScale), pos);
	}
}

void MMMEngine::LevelUpBubble::UpdateControl()
{
	auto& input = ControlManager::Get();

	input->SetMinLayer(10);

	if (input->GetKeyDown(KeyCode::LeftArrow, 10)) {
		if (mSelectIdx > 0) {
			mSelectIdx--;
			isDirty = true;
		}
	}
	else if (input->GetKeyDown(KeyCode::RightArrow, 10)) {
		if (mSelectIdx < mIcons.size() - 1) {
			mSelectIdx++;
			isDirty = true;
		}
	}
	else if (input->GetKeyDown(KeyCode::Space, 10)) {
		//std::cout << "LevelUpBubble::Selected " << std::to_string(mSelectIdx) << std::endl;
		LevelUpManager::Get()->SetSelection(mSelectIdx);
		input->ReleaseMinLayer();
		isActive = false;
	}
}

void MMMEngine::LevelUpBubble::UpdateIcon()
{
	auto tp = mPlayer->GetTransform();
	auto wPos = tp->GetWorldPosition();

	float camDistance = Vector3::Distance(Camera::GetMainCamera()->GetTransform()->GetWorldPosition(), wPos);
	float distFactor = camDistance * mDistanceFactor;

	float deselectIconSize = (mDeselectIconSize * mUIScale) / distFactor;
	float selectIconSize = (mSelectIconSize * mUIScale) / distFactor;

	for (auto& icon : mIcons) {
		icon->GetRectTransform()->SetWorldScale(
			deselectIconSize, deselectIconSize, deselectIconSize);
	}

	if (mIcons.size() > mSelectIdx) {
		mIcons[mSelectIdx]->GetRectTransform()->SetWorldScale(
			selectIconSize, selectIconSize, selectIconSize);
	}
}
