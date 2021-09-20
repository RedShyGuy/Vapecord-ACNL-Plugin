#include <CTRPluginFramework.hpp>

#define majorV 1
#define minorV 9
#define revisV 5

namespace CTRPluginFramework {
    enum class CONFIG : s64 {
		Color = 0,
		FWKColor,
		Language,
		Info,
		GameID,
		GameVer,
		DevMode,
		Version
	};

	enum f_Language {
        NoLang = 0, //If no language was chosen yet
        JapaneseLang,
        EnglishLang,
        FrenchLang,
		GermanLang,	
		ItalianLang,
		SpanishLang,
		MaxLang
	};

	enum f_Color {
        NoMode = 0, //If No mode was chosen yet
        ColorMode,
        LiteMode,     
        LuxuryMode,
		CustomMode,
        MaxColor
	};

	enum fwk_Color {
        NoFWK = 0, //If No mode was chosen yet
		FWK_Custom,
        MaxFWK
	};

	enum f_GameVer {
		NoneVer = 0, //Message never appeared
		Declined, //Message appeared and "No" was selected
		Accepted //Message appeared and "Yes" was selected
	};

	enum f_GameID {
		NoneID = 0, 
		WrongID, 
		CorrectID 
	};

	void WriteConfig(CONFIG config, u8 byte);
	void ReadConfig(CONFIG config, u8 &byte);
	void CheckForCONFIG(void);

	bool IsDevModeUsable(void);
	void StartingMsg(void);
	void CustomFWK(bool SetInMenu);
	void SetupColors(bool SetInMenu);
	void SetupLanguage(bool SetInMenu);
	void Check_Color_Language(void);
}