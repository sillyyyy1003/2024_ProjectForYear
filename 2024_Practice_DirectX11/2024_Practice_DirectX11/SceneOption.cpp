#include "SceneOption.h"
#include <memory>
#include "KInput.h"
#include "SceneManager.h"
#include "UI_Square.h"


void SceneOption::Init()
{
	test = std::make_shared<UI_Square>();
	test->Init(nullptr, 20, DirectX::XMINT2(1, 1));
	test->LoadDefShader("Assets/Shader/VS_DefaultUI.cso", "Assets/Shader/PS_DefaultUI.cso");
	test->SetScale({ 300,300 });

    container = std::make_unique<UIStackContainer>();

}

void SceneOption::UnInit()
{
}

void SceneOption::Update(float dt)
{
    static int count;\
	if (KInput::IsKeyTrigger(VK_ESCAPE))
		SceneManager::Get()->SetMainScene("Title");

	test->Update();

    ImGui::Begin("Counter Window");

    // 创建一个按钮
    if (ImGui::Button("Click Me"))
    {
        // 如果需要立即触发，可以在这里写逻辑
    }

    // 检查按钮是否被按下并释放（trigger 型）
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        count++;  // 每次点击后计数加1
    }

    // 显示计数值
    ImGui::Text("Count: %d", count);

    ImGui::End();

	
}

void SceneOption::Draw()
{
	
	test->Draw();
}
