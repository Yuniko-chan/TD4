#include "CourseTraversalSystem.h"
#include "../../Engine/2D/ImguiManager.h"

// コース踏破回数
int32_t CourseTraversalSystem::courseTraversalNum_ = 0;

void CourseTraversalSystem::Initialize()
{

	courseTraversalRank_ = std::make_unique<CourseTraversalRank>();
	courseTraversalRank_->Initialize(courseTraversalNum_);

	courseTraversalDraw_ = std::make_unique<CourseTraversalDraw>();
	courseTraversalDraw_->Initialize(courseTraversalNum_);

}

void CourseTraversalSystem::Update()
{

	courseTraversalRank_->Update(courseTraversalNum_);

	courseTraversalDraw_->Update();

}

void CourseTraversalSystem::Draw()
{

	courseTraversalDraw_->Draw();

}

void CourseTraversalSystem::ImGuiDraw()
{

	ImGui::Begin("CourseTraversalSystem");
	ImGui::DragInt("コース踏破回数", &courseTraversalNum_, 0.1f, 0, 100);
	courseTraversalRank_->ImGuiDraw();
	ImGui::End();
	
}
