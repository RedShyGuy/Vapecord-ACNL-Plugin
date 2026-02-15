#include "cheats.hpp"
#include "Helpers/GameKeyboard.hpp"
#include "Helpers/Chat.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Inventory.hpp"
#include "Files.h"

#include <optional>
#include <span>
#include <string_view>
#include <utility>

using namespace std::literals;

namespace CTRPluginFramework {
//Chat Bubbles Don't Disappear /*Credits to Levi*/
	void bubblesDisappear(MenuEntry *entry) { 
		static Address bubble(0x2145F8);

		if(entry->WasJustActivated()) {
			bubble.Patch(0xE1A00000);
		}
		else if(!entry->IsActivated()) {
			bubble.Unpatch();
		}
	}

	static std::string Holder = "";
//65 char is max (0x82)
	void ChatCopyPaste(MenuEntry *entry) {
		if(entry->WasJustActivated()) {
			if(!File::Exists(Utils::Format(PATH_CBOARD, Address::regionName.c_str()))) {
				File::Create(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));
			}

			Holder.clear();
			File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::READ);
			LineReader reader(f_board);
			reader(Holder);
			Holder.resize(65);
			Holder.shrink_to_fit();

			f_board.Flush();
			f_board.Close();
		}

		if(entry->Hotkeys[0].IsPressed()) {
			Holder.clear();
			File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::READ);
			LineReader reader(f_board);
			reader(Holder);
			Holder.resize(65);
			Holder.shrink_to_fit();

			f_board.Flush();
			f_board.Close();

			if(GameKeyboard::Write(Holder)) {
				OSDExtras::Notify(Language::getInstance()->get(TextID::CHATCOPYPASTE_PASTE) << " " << Holder, Color(0x0091FFFF));
			}
		}

		else if(entry->Hotkeys[1].IsPressed()) {
			Holder.clear();
			if(GameKeyboard::CopySelected(Holder)) {
				File::Remove(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));
				File::Create(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));

				File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::WRITE);

				LineWriter writer(f_board);
				writer << Holder;
				writer.Flush();
				writer.Close();

				f_board.Flush();
				f_board.Close();

				OSDExtras::Notify(Language::getInstance()->get(TextID::CHATCOPYPASTE_COPY) << " " << Holder, Color(0xFF0077FF));
			}
		}

		else if(entry->Hotkeys[2].IsPressed()) {
			Holder.clear();
			if(GameKeyboard::CopySelected(Holder)) {
				GameKeyboard::DeleteSelected();

				File::Remove(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));
				File::Create(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));

				File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::WRITE);
				
				LineWriter writer(f_board);
				writer << Holder;
				writer.Flush();
				writer.Close();

				f_board.Flush();
				f_board.Close();

				OSDExtras::Notify(Language::getInstance()->get(TextID::CHATCOPYPASTE_CUT) << " " << Holder, Color(0x00FF6FFF));
			}
		}

		else if(entry->Hotkeys[3].IsPressed()) {
			if(GameKeyboard::DeleteSelected()) {
				OSDExtras::Notify(TextID::CHATCOPYPASTE_DELETE, Color::Red);
			}
		}
	}

