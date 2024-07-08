#include "userinterface.h"
#include "includes.h"
#include "global.h"
#include "cod4/include.h"
#include "savefile.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"

#include <string>
#include <format>
#include "cod4/angles.h"

#define PUSHGREYBTN(x) \
PushStyleColor(ImGuiCol_Button, ImColor(50, 200, 100, 80).Value); \
PushStyleColor(ImGuiCol_ButtonHovered, ImColor(50, 190, 110, 200).Value); \
PushStyleColor(ImGuiCol_ButtonActive, ImColor(50, 200, 150, 255).Value); \
x \
PopStyleColor(3);

#define PUSHGREENBTN(x) \
PushStyleColor(ImGuiCol_Button, ImColor(50, 200, 100, 80).Value); \
PushStyleColor(ImGuiCol_ButtonHovered, ImColor(50, 190, 110, 200).Value); \
PushStyleColor(ImGuiCol_ButtonActive, ImColor(50, 200, 150, 255).Value); \
x \
PopStyleColor(3);

#define PUSHREDBTN(x) \
PushStyleColor(ImGuiCol_Button, ImColor(200, 50, 50, 80).Value); \
PushStyleColor(ImGuiCol_ButtonHovered, ImColor(190, 50, 60, 200).Value); \
PushStyleColor(ImGuiCol_ButtonActive, ImColor(200, 50, 100, 255).Value); \
x \
PopStyleColor(3);



namespace userinterface
{
	void DrawCurrentRecordingMenu();
	void DrawRecordingsMenu();
	void DrawDebugMenu();
	void DrawIndicators();
	void DrawIndicator(ImVec2 pos, ImVec2 size, std::string text);
	void TextCentered(std::string text);
	void ButtonToggle(const char* label, bool& v);

	bool showCurrentRecordingMenu = true;
	bool showReplaysMenu = true;
	bool showSpeedGraph = false;
	bool showDebugMenu = false;
	bool showDemoMenu = false;

	void Draw()
	{
		using namespace ImGui;
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		NewFrame();

		if (userinterface::showMenu)
		{
			GetIO().MouseDrawCursor = true;

			BeginMainMenuBar();
			
			ButtonToggle("Current Recording", showCurrentRecordingMenu);
			ButtonToggle("Replays", showReplaysMenu);
			ButtonToggle("Speed Graph", showSpeedGraph);
			ButtonToggle("Debug", showDebugMenu);
			ButtonToggle("ImGui Demo", showDemoMenu);

			EndMainMenuBar();
			
			if (showCurrentRecordingMenu) DrawCurrentRecordingMenu();
			if (showReplaysMenu) DrawRecordingsMenu();
			if (showDebugMenu) DrawDebugMenu();
			if (showDemoMenu) ShowDemoWindow();
		}
		else
			GetIO().MouseDrawCursor = false;

		DrawIndicators();

		EndFrame();
		Render();
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


		//static auto in_mouse = GetDvar(dataid::dvar::in_mouse.offset);

		//if (in_mouse != nullptr)
		//{
		//	in_mouse->current.enabled = showMenu ? 0 : 1;
		//	in_mouse->latched.enabled = showMenu ? 0 : 1;
		//	in_mouse->modified = 0;

		//	using restart_input_fn = void (*)();
		//	((restart_input_fn)(dataid::func::restart_input.addr))();
		//	auto& mouse = *(uint8_t*)dataid::boolean::mouse_enabled.addr;
		//	mouse = in_mouse->current.enabled;
		//}

		if (showMenu)
		{
			recorder::StopRecordingSegment();
			if (replayer::isReplaying && !replayer::autoReplay)
				replayer::Stop();
		}
	}

