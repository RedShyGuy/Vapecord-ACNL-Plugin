#include <CTRPluginFramework.hpp>
#include "core/game_api/Chat.hpp"
#include "core/HUD.hpp"
#include <cstring>

namespace CTRPluginFramework {
    namespace {
        constexpr const char *kAnimationCommand = "a:";
        constexpr const char *kEmotionCommand = "e:";
        constexpr const char *kSnakeCommand = "s:";
        constexpr const char *kMusicCommand = "m:";
        constexpr const char *kItemCommand = "i:";
        constexpr const char *kNameCommand = "n:";
        constexpr const char *kItemFlagCommand = "f:";

        struct RuntimeState {
            Item item = {0x7FFE, 0};
        };

        struct ParsedCommand {
            std::string command;
            std::string specialCommand;
            std::string ID_8Bit;
            std::string ID_12Bit;
            std::string ID_16Bit;
            std::string SPID_16Bit;
            std::string itemName;
        };

        RuntimeState gState;

        std::string ToLowerCopy(std::string text) {
            UtilsExtras::ConvertToLowcase(text);
            return text;
        }

        u32 PackItemForDisplay(const Item &item) {
            return (static_cast<u32>(item.Flags) << 16) | item.ID;
        }

        u32 ResolvePlayerMessageDataAddress(void) {
            u8 playerIndex = Game::GetActualPlayerIndex();
            const u8 onlinePlayerIndex = Game::GetOnlinePlayerIndex();

            if (playerIndex == onlinePlayerIndex) {
                playerIndex = 0;
            } else if (playerIndex == 0) {
                playerIndex = onlinePlayerIndex;
            }

            u32 playerData = *reinterpret_cast<u32 *>(Address(0x94FD84).addr);
            playerData += 0x464;
            playerData += (0x530 * playerIndex);

            return playerData;
        }

        std::string ReadPlayerMessage(const u32 messageDataAddress) {
            std::string playerMessage;
            Process::ReadString(
                messageDataAddress + 0x40C,
                playerMessage,
                0x82,
                StringFormat::Utf16
            );
            return playerMessage;
        }

        void ClearPlayerMessage(const u32 messageDataAddress) {
            std::memset(reinterpret_cast<void *>(messageDataAddress + 0x40C), 0, 0x82);
        }

        u8 ReadPlayerIndex(const u32 messageDataAddress) {
            return *reinterpret_cast<u8 *>(messageDataAddress + 0x3EC);
        }

        bool IsPlayerMessageOnScreen(const u32 messageDataAddress) {
            u8 isVisible = 0;
            Process::Read8(messageDataAddress + 0x3F0, isVisible);
            return (isVisible != 0);
        }

        bool TryGetWorldCoords(u32 &x, u32 &y) {
            return PlayerClass::GetInstance()->GetWorldCoords(&x, &y);
        }

        void ExecuteAnimationWithReset(
            const u8 playerIndex,
            const u8 animationId,
            const u16 emotionId,
            const u16 snakeId,
            const u16 musicId,
            const u32 x,
            const u32 y,
            const u32 resetDelayMs
        ) {
            Animation::ExecuteAnimationWrapper(
                playerIndex,
                animationId,
                {0, 0},
                emotionId,
                snakeId,
                musicId,
                0,
                x,
                y,
                true
            );

            Sleep(Milliseconds(resetDelayMs));

            Animation::ExecuteAnimationWrapper(
                playerIndex,
                6,
                {0, 0},
                0,
                0,
                0,
                0,
                x,
                y,
                true
            );
        }

        ParsedCommand ParseCommandText(std::string playerText) {
            playerText.resize(25, ' ');

            ParsedCommand parsed{};
            parsed.command = ToLowerCopy(playerText.substr(0, 2));
            parsed.specialCommand = ToLowerCopy(playerText.substr(6, 2));
            parsed.ID_8Bit = playerText.substr(2, 2);
            parsed.ID_12Bit = playerText.substr(2, 3);
            parsed.ID_16Bit = playerText.substr(2, 4);
            parsed.SPID_16Bit = playerText.substr(8, 4);
            parsed.itemName = playerText.substr(2, 23);

            return parsed;
        }

        bool TryExecuteAnimationCommand(
            const u8 playerIndex,
            const u8 animationId,
            const u16 emotionId,
            const u16 snakeId,
            const u16 musicId,
            const u32 resetDelayMs
        ) {
            u32 x, y;
            if (!TryGetWorldCoords(x, y)) {
                return false;
            }

            ExecuteAnimationWithReset(playerIndex, animationId, emotionId, snakeId, musicId, x, y, resetDelayMs);
            return true;
        }

        bool TryExecuteItemCommand(Item &item) {
            u32 x, y;
            if (!TryGetWorldCoords(x, y)) {
                return false;
            }

            Dropper::PlaceItemWrapper(
                0xA,
                ReplaceEverything,
                &item,
                &item,
                x,
                y,
                0,
                0,
                0,
                0,
                0,
                0x56,
                0xA5,
                false
            );

            return true;
        }

        void HandleAnimationCommand(const ParsedCommand &parsed, const u8 playerIndex) {
            const u8 animID = StringToHex<u8>(parsed.ID_8Bit, 6);
            if (!IDChecks::AnimationValid(animID, playerIndex)) {
                HUD::Notify(
                    Language::getInstance()->get(TextID::CHAT_INVALID_ANIMATION),
                    Color::Red
                );
                return;
            }

            if (TryExecuteAnimationCommand(playerIndex, animID, 0, 0, 0, 2000)) {
                HUD::Notify(
                    Utils::Format(
                        Language::getInstance()->get(TextID::CHAT_ANIMATION).c_str(),
                        animID
                    )
                );
            }

        }