//Force Send Chat
	void Forcesendchat(MenuEntry *entry) {
		static Address callchat(0x52440C);
		static Address patch(0x1939EC);

		if(entry->WasJustActivated()) {
			patch.Patch(0xEA000000);
		}

		if(entry->Hotkeys[0].IsDown()) {
			if(!GameKeyboard::IsOpen()) {
				return;
			}

			callchat.Call<void>(1, 2);
		}

		if(!entry->IsActivated()) {
			patch.Unpatch();
		}
	}

	void chatCommands(MenuEntry *entry) {
		if (entry->WasJustActivated()) {
			Chat::EnableCommands();
		}
		else if (!entry->IsActivated()) {
			Chat::DisableCommands();
		}
	}

	int ButtonList[5][3] = {
		{16, 5, 4}, //Copy
		{48, 5, 5}, //Paste
		{96, 5, 3}, //Cut
		{128, 5, 6}, //Delete
		{170, 5, 4}, //Send
	};

	int TouchButton(void) {
		static UIntRect CopyCoord(ButtonList[0][0], ButtonList[0][1], ButtonList[0][2] * 6, 11);
		static UIntRect PasteCoord(ButtonList[1][0], ButtonList[1][1], ButtonList[1][2] * 6, 11);
		static UIntRect CutCoord(ButtonList[2][0], ButtonList[2][1], ButtonList[2][2] * 6, 11);
		static UIntRect DeleteCoord(ButtonList[3][0], ButtonList[3][1], ButtonList[3][2] * 6, 11);
		static UIntRect SendCoord(ButtonList[4][0], ButtonList[4][1], ButtonList[4][2] * 6, 11);

		if (CopyCoord.Contains(Touch::GetPosition())) {
			return 1;
		}
		if (PasteCoord.Contains(Touch::GetPosition())) {
			return 2;
		}
		if (CutCoord.Contains(Touch::GetPosition())) {
			return 3;
		}
		if (DeleteCoord.Contains(Touch::GetPosition())) {
			return 4;
		}
		if (SendCoord.Contains(Touch::GetPosition())) {
			return 5;
		}

		return 0;
	}

	bool DrawChatButton(const Screen &scr) {
		if(!GameKeyboard::IsOpen()) {
			return false;
		}

		if (scr.IsTop) {
			return false;
		}

		scr.Draw(Language::getInstance()->get(TextID::CHAT_BUTTON_COPY), ButtonList[0][0], ButtonList[0][1], Color::Yellow, Color::Black);
		scr.Draw(Language::getInstance()->get(TextID::CHAT_BUTTON_PASTE), ButtonList[1][0], ButtonList[1][1], Color::Yellow, Color::Black);
		scr.Draw(Language::getInstance()->get(TextID::CHAT_BUTTON_CUT), ButtonList[2][0], ButtonList[2][1], Color::Yellow, Color::Black);
		scr.Draw(Language::getInstance()->get(TextID::CHAT_BUTTON_DELETE), ButtonList[3][0], ButtonList[3][1], Color::Yellow, Color::Black);
		scr.Draw(Language::getInstance()->get(TextID::CHAT_BUTTON_SEND), ButtonList[4][0], ButtonList[4][1], Color::Yellow, Color::Black);
		
		return true;
	}

	// Add a convenient button to chat.
	void ChatButton(MenuEntry *entry) {
		static Address callchat(0x52440C);

		PluginMenu *menu = PluginMenu::GetRunningInstance();
		if(entry->WasJustActivated()) {
			OSD::Run(DrawChatButton); 
			if(!File::Exists(Utils::Format(PATH_CBOARD, Address::regionName.c_str()))) {
				File::Create(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));
			}

			Holder.clear();
			File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::READ);
			LineReader reader(f_board);
			reader(Holder);
			Holder.resize(65);
			Holder.shrink_to_fit();

			f_board.Flush();
			f_board.Close();
		}
		else if(!entry->IsActivated()) {
			OSD::Stop(DrawChatButton); 
		}

		if (Controller::IsKeyPressed(Touchpad) && GameKeyboard::IsOpen()) {
			int ButtonID = TouchButton();
			switch (ButtonID) {
				case 1: { // Copy
					Holder.clear();
					if (GameKeyboard::CopySelected(Holder)) {
						File::Remove(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));
						File::Create(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));

						File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::WRITE);

						LineWriter writer(f_board);
						writer << Holder;
						writer.Flush();
						writer.Close();

						f_board.Flush();
						f_board.Close();
						OSDExtras::Notify(TextID::CHAT_BUTTON_COPIED, Color(0xFF0077FF));
					}
				}
				break;
				case 2: {// Paste
					Holder.clear();
					File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::READ);
					LineReader reader(f_board);
					reader(Holder);
					Holder.resize(65);
					Holder.shrink_to_fit();

					f_board.Flush();
					f_board.Close();

					GameKeyboard::Write(Holder);
				}
				break;
				case 3: {// Cut
					Holder.clear();
					if (GameKeyboard::CopySelected(Holder)){
						GameKeyboard::DeleteSelected();

						File::Remove(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));
						File::Create(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));

						File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::WRITE);
						
						LineWriter writer(f_board);
						writer << Holder;
						writer.Flush();
						writer.Close();

						f_board.Flush();
						f_board.Close();

						OSDExtras::Notify(TextID::CHAT_BUTTON_CUT, Color(0x00FF6FFF));
					}
				}
				break;
				case 4: // Delete
					GameKeyboard::Delete();
				break;
				case 5: // Send
					callchat.Call<void>(1, 2);
				break;
				default:
				break;
			}
		}
	}

	void key_limit(MenuEntry* entry) {
		if(!GameKeyboard::IsOpen()) {
			return;
		}
		
		if(Inventory::GetCurrent() == 4) {
			return;
		}
		
		u32 KeyData = *(u32 *)(Game::BaseInvPointer() + 0xC) + 0x1328;
		static const Address KeyEnter(0xAD7253);
		static const Address KeyAt(0xAD75C0);
	
		Process::Write8(KeyData + 0xC, 0x41);
		Process::Write8(KeyData + 0x12B, 0x44);
		Process::Write8(KeyEnter.addr, 1);
		Process::Write8(KeyAt.addr, 1);
    }

	void CustomKeyboard(MenuEntry* entry) {   	
		static constexpr u16 patch[110] = {
			0xE000, 0xE001, 0xE002, 0xE003, 0xE004, 0xE005, 0xE006, 0xE008, 0xE009,
			0xE070, 0xE06F, 0xE06C, 0xE00C, 0xE00D, 0xE00E, 0xE00F, 0xE010, 0xE011,
			0xE012, 0xE013, 0xE014, 0xE03C, 0xE03B, 0xE03D, 0xE072, 0xE019, 0xE01A,
			0xE01B, 0xE01C, 0xE01D, 0xE01E, 0xE01F, 0xE020, 0xE021, 0xE022, 0xE023,
			0xE024, 0xE025, 0xE026, 0xE027, 0xE028, 0xE029, 0xE02A, 0xE02B, 0xE02C,
			0xE02D, 0xE02E, 0xE02F, 0xE030, 0xE031, 0xE032, 0xE033, 0xE034, 0xE035,
			0xE036, 0xE037, 0xE038, 0xE039, 0xE079, 0xE07A, 0xE07B, 0xE07C, 0xE03E,
			0xE03F, 0xE040, 0xE041, 0xE042, 0xE043, 0xE044, 0xE045, 0xE046, 0xE047,
			0xE048, 0xE049, 0xE04A, 0xE04B, 0xE04C, 0xE04D, 0xE04E, 0xE04F, 0xE050,
			0xE051, 0xE052, 0xE053, 0xE054, 0xE055, 0xE056, 0xE057, 0xE058, 0xE059,
			0xE05A, 0xE05B, 0xE05C, 0xE05D, 0xE05E, 0xE05F, 0xE060, 0xE061, 0xE062,
			0xE063, 0xE064, 0xE065, 0xE069, 0xE06A, 0xE073, 0xE067, 0xE074, 0xE075,
			0xE076, 0xE077
        };	
	
		static const Address IsOpen(0xAD7050);

        if(*(u16 *)IsOpen.addr == 0x0103) {
			u32 offset = 0;
			static const Address customKey(0xAD7630);	
            offset = *(u32 *)customKey.addr;
            if(offset != 0) {
                Process::Read32(offset + 0x224, offset);

				u16 value = 0;
			//US/EU French, US/EU English, US/EU Spanish, EU Italian, EU German
				if(Process::Read16(offset + 0x26, value) && value == 0x2E) {
					Process::CopyMemory((void *)(offset + 0x26), (void *)patch, 0x6E * 2); 
				}
			//Japanese Keyboard
				else if(Process::Read16(offset + 0x1DC, value) && value == 0x3001) {
					Process::CopyMemory((void *)(offset + 0x1DC), (void *)patch, 0x6E * 2); 
				}
			}
		}
    }

	void morenumberisland(MenuEntry *entry) {
		static const Address numbers(0xAD7158);
		Process::Write8(numbers.addr, 2);
		
		if(!entry->IsActivated()) {
			Process::Write8(numbers.addr, 0);
		}
	}

	namespace {

		constexpr size_t URLEncode(std::span<char> out, std::u8string_view in) {
			constexpr const char* hex = "0123456789abcdef";
			const size_t outSize = out.size();
			size_t pos = 0;
			for (size_t i = 0; i < in.size(); i++) {
				auto& c = in[i];
				if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9')) {
					if (pos < outSize) {
						out[pos] = static_cast<char>(c);
					}
					pos++;
				}
				else {
					if (pos < outSize) {
						out[pos] = '%';
					}
					pos++;
					if (pos < outSize) {
						out[pos] = hex[(c >> 4) & 0xf];
					}
					pos++;
					if (pos < outSize) {
						out[pos] = hex[c & 0xf];
					}
					pos++;
				}
			}
			if (pos < outSize) {
				out[pos] = '\0';
			}
			pos++;
			return pos;
		}

		class Httpc {
		public:
			static std::optional<Httpc> TryCreate() {
				if (initCounter == 0) {
					Result result = httpcInit(0);
					if (R_FAILED(result)) {
						OSD::Notify(Utils::Format("http init error: %08X", result));
						return std::nullopt;
					}
					#if DEVMODE
					else {
						OSD::Notify("http initialized");
					}
					#endif
				}
				++initCounter;
				return Httpc {};
			}

			Httpc(const Httpc&) = delete;
			Httpc& operator=(const Httpc&) = delete;

			Httpc(Httpc&&) {
				++initCounter;
			}

			Httpc& operator=(Httpc&&) {
				return *this;
			}

			~Httpc() {
				if (--initCounter == 0) {
					httpcExit();
				}
			}

		private:
			constexpr Httpc() = default;

			static inline u32 initCounter {};
		};

		class TranslateContext {
		public:
			constexpr TranslateContext() = default;
			TranslateContext(const TranslateContext&) = delete;
			TranslateContext& operator=(const TranslateContext&) = delete;

			constexpr TranslateContext(TranslateContext&& other) {
				hasCtx = std::exchange(other.hasCtx, {});
			}

			constexpr TranslateContext& operator=(TranslateContext&& other) {
				if (this == &other) {
					return *this;
				}
				this->~TranslateContext();
				hasCtx = std::exchange(other.hasCtx, {});
				return *this;
			}

			~TranslateContext() {
				if (hasCtx) {
					httpcCloseContext(&ctx);
					ctx = {0, 0};
				}
			}

			bool BeginRequest(std::u8string_view input, std::string_view targetLanguage) {
				if (IsBusy()) {
					return false;
				}

				std::vector<char> buf;
				buf.resize(input.size() * 3 + 1);
				URLEncode(buf, input);
				Result result;
				{
					std::string url = Utils::Format("https://translate.googleapis.com/translate_a/single?client=gtx&sl=auto&tl=%s&dt=t&q=",
						targetLanguage.data()) + buf.data();
					result = httpcOpenContext(&ctx, HTTPC_METHOD_GET, url.data(), 0);
				}

				if (R_FAILED(result)) {
					ctx = {0, 0};
					OSD::Notify(Utils::Format("httpcOpenContext: %08x", result));
					return false;
				}

				hasCtx = true;

				result = httpcBeginRequest(&ctx);
				if (R_FAILED(result)) {
					OSD::Notify(Utils::Format("httpcBeginRequest: %08x", result));
					return false;
				}
				return true;
			}

			std::string ReceiveResponse(bool& done, bool& error) {
				done = true;
				error = true;

				Result result;
				std::underlying_type_t<HTTPC_RequestStatus> status {};
				result = httpcGetRequestState(&ctx, reinterpret_cast<HTTPC_RequestStatus*>(&status));
				if (R_FAILED(result)) {
					OSD::Notify(Utils::Format("httpcGetRequestState: %08x, %u", result, status));
					return {};
				}

				if (status < 0x8) {
					done = false;
					error = false;
					return {};
				}

				std::array<char, 0x400> recv;
				recv.back() = 0;
				result = httpcReceiveData(&ctx, reinterpret_cast<u8*>(recv.data()), recv.size());
				if (R_FAILED(result)) {
					OSD::Notify(Utils::Format("httpcReceiveData: %08x", result));
					return {};
				}

				//if the result structure ever changes, this will break...
				std::string_view recvView = {recv.data() + 4};
				size_t len = recvView.find("\",\""sv);
				if (len == std::string_view::npos) {
					OSD::Notify("received unexpected data");
					return {};
				}
				recv[len + 4] = 0;
				error = false;
				return {recvView.data(), len};
			}

			static bool IsBusy() { return ctx.servhandle != 0 || ctx.httphandle != 0; }

		private:
			//only one context at a time for now
			static inline constinit httpcContext ctx {0, 0};

			bool hasCtx = false;
		};

		const std::vector languageCodes {
			"ja"s,
			"en"s,
			"fr"s,
			"de"s,
			"it"s,
			"es"s,
			"zh-CN"s,
			"ko"s,
			"nl"s,
			"pt"s,
			"ru"s,
			"zh-TW"s,
		};
	}

	void ChatTranslationOptions(MenuEntry* entry) {
		static LanguageId targetLanguage = LanguageId::Japanese;
		static LanguageId myLanguage = LanguageId::English;

		struct ScriptWordPtr {
			const void* vtable;
			char16_t* m_pText;
			size_t m_Size;
		};

		struct TranslateMessage {
			void Update() {
				if (state == 0) {
					if (translate.IsBusy()) {
						return;
					}

					const LanguageId target = IsTranslateOther() ? myLanguage : targetLanguage;
					const auto& targetCode = languageCodes[static_cast<u32>(target)];

					std::array<char8_t, std::tuple_size_v<decltype(text)> * 4> msg;
					const auto len = utf16_to_utf8(reinterpret_cast<u8*>(msg.data()), reinterpret_cast<const u16*>(text.data()), msg.size());

					if (len < 0) {
						OSD::Notify("utf16 to utf8 conversion failed");
						state = 2;
						return;
					}

					state = translate.BeginRequest({msg.data(), std::min(static_cast<size_t>(len), msg.size())}, targetCode) ? 1 : 2;
				}
				else if (state == 1) {
					bool done;
					bool error;
					std::string result = translate.ReceiveResponse(done, error);

					if (!done) {
						return;
					}

					if (error) {
						state = 2;
						return;
					}

					if (IsTranslateOther()) {
						if (u32 player = Player::GetSaveOffset(sender)) {
							const auto& playerName = *reinterpret_cast<const std::array<u16, 9>*>(player + 0x55A8);
							std::array<char, 9 * 4> playerNameUtf8 {};
							utf16_to_utf8(reinterpret_cast<u8*>(playerNameUtf8.data()), playerName.data(), playerNameUtf8.size());
							OSDExtras::Notify((Player::GetColor(sender) << std::string(playerNameUtf8.data()) << Color::White) + ": " + result);
						}
						else OSDExtras::Notify(result);
					}
					else {
						static Address submitChat(0x218104);
						static Address bsBalloonChatInstance(0x94FD84);
						static Address wordPtrVtable(0x90B664);

						ssize_t resultLen = utf8_to_utf16(reinterpret_cast<u16*>(text.data()), reinterpret_cast<const u8*>(result.data()), text.size());
						if (resultLen < 0) {
							OSD::Notify("utf8 to utf16 conversion failed");
							state = 2;
							return;
						}
						size_t len = std::min(static_cast<size_t>(resultLen), text.size() - 1);
						text[len] = 0;
						ScriptWordPtr word(reinterpret_cast<const void*>(wordPtrVtable.addr), text.data(), len + 1);
						submitChat.Call<void>(*(u32*)bsBalloonChatInstance.addr, &word);
					}
					state = 2;
				}
			}

			bool IsDone() const {
				return state == 2;
			}

			bool IsTranslateOther() const {
				return sender < 4;
			}

			std::array<char16_t, 65> text;
			u8 sender = 4;
			u8 state = 0;
			TranslateContext translate {};
		};
		
		class TranslateCommon : public Httpc {
		public:
			TranslateCommon(Httpc&& http) : Httpc(std::move(http)) { }

			void Update()
			{
				Lock _ {lock};
				for (auto& msg : messages) msg.Update();
				std::erase_if(messages, [](const auto& msg) { return msg.IsDone(); });
			}

		protected:
			LightLock lock {};
			std::vector<TranslateMessage> messages;
		};

		class TranslateOthersMessages : public TranslateCommon {
		public:
			static std::optional<TranslateOthersMessages>& GetInstance() {
				static std::optional<TranslateOthersMessages> instance {};
				return instance;
			}

			TranslateOthersMessages(Httpc&& http) : TranslateCommon(std::move(http)) { }

			~TranslateOthersMessages() {
				getMessage.Disable();
			}

		private:
			struct Packet {
				s64 timestamp;
				u32 param;
				u8 type;
				std::array<char16_t, 65> message;
			};
			static_assert(sizeof(Packet) == 0x90);

			static void GetMessageHook(const Packet* pkt, void* out) {
				register u8 senderR4 asm("r4");
				const auto sender = senderR4;
				auto& instance = GetInstance();

				//apparently other types don't contain text, so skip those
				if (pkt->type == 0 || pkt->type == 3 || pkt->type == 4) {
					Lock _(instance->lock);
					instance->messages.emplace_back(pkt->message, sender);
				}

				const HookContext &curr = HookContext::GetCurrent();
				static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
				func.Call<void>(pkt, out);
			}

			Hook getMessage = [] {
				static Address getMessageFromPacket(0x21724C);
				Hook result;
				result.Initialize(getMessageFromPacket.addr, (u32)GetMessageHook);
				result.SetFlags(USE_LR_TO_RETURN);
				result.Enable();
				return result;
			}();
		};

		class TranslateOwnMessages : public TranslateCommon {
		public:
			static std::optional<TranslateOwnMessages>& GetInstance() {
				static std::optional<TranslateOwnMessages> instance {};
				return instance;
			}

			TranslateOwnMessages(Httpc&& http) : TranslateCommon(std::move(http)) { }

			~TranslateOwnMessages() {
				sendMessage.Disable();
			}

		private:
			static void SendMessageHook(void*, const ScriptWordPtr* chat) {
				auto& instance = GetInstance();
				Lock _(instance->lock);
				instance->messages.emplace_back();
				std::char_traits<char16_t>::copy(instance->messages.back().text.data(), chat->m_pText, chat->m_Size);
			}

			Hook sendMessage = [] {
				static Address sendChatCall(0x1939D4);
				Hook result;
				result.Initialize(sendChatCall.addr, (u32)SendMessageHook);
				result.SetFlags(USE_LR_TO_RETURN);
				result.Enable();
				return result;
			}();
		};

		if (auto& instance = TranslateOthersMessages::GetInstance()) {
			instance->Update();
		}
		if (auto& instance = TranslateOwnMessages::GetInstance()) {
			instance->Update();
		}

		if (!entry->IsActivated()) {
			TranslateOthersMessages::GetInstance().reset();
			TranslateOwnMessages::GetInstance().reset();
		}

		if (!entry->Hotkeys[0].IsPressed()) {
			return;
		}

		auto* lang = Language::getInstance();

		const std::vector<std::string> options = {
			lang->get(TextID::CHAT_TRANSLATION_OPTION_TRANSLATE_OTHER) + ": " + (TranslateOthersMessages::GetInstance()
				? lang->get(TextID::STATE_ON) : lang->get(TextID::STATE_OFF)),
			lang->get(TextID::CHAT_TRANSLATION_OPTION_TRANSLATE_OWN) + ": " + (TranslateOwnMessages::GetInstance()
				? lang->get(TextID::STATE_ON) : lang->get(TextID::STATE_OFF)),
			lang->get(TextID::CHAT_TRANSLATION_OPTION_SET_TARGET_LANGUAGE) + ": " + languageCodes[static_cast<u32>(targetLanguage)],
			lang->get(TextID::CHAT_TRANSLATION_OPTION_SET_OWN_LANGUAGE) + ": " + languageCodes[static_cast<u32>(myLanguage)],
		};

		Keyboard KB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), options);
		
		if (int choice = KB.Open(); choice == 0) {
			auto& instance = TranslateOthersMessages::GetInstance();
			if (instance) {
				instance.reset();
			}
			else {
				if (auto http = Httpc::TryCreate()) {
					instance.emplace(std::move(*http));
				}
			}
		}
		else if (choice == 1) {
			auto& instance = TranslateOwnMessages::GetInstance();
			if (instance) {
				instance.reset();
			}
			else {
				if (auto http = Httpc::TryCreate()) {
					instance.emplace(std::move(*http));
				}
			}
		}
		else if (choice == 2) {
			KB.Populate(languageCodes);
			if (int index = KB.Open(); index >= 0) {
				targetLanguage = static_cast<LanguageId>(index);
			}
		}
		else if (choice == 3) {
			KB.Populate(languageCodes);
			if (int index = KB.Open(); index >= 0) {
				myLanguage = static_cast<LanguageId>(index);
			}
		}
	}
}
