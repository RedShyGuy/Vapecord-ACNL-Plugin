#include <CTRPluginFramework.hpp>

#define majorV 2
#define minorV 0
#define revisV 0

namespace CTRPluginFramework {
	enum f_Language {
        NoLang = 0, //If no language was chosen yet
        JapaneseLang,
        EnglishLang,
        FrenchLang,
		GermanLang,	
		ItalianLang,
		SpanishLang,
		KoreanLang, 
		MaxLang
	};

	void WriteLanguage(f_Language language);
	void ReadLanguage(f_Language &language);
	void CheckForLanguageFile(void);

	void SetupLanguage(bool SetInMenu);
	void cheatsVisibility(bool SetInMenu);
	void resetSettings(bool SetInMenu);
}