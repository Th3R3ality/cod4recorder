#include "userinterface.h"
#include "includes.h"
#include "global.h"
#include "cod4/include.h"

#include <string>
#include <format>

namespace userinterface
{
	void DrawMenu();
	void DrawIndicators();
	void DrawIndicator(ImVec2 pos, ImVec2 size, std::string text);
	void TextCentered(std::string text);

	void Draw()
	{
		using namespace ImGui;
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (userinterface::showMenu)
		{
			ImGui::GetIO().MouseDrawCursor = true;
			DrawMenu();
			//ImGui::ShowDemoWindow();
		}
		DrawIndicators();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	void ToggleMenu(int override)
	{
		switch (override)
		{
		case -1:
			showMenu = false;
			break;
		case 0:
			showMenu = !showMenu;
			break;
		case 1:
			showMenu = true;
			break;
		}

		if (showMenu)
			recorder::EndRecording();
	}


	void DrawMenu()
	{
		using namespace ImGui;

		{
			Begin("Controls");

			Text("com_maxfps %i", GetDvar(dvarids::com_maxfps.offset)->current.integer);

			if (CollapsingHeader("cmd view"))
			{
				global::viewcmd = true;
				{
					Text("createcmdcount %i", global::createcmdcount);
					Text("servertime %i", global::cmd.servertime);
					Text("buttons %i", global::cmd.buttons);
					Text("viewangles0 %i", global::cmd.viewangles[0]);
					Text("viewangles1 %i", global::cmd.viewangles[1]);
					Text("viewangles2 %i", global::cmd.viewangles[2]);
					Text("weaponIndex %i", global::cmd.weaponIndex);
					Text("a1 %i", global::cmd.a1);
					Text("forwardmove %i", global::cmd.forwardmove);
					Text("sidemove %i", global::cmd.sidemove);
					Text("i1 %i", global::cmd.i1);
					Text("i2 %i", global::cmd.i2);
				}
			}
			else
				global::viewcmd = false;

			if (Button("New Recording"))
			{
				global::wantsRecord = true;
				userinterface::showMenu = false;
			}

			PushStyleColor(ImGuiCol_Button, ImColor(50, 200, 100, 80).Value);
			PushStyleColor(ImGuiCol_ButtonHovered, ImColor(50, 190, 110, 200).Value);
			PushStyleColor(ImGuiCol_ButtonActive, ImColor(50, 200, 150, 255).Value);
			if (Button("Replay"))
			{
				global::wantsReplay = true;
			}
			PopStyleColor(3);

			if (!replayer::autoReplay)
			{
				PushStyleColor(ImGuiCol_Button, ImColor(50, 200, 100, 80).Value);
				PushStyleColor(ImGuiCol_ButtonHovered, ImColor(50, 190, 110, 200).Value);
				PushStyleColor(ImGuiCol_ButtonActive, ImColor(50, 200, 150, 255).Value);
				if (Button("Auto Replay"))
				{
					replayer::autoReplay = true;
				}
				PopStyleColor(3);

			}
			else
			{
				if (Button("Stop Auto Replay"))
				{
					replayer::autoReplay = false;
					replayer::isReplaying = false;
					global::wantsReplay = false;
					replayer::replayIndex = 0;
				}
			}

			End();
		}
	}

	void DrawIndicators()
	{
		using namespace ImGui;
		RECT clientRect;
		ImVec2 defaultSize = ImVec2(160, 60);

		GetClientRect(global::window, &clientRect);
		{
			if (userinterface::replayCountDown > 0)
			{
				std::string text = std::format("Replaying in {}", userinterface::replayCountDown);
				DrawIndicator(ImVec2(clientRect.right / 2 - defaultSize.x / 2, (clientRect.bottom * 3) / 4), defaultSize, text);
			}
			else if (replayer::isReplaying)
			{
				std::string text = std::format("[- {} / {} -]", replayer::replayIndex + 1, recorder::recording.size());
				DrawIndicator(ImVec2(clientRect.right / 2 - defaultSize.x / 2, (clientRect.bottom * 3) / 4), defaultSize, text);
			}
			if (userinterface::recordCountDown > 0)
			{
				std::string text = std::format("Recording in {}", userinterface::recordCountDown);
				DrawIndicator(ImVec2(clientRect.right / 2 - defaultSize.x / 2, (clientRect.bottom * 3) / 4), defaultSize, text);
			}
			else if (recorder::isRecording)
			{
				std::string text = std::format("[+ {} +]", recorder::recording.size());
				DrawIndicator(ImVec2(clientRect.right / 2 - defaultSize.x / 2, (clientRect.bottom * 3) / 4), defaultSize, text);
			}
		}
	}

	void DrawIndicator(ImVec2 pos, ImVec2 size, std::string text)
	{
		using namespace ImGui;
		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoResize;
		flags |= ImGuiWindowFlags_NoMove;
		flags |= ImGuiWindowFlags_NoInputs;
		flags |= ImGuiWindowFlags_NoMouseInputs;
		flags |= ImGuiWindowFlags_NoTitleBar;

		SetNextWindowSize(size);
		SetNextWindowPos(pos);

		Begin("ReplayCountdownIndicator", 0, flags);
		TextCentered(text);
		End();
	}

	void TextCentered(std::string text)
	{
		float win_width = ImGui::GetWindowSize().x;
		float text_width = ImGui::CalcTextSize(text.c_str()).x;

		// calculate the indentation that centers the text on one line, relative
		// to window left, regardless of the `ImGuiStyleVar_WindowPadding` value
		float text_indentation = (win_width - text_width) * 0.5f;

		// if text is too long to be drawn on one line, `text_indentation` can
		// become too small or even negative, so we check a minimum indentation
		float min_indentation = 20.0f;
		if (text_indentation <= min_indentation)
		{
			text_indentation = min_indentation;
		}

		ImGui::SameLine(text_indentation);
		ImGui::PushTextWrapPos(win_width - text_indentation);
		ImGui::SetCursorPosY((ImGui::GetWindowSize().y - ImGui::CalcTextSize(text.c_str()).y) * 0.5f);
		ImGui::TextWrapped(text.c_str());
		ImGui::PopTextWrapPos();
	}
}
