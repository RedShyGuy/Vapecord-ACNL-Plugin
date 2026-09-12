#include "core/HUD.hpp"
#include "core/game_api/Game.hpp"

namespace CTRPluginFramework {

	static const char* s_SpinnerFrames[] = {
		"|", "/", "-", "\\",
		"|", "/", "-", "\\"
	};
	static constexpr int SPINNER_FRAMES = 8;

	void SleepTime(void) {
		constexpr int tick_ms = 150;
		static constexpr Color spinnerColor{255, 0, 247};

		std::string text = "Waiting for game to load |";
		HUD::Handle handle = HUD::Show(7.0f, 10.0f, text, spinnerColor);

		int frame = 0;
		while (Game::IsRoomLoading()) {
			frame = (frame + 1) % SPINNER_FRAMES;
			text = "Waiting for game to load ";
			text += s_SpinnerFrames[frame];
			HUD::Update(handle, text, spinnerColor);
			Sleep(Milliseconds(tick_ms));
		}

		HUD::Hide(handle);
	}
}