	void DrawCurrentRecordingMenu()
	{
		using namespace ImGui;

		{
			Begin("Current Recording");

			static float height = CalcTextSize("A").y + 3 * 2/*frame padding * 2*/;
			float width = GetContentRegionAvailWidth();
			auto btnSize = ImVec2(width, height);

			PushItemWidth(width);
			if (recorder::currentRecording == nullptr)
			{
				if (Button("New Recording", btnSize))
				{
					recorder::NewRecording();
					//global::wantsRecord = true;
					//userinterface::showMenu = false;
				}
				PopItemWidth();
				End();
				return;
			}
			BeginTabBar("CurrentRecordingTab");
			static bool skbidi = true;
			if (!BeginTabItem(
				std::format("{}###currentrectab{}",
					recorder::currentRecording->name,
					recorder::currentRecording->uuid).c_str(),
				&skbidi))
			{
				if (!skbidi)
					recorder::currentRecording = nullptr;
				skbidi = true;
				EndTabBar();
				PopItemWidth();
				End();
				return;
			}

			InputText(std::format("###currentname{}", recorder::currentRecording->uuid).c_str(), recorder::currentRecording->name, 63);


			if (recorder::currentSegment.size() == 0)
			{
				if (recorder::currentRecording->cmds.size() == 0)
				{
					if (Button("Start Recording", btnSize))
					{
						recorder::WantNewSegment(true);
					}
					NewLine();
				}
				else
				{
					if (Button("New Segment", btnSize))
					{
						recorder::WantNewSegment(false);
					}
					HelpMarker(
						"This will start replaying the current recording.\n"
						"Begin moving to start the segment.\n"
						"The segment will automatically begin if the end of the recording has been reached."
					);
				}
			}
			else
			{
				Text("current segment:");
				Text("cmd : %u", recorder::currentSegment.size());
				Text("idx : %u", recorder::currentSegmentStartIndex);

				PUSHGREENBTN(
				if (Button("Apply Segment", btnSize))
				{
					recorder::SaveSegmentToCurrentRecording();
				});

				PUSHREDBTN(
				if (Button("Discard Segment", btnSize))
				{
					recorder::DiscardSegment();
				});
			}

			NewLine();

			PUSHGREENBTN(
			if (Button("Replay", btnSize))
			{
				replayer::WantReplay();
			})

			if (!replayer::autoReplay)
			{
				PUSHGREENBTN(
				if (Button("Auto Replay", btnSize))
				{
					replayer::WantReplay(true);
					ToggleMenu(-1);
				})
			}
			else
			{
				PUSHREDBTN(
				if (Button("Stop Auto Replay", btnSize))
				{
					replayer::autoReplay = false;
					replayer::Stop();
				})
			}
			EndTabItem();
			EndTabBar();
			PopItemWidth();
			End();
		}
	}

	void DrawRecordingsMenu()
	{
		using namespace ImGui;
		Begin("Recordings");

		if (CollapsingHeader("Recordings in Memory"))
		{
			for (std::shared_ptr<recorder::Recording> recording : recorder::recordings)
			{
				if (TreeNode(std::format("%s###mem{}", recording->uuid).c_str(), recording->name))
				{
					InputText(std::format("###memname{}", recording->uuid).c_str(), recording->name, 63);
					
					Text("uuid : %llu", recording->uuid);
					Text("pos  : %f, %f, %f", recording->startPos.x, recording->startPos.y, recording->startPos.z);
					Text("rot  : %f, %f", recording->startRot.x, recording->startRot.y);
					Text("cmds : %lu", recording->cmds.size());
					Text("data @ %lu", recording->onDiskOffset);
					Text("disk : %s", recording->onDisk ? "true" : "false");
					
					if (recorder::currentRecording != recording)
					{
						if (Button("Select"))
							recorder::currentRecording = recording;
					}
					else
					{
						if (Button("Deselect"))
							recorder::currentRecording = nullptr;
					}

					if (!recording->onDisk)
					{
						if (Button("Save to Disk"))
							savefile::SaveRecordingToDisk(recording);
					}
					else
					{
						if (Button("Delete from Disk"))
						{
							savefile::DeleteRecordingOnDisk(recording->uuid);
						}
					}

					TreePop();
				}
			}
		}

		if (CollapsingHeader("Peek Recordings on Disk"))
		{
			/*auto& diskRecordings = savefile::PeekSavedRecordings();

			for (auto& disk : diskRecordings)
			{
				if (!disk.inUse)
					continue;

				if (TreeNode(std::format("{}###disk{}", disk.name, disk.uuid).c_str()))
				{
					Text("uuid : %llu", disk.uuid);
					Text("cmds : %lu", disk.cmdCount);
					Text("data @ %lu", disk.offset);
					Text("used : %s", disk.inUse ? "True" : "False");
					Text("frag : %lu", disk.fragmentCmdCap);
					
					if (Button("Load"))
					{
						savefile::LoadRecordingFromDisk(disk.uuid);
					}
					if (Button("Delete"))
					{
						savefile::DeleteRecordingOnDisk(disk.uuid);
					}

					TreePop();
				}
			}*/
		}
		End();
	}