        void HandleEmotionCommand(const ParsedCommand &parsed, const u8 playerIndex) {
            const u8 emotionID = StringToHex<u8>(parsed.ID_8Bit, 1);
            if (!IDChecks::EmotionValid(emotionID)) {
                HUD::Notify(
                    Language::getInstance()->get(TextID::CHAT_INVALID_EMOTION),
                    Color::Red
                );
                return;
            }

            if (TryExecuteAnimationCommand(playerIndex, 0xAF, emotionID, 0, 0, 2000)) {
                HUD::Notify(
                    Utils::Format(
                        Language::getInstance()->get(TextID::CHAT_EMOTION).c_str(),
                        emotionID
                    )
                );
            }

        }

        void HandleSnakeCommand(const ParsedCommand &parsed, const u8 playerIndex) {
            const u16 snakeID = StringToHex<u16>(parsed.ID_12Bit, 1);
            if (!IDChecks::SnakeValid(snakeID)) {
                HUD::Notify(
                    Language::getInstance()->get(TextID::CHAT_INVALID_SNAKE),
                    Color::Red
                );
                return;
            }

            if (TryExecuteAnimationCommand(playerIndex, 0xC5, 0, snakeID, 0, 2000)) {
                HUD::Notify(
                    Utils::Format(
                        Language::getInstance()->get(TextID::CHAT_SNAKE).c_str(),
                        snakeID
                    )
                );
            }

        }

        void HandleMusicCommand(const ParsedCommand &parsed, const u8 playerIndex) {
            const u16 musicID = StringToHex<u16>(parsed.ID_12Bit, 0x660);
            if (!IDChecks::MusicValid(musicID)) {
                HUD::Notify(
                    Language::getInstance()->get(TextID::CHAT_INVALID_MUSIC),
                    Color::Red
                );
                return;
            }

            if (TryExecuteAnimationCommand(playerIndex, 0xC4, 0, 0, musicID, 100)) {
                HUD::Notify(
                    Utils::Format(
                        Language::getInstance()->get(TextID::CHAT_MUSIC).c_str(),
                        musicID
                    )
                );
            }

        }

        void HandleItemCommand(const ParsedCommand &parsed) {
            gState.item.ID = StringToHex<u16>(parsed.ID_16Bit, 0x2001);
            if (!gState.item.isValid()) {
                HUD::Notify(
                    Language::getInstance()->get(TextID::INVALID_ITEM),
                    Color::Red
                );
                return;
            }

            if (parsed.specialCommand == kItemFlagCommand) {
                gState.item.Flags = StringToHex<u16>(parsed.SPID_16Bit, 0);
            }

            if (TryExecuteItemCommand(gState.item)) {
                HUD::Notify(
                    Utils::Format(
                        Language::getInstance()->get(TextID::CHAT_ITEM).c_str(),
                        PackItemForDisplay(gState.item)
                    )
                );
            }

        }

        void HandleNameCommand(ParsedCommand parsed) {
            UtilsExtras::Trim(parsed.itemName);

            ItemNamePack match;
            if (!Item::searchByKeyword(parsed.itemName, match)) {
                HUD::Notify(
                    Language::getInstance()->get(TextID::CHAT_NO_ITEM_FOUND),
                    Color::Red
                );
                return;
            }

            gState.item = Item(match.ID);
            if (!gState.item.isValid()) {
                HUD::Notify(
                    Language::getInstance()->get(TextID::INVALID_ITEM),
                    Color::Red
                );
                return;
            }

            if (TryExecuteItemCommand(gState.item)) {
                HUD::Notify(
                    Utils::Format(
                        Language::getInstance()->get(TextID::CHAT_ITEM).c_str(),
                        PackItemForDisplay(gState.item)
                    )
                );
            }

        }

        bool ExecuteParsedCommand(const ParsedCommand &parsed, const u8 playerIndex) {
            if (parsed.command == kAnimationCommand) {
                HandleAnimationCommand(parsed, playerIndex);
                return true;
            }
            if (parsed.command == kEmotionCommand) {
                HandleEmotionCommand(parsed, playerIndex);
                return true;
            }
            if (parsed.command == kSnakeCommand) {
                HandleSnakeCommand(parsed, playerIndex);
                return true;
            }
            if (parsed.command == kMusicCommand) {
                HandleMusicCommand(parsed, playerIndex);
                return true;
            }
            if (parsed.command == kItemCommand) {
                HandleItemCommand(parsed);
                return true;
            }
            if (parsed.command == kNameCommand) {
                HandleNameCommand(parsed);
                return true;
            }

            return false;
        }
    }

    void Chat::ProcessChatCommand(void) {
        const u32 messageDataAddress = ResolvePlayerMessageDataAddress();
        if (!IsPlayerMessageOnScreen(messageDataAddress)) {
            return;
        }

        const std::string playerMessage = ReadPlayerMessage(messageDataAddress);
        if (playerMessage.empty()) {
            return;
        }

        const ParsedCommand parsed = ParseCommandText(playerMessage);
        const u8 playerIndex = ReadPlayerIndex(messageDataAddress);

        if (ExecuteParsedCommand(parsed, playerIndex)) {
            ClearPlayerMessage(messageDataAddress);
        }
    }

    void Chat::CommandCallback(void) {
        if (!PlayerClass::GetInstance()->IsLoaded()) {
            return;
        }

        ProcessChatCommand();
    }

    void Chat::EnableCommands(void) {
        PluginMenu *menu = PluginMenu::GetRunningInstance();
        if (!menu) {
            return;
        }

        *menu += CommandCallback;
    }

    void Chat::DisableCommands(void) {
        PluginMenu *menu = PluginMenu::GetRunningInstance();
        if (!menu) {
            return;
        }

        *menu -= CommandCallback;
    }
}
