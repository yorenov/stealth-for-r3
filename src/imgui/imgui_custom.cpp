#include "imgui/imgui_custom.h"

#include <imgui_internal.h>

bool
ImGui::Hotkey(const char* label,
              int* k,
              const ImVec2& size_arg) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;
	const ImVec4* colors = GetStyle().Colors;

	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, nullptr, true);
	const ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);
	const ImRect frame_bb(window->DC.CursorPos + ImVec2(label_size.x + style.ItemInnerSpacing.x, 0.0f),
	                      window->DC.CursorPos + size);
	const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, id))
		return false;

	const bool focus_requested = FocusableItemRegister(window, g.ActiveId == id);
	// const bool focus_requested_by_code = focus_requested && (g.WindowsFocusOrder.back() == window && g.
	// 	FocusRequestCurrCounterRegular == window->DC.CoFocusCounterRegular);
	// const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;
	const bool hovered = ItemHoverable(frame_bb, id, ImGuiItemFlags_None);

	if (hovered) {
		SetHoveredID(id);
	}

	if (const bool user_clicked = hovered && io.MouseClicked[0];
		focus_requested || user_clicked) {
		if (g.ActiveId != id) {
			memset(io.MouseDown, 0, sizeof(io.MouseDown));
			memset(io.KeysDown, 0, sizeof(io.KeysDown));
			*k = 0;
		}
		SetActiveID(id, window);
		FocusWindow(window);
	} else if (io.MouseClicked[0]) {
		if (g.ActiveId == id)
			ClearActiveID();
	}

	bool value_changed = false;
	int key = *k;

	if (g.ActiveId == id) {
		for (auto i = 0x08; i <= 0xA5; i++) {
			if (io.KeysDown[i]) {
				key = i;
				value_changed = true;
				ClearActiveID();
			}
		}

		if (IsKeyPressedMap(ImGuiKey_Escape)) {
			*k = 0;
			ClearActiveID();
		} else *k = key;
	}

	char buf_display[32] = "NONE";
	window->DrawList->AddRectFilled(frame_bb.Min,
	                                frame_bb.Max,
	                                GetColorU32(colors[ImGuiCol_Button]),
	                                style.FrameRounding);

	if (*k != 0 && g.ActiveId != id)
		strcpy(buf_display, szKeyNames[*k]);
	else if (g.ActiveId == id)
		strcpy(buf_display, "- press a key -");

	const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y);
	RenderTextClipped(frame_bb.Min + style.FramePadding,
	                  frame_bb.Max - style.FramePadding,
	                  buf_display,
	                  nullptr,
	                  nullptr,
	                  style.ButtonTextAlign,
	                  &clip_rect);
	if (label_size.x > 0)
		RenderText(ImVec2(total_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), label);

	return value_changed;
}

void
ImGui::ArrowButtonDisabled(const char* id,
                           const ImGuiDir dir) {
	float sz = GetFrameHeight();
	// PushStyleVar(ImGuiStyleVar_Alpha, GetStyle().Alpha * 0.5f);
	BeginDisabled(true);
	ArrowButtonEx(id, dir, {sz, sz});
	EndDisabled();
	// PopStyleVar();
}
