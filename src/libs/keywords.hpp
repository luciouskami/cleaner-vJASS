#pragma once

struct Keywords {
	//structure
	bool inGlobals;
	bool processedGlobals;
	static const char* globals;		//1
		static const char* udg_;		//multiple
		static const char* gg_;			//multiple
		static const char* endglobals;	//1
	bool processedInitGlobals;
	static const char* InitGlobals; //1

	//structure
	static const char* Trig_;		//multiple
	static const char* InitTrig_;	// 1
		bool processedRunInitializationTriggers;
			static const char* RunInitializationTriggers;	//1?
	bool processedInitCustomTriggers;
		static const char* InitCustomTriggers;		//1?

	bool processedInitCustomPlayerSlots;
		static const char* InitCustomPlayerSlots;			//1?
	bool processedInitCustomTeams;
		static const char* InitCustomTeams;					//1?

	bool processedmain;
	bool inMain;
		static const char* main;							//1;
			static const char* ExecuteFunc;					//multiple
		bool processedconfig;
			static const char* config;						//1

	bool inFunction;
		static const char* endfunction;						//per, 1

	// BEGIN IMPORT OF E:\\Program Files (x86)\\Warcraft III\\jassnewgenpack5d\\jass\\vJASS.j						//multiple?
	static const char* import;
	static const char* import2;

	Keywords() {
		inGlobals = false;
		processedGlobals = false;
		processedInitCustomTriggers = false;
		processedRunInitializationTriggers = false;
		processedInitCustomPlayerSlots = false;
		processedInitCustomTeams = false;
		processedmain = false;
		inMain = false;
		processedconfig = false;
		inFunction = false;
		processedInitGlobals = false;
	}
};