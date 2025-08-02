#include "UIManager.h"
#include "../../Engine/base/Texture/TextureManager.h"
#include "../../Engine/2D/ImguiManager.h"

void UIManager::Initialize()
{

	// テクスチャ読み込み
	// DirectXCommon
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// ファイルパス
	const std::string filePath = "Resources/";
	for (uint32_t i = 0; i < TextureIndex::kTextureIndexOfCount; ++i) {
		textureHandles_[i] = TextureManager::Load(filePath + kTextureFileNames_[i], dxCommon_);
	}

	// UI初期化
	for (uint32_t i = 0; i < UIIndex::kUIIndexOfCount; ++i) {
		uis_[i].reset(CreateUI(kUICreateDatas_[i].classIndex));
		uis_[i]->Initialize(textureHandles_[kUICreateDatas_[i].textureIndex], kUICreateDatas_[i].position, kUICreateDatas_[i].size);
	}

	// ImGuiモード
	imGuiMode_ = 0;

	static_cast<TimeUI*>(uis_[kUIIndexTimeOneHundred].get())->SetDigitIndex(TimeUI::DigitIndex::kDigitIndexOneHundred);
	static_cast<TimeUI*>(uis_[kUIIndexTimeTen].get())->SetDigitIndex(TimeUI::DigitIndex::kDigitIndexTen);
	static_cast<TimeUI*>(uis_[kUIIndexTimeOne].get())->SetDigitIndex(TimeUI::DigitIndex::kDigitIndexOne);

	uis_[kUIIndexClock]->GetSprite()->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });

	static_cast<TutorialCheckUI*>(uis_[kUIIndexTutorialCheckEngine].get())->SetTutorialIndex(TutorialCheckUI::TutorialIndex::kTutorialIndexEngine);
	static_cast<TutorialCheckUI*>(uis_[kUIIndexTutorialCheckTire].get())->SetTutorialIndex(TutorialCheckUI::TutorialIndex::kTutorialIndexTire);
	static_cast<TutorialCheckUI*>(uis_[kUIIndexTutorialCheckFrame].get())->SetTutorialIndex(TutorialCheckUI::TutorialIndex::kTutorialIndexFrame);

	static_cast<SpecGaugeUI*>(uis_[kUIIndexSpecGaugeSpeed].get())->SetGaugeIndex(SpecGaugeUI::GaugeIndex::kGaugeIndexSpeed);
	static_cast<SpecGaugeUI*>(uis_[kUIIndexSpecGaugeRightTurn].get())->SetGaugeIndex(SpecGaugeUI::GaugeIndex::kGaugeIndexRightTurn);
	static_cast<SpecGaugeUI*>(uis_[kUIIndexSpecGaugeLeftTurn].get())->SetGaugeIndex(SpecGaugeUI::GaugeIndex::kGaugeIndexLeftTurn);

}

void UIManager::Update()
{

	for (uint32_t i = 0; i < UIIndex::kUIIndexOfCount; ++i) {
		uis_[i]->Update();
	}

}

void UIManager::Draw()
{

	for (uint32_t i = 0; i < UIIndex::kUIIndexOfCount; ++i) {
		uis_[i]->Draw();
	}

}

void UIManager::ImGuiDraw()
{

	ImGui::Begin("UI");
	ImGui::Text("Select : ");
	for (uint32_t i = 0; i < kUIIndexOfCount; ++i) {
		// 改行するタイミング
		const int kLineCheck = 4;
		if (i % kLineCheck != 0) {
			ImGui::SameLine();
		}
		ImGui::RadioButton(kUINames_[i].c_str(), &imGuiMode_, i);
	}
	
	if (imGuiMode_ < kUIIndexOfCount) {
		uis_[imGuiMode_]->ImGuiDraw();
	}

	ImGui::End();

}

void UIManager::SetVehicleCore(VehicleCore* vehicleCore)
{

	static_cast<SpecUI*>(uis_[kUIIndexSpec].get())->SetVehicleCore(vehicleCore);
	static_cast<SpecGaugeUI*>(uis_[kUIIndexSpecGaugeSpeed].get())->SetVehicleCore(vehicleCore);
	static_cast<SpecGaugeUI*>(uis_[kUIIndexSpecGaugeRightTurn].get())->SetVehicleCore(vehicleCore);
	static_cast<SpecGaugeUI*>(uis_[kUIIndexSpecGaugeLeftTurn].get())->SetVehicleCore(vehicleCore);

}

BaseUI* UIManager::CreateUI(ClassIndex index)
{

	BaseUI* result = nullptr;

	// 番号に合わせて作成
	switch (index)
	{
	case UIManager::kClassIndexBase:
		result = new BaseUI();
		break;
	case UIManager::kClassIndexManual:
		result = new ManualUI();
		break;
	case UIManager::kClassIndexTime:
		result = new TimeUI();
		break;
	case UIManager::kClassIndexTimeClock:
		result = new ClockUI();
		break;
	case UIManager::kClassIndexTutorial:
		result = new TutorialUI();
		break;
	case UIManager::kClassIndexTutorialCheck:
		result = new TutorialCheckUI();
		break;
	case UIManager::kClassIndexSpec:
		result = new SpecUI();
		break;
	case UIManager::kClassIndexSpecGauge:
		result = new SpecGaugeUI();
		break;
	case UIManager::kClassIndexOfCount:
	default:
		assert(0);
		break;
	}

	return result;

}