	void DrawDebugMenu()
	{
		using namespace ImGui;

		{
			Begin("Debug Controls");

			fvec2 deltaangles = { dataptr::cg->nextSnap->ps.delta_angles[0], dataptr::cg->nextSnap->ps.delta_angles[1] };
			Text("nextSnap->ps.delta_angles");
			Text("    %f, %f", deltaangles.x, deltaangles.y);

			fvec2 viewangles = { dataptr::client->viewangles[0], dataptr::client->viewangles[1] };
			Text("client->viewangles");
			Text("    %f, %f", viewangles.x, viewangles.y);

			fvec2 realangles = AngleNormalize180(viewangles + deltaangles);
			Text("realangles");
			Text("    %f, %f", realangles.x, realangles.y);

			//fvec2 cmdangles = { SHORT2ANGLE(global::cmd.viewangles[0]), SHORT2ANGLE(global::cmd.viewangles[1])};
			//Text("cmd angle");
			//Text("    %f, %f", cmdangles.x, cmdangles.y);

			//fvec2 deltas = AngleDelta(deltaangles, cmdangles);
			//Text("deltas");
			//Text("    %f, %f", deltas.x, deltas.y);

			//fvec2 real_delta = AngleDelta(deltas, deltaangles);
			//Text("real_delta");
			//Text("    %f, %f", real_delta.x, real_delta.y);

			//fvec2 test = { -AngleNormalize180(cmdangles.x), -AngleNormalize180(cmdangles.y) };
			//Text("test");
			//Text("    %f, %f", test.x, test.y);

			//fvec2 final = AngleDelta(deltaangles, fvec2(0.f, 90.f));
			//Text("final");
			//Text("    %f, %f", final.x, final.y);

			//fvec2 finaladd = test - final;
			//Text("finaladd");
			//Text("    %f, %f", finaladd.x, finaladd.y);


			Text("com_maxfps %i", GetDvar(dataid::dvar::com_maxfps.offset)->current.integer);

			if (CollapsingHeader("cmd view"))
			{
				global::viewcmd = true;
				{
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

			Checkbox("Debug prints", &global::debugPrints);

			/*auto& diskRecordings = savefile::PeekSavedRecordings();

			for (auto& disk : diskRecordings)
			{
				if (TreeNode(std::format("{}###disk{}", disk.name, disk.uuid).c_str()))
				{
					Text("uuid : %llu", disk.uuid);
					Text("cmds : %lu", disk.cmdCount);
					Text("data @ %lu", disk.offset);
					Text("used : %s", disk.inUse ? "True" : "False");
					Text("frag : %lu", disk.fragmentCmdCap);

					if (Button("Load"))
					{
						savefile::LoadRecordingFromDisk(disk.uuid);
					}
					if (Button("Delete"))
					{
						savefile::DeleteRecordingOnDisk(disk.uuid);
					}

					TreePop();
				}
			}*/

			End();
		}
	}

	void DrawIndicators()
	{
		using namespace ImGui;
		RECT clientRect;
		GetClientRect(global::window, &clientRect);
		auto defaultSize = ImVec2(160, 60);

		//speed graph
		if (showSpeedGraph)
		{
			static ImGuiWindowFlags flags =
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoInputs |
				ImGuiWindowFlags_NoMouseInputs |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoBackground;

			ImVec2 size = defaultSize;
			size.x *= 2;
			size.y *= 2;
			SetNextWindowSize(size);
			SetNextWindowPos(ImVec2(clientRect.right / 2 - size.x / 2, (clientRect.bottom * 6) / 7));

			PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
			
			PushStyleColor(ImGuiCol_FrameBg, ImColor(0,0,0,50).Value);
			
			Begin("SpeedGraphIndicator", 0, flags);
			
			static int speedLogIdx = 0;
			static std::array<float, 128> speedLog = {};
			
			int currentSpeed = static_cast<int>(MagnitudeXY(dataptr::client->cgameVelocity));

			speedLog.at(speedLogIdx) = static_cast<float>(currentSpeed);
			auto speedText = std::format("{}", currentSpeed);

			speedLogIdx = (speedLogIdx + 1) % speedLog.size();
			ImGui::PlotLines("", speedLog.data(), speedLog.size(), speedLogIdx, speedText.c_str(), 0.0f, 700.0f, size);
			End();
			PopStyleColor();
			PopStyleVar(2);
		}

		// countdown indicators
		{
			if (userinterface::replayCountDown > 0)
			{
				std::string text = std::format("Replaying in {}", userinterface::replayCountDown);
				DrawIndicator(ImVec2(clientRect.right / 2 - defaultSize.x / 2, (clientRect.bottom * 3) / 4), defaultSize, text);
			}
			else if (replayer::isReplaying)
			{
				std::string text = std::format("[- {} / {} -]",
					replayer::recordingIndex + 1,
					recorder::currentRecording->cmds.size());
				DrawIndicator(ImVec2(clientRect.right / 2 - defaultSize.x / 2, (clientRect.bottom * 3) / 4), defaultSize, text);
			}

			if (userinterface::recordCountDown > 0)
			{
				std::string text = std::format("Recording in {}", userinterface::recordCountDown);
				DrawIndicator(ImVec2(clientRect.right / 2 - defaultSize.x / 2, (clientRect.bottom * 3) / 4), defaultSize, text);
			}
			else if (recorder::isRecording)
			{
				std::string text = std::format("[+ {} +]", recorder::currentSegment.size());
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

	void ButtonToggle(const char* label, bool& v)
	{
		using namespace ImGui;

		if (v == false)
		{
			PushStyleColor(ImGuiCol_Button, ImColor(50, 50, 60, 80).Value);
			PushStyleColor(ImGuiCol_ButtonHovered, ImColor(60, 60, 70, 200).Value);
			PushStyleColor(ImGuiCol_ButtonActive, ImColor(65, 65, 75, 255).Value);
			if (Button(label))
			{
				v = !v;
			}
			PopStyleColor(3);
		}
		else
			if (Button(label))
				v = !v;
	}
}
