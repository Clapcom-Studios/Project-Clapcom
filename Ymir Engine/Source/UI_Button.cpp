#include "UI_Button.h"
#include "GameObject.h"
#include "G_UI.h"

#include "UI_Canvas.h"
#include "ModuleEditor.h"

#include "External/mmgr/mmgr.h"

UI_Button::UI_Button(GameObject* g, int x, int y, int w, int h) : C_UI(UI_TYPE::BUTTON, ComponentType::UI, g, "Button", x, y, w, h)
{
	isInteractable = true;

	focusedColor = { 0, 1, 0, 1 };
	pressedColor = { 1, 0, 1, 1 };
	selectedColor = { 0, 0, 1, 1 };
	disabledColor = { 1, 1, 1, 1 };

	image = nullptr;
	displayText = nullptr;
}

UI_Button::~UI_Button()
{
	// Image is already another GameObject
	image = nullptr;
}

void UI_Button::OnInspector()
{
	bool exists = true;
	ImGui::Checkbox(("##UI Button Checkbox" + std::to_string(mOwner->UID)).c_str(), &active);		ImGui::SameLine();

	if (ImGui::CollapsingHeader(("Button##UI Collapsing Header" + std::to_string(mOwner->UID)).c_str(), &exists, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!active) { ImGui::BeginDisabled(); }

		ImGui::Dummy(ImVec2(0, 10));

		if (ImGui::Checkbox("Interactable", &isInteractable))
		{
			if (!isInteractable)
			{
				image->color = disabledColor;
			}
		} ImGui::SameLine();
		ImGui::Checkbox("Draggeable", &isDraggable);

		ImGui::Dummy(ImVec2(0, 10));
		ImGui::Text("Text: %s", displayText->text.c_str());
		ImGui::Dummy(ImVec2(0, 10));

		ImGui::SeparatorText("Colors");
		ImGui::ColorEdit4("Normal color", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Focused color", (float*)&focusedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Pressed color", (float*)&pressedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Selected color", (float*)&selectedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Disabled color", (float*)&disabledColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

		{
			ImGui::Dummy(ImVec2(0, 10));
			ImGui::Text("Funtion: "); ImGui::SameLine();

			ImGui::Text("<null>");
		}

		if (!active) { ImGui::EndDisabled(); }
	}
	ImGui::SameLine();

	if (!exists) { mOwner->RemoveComponent(this); }
}

void UI_Button::OnNormal()
{
	if (isInteractable)
	{
		image->color = color;
	}
	else
	{
		image->color = disabledColor;
	}
}

void UI_Button::OnFocused()
{
	image->color = focusedColor;
}

void UI_Button::OnPressed()
{
	image->color = pressedColor;
}

void UI_Button::OnSelected()
{
	image->color = selectedColor;
}

void UI_Button::OnRelease()
{

}
