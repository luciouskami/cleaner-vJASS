#include <iostream>
#include <stdint.h>
#include <list>

#include "StormLib.h"

#include "keywords.hpp"
#include "Script.hpp"
#include "StrLine.hpp"

using namespace std;

bool comparePartial(const char* word1, const char* word2) {
	size_t i = 0;
	size_t i2 = 0;

	//find the start
	while (word2[i2] != 0 && word2[i2] != '*') {
		i2 = 0;

		for (; word1[i] != 0 && word1[i] != '=' && word1[i] != word2[0]; ++i);
		while (word1[i] != 0 && word1[i] != '=' && word2[i2] != 0 && word1[i] == word2[i2]) { ++i; ++i2; }
		if (word1[i] == '=' || (word1[i] == 0 && word2[i2] != 0 && word2[i2] != '*')) { return false; }
	}

	return true;
}

size_t compareFull(const char* word1, const char* word2) {
	size_t i = 0;

	for (; word1[i] != 0 && word2[i] != 0 && word1[i] == word2[i]; ++i);

	return word2[i] == 0 || word2[i] == '*';

	return 0;
}

inline void readLine(Script& input, StrLine& line) {
	line.pos = 0;

	while (input.dat[input.pos] == ' ' || input.dat[input.pos] == '\t' || input.dat[input.pos] == '\r' || input.dat[input.pos] == '\n') {
		++input.pos;
	}
	while (input.dat[input.pos] != '\n' && input.dat[input.pos] != 0) {
		line.dat[line.pos++] = input.dat[input.pos++];
	}
	line.dat[line.pos++] = '\n';
	line.dat[line.pos] = 0;

	line.pos = 0;
}

void str_find(StrLine& line, const char* keyword) {
	size_t i = 0;

	for (; line.dat[i] != 0 && keyword[i] != 0 && line.dat[i] == keyword[i]; ++i);

	if (keyword[i] == 0 || keyword[i] == '*') {
		line.pos = i;
	}
}

bool checkGlobals(StrLine& line, Script& output, Keywords& keywords) {
	if (!keywords.processedGlobals) {
		if (keywords.inGlobals) {
			if (compareFull(line.dat, keywords.endglobals)) {
				keywords.processedGlobals = true;
				keywords.inGlobals = false;
				output.concat(line.dat);
			}
			else if (comparePartial(line.dat, keywords.gg_) || comparePartial(line.dat, keywords.udg_)) {
				output.concat(line.dat);
			}

			return true;
		}
		else if (compareFull(line.dat, keywords.globals)) {
			output.concat(line.dat);
			keywords.inGlobals = true;

			return true;
		}
	}
	else if (!keywords.processedInitGlobals) {
		if (compareFull(line.dat, keywords.InitGlobals)) {
			output.concat(line.dat);
			keywords.processedInitGlobals = true;
			keywords.inFunction = true;
		}

		return true;
	}

	return false;
}

bool checkFunc(StrLine& line, Script& output, Keywords& keywords) {
	if (keywords.inFunction) {
		keywords.inFunction = !compareFull(line.dat, keywords.endfunction);
		keywords.inMain = keywords.inMain && keywords.inFunction;

		if (!keywords.inMain || !compareFull(line.dat, keywords.ExecuteFunc)) {
			output.concat(line.dat);
		}

		return true;
	}

	return false;
}

bool checkTriggers(StrLine& line, Script& output, Keywords& keywords) {
	if (!keywords.processedInitCustomPlayerSlots) {
		if (!keywords.processedInitCustomTriggers) {
			if (compareFull(line.dat, keywords.Trig_) || compareFull(line.dat, keywords.InitTrig_)) {
				output.concat(line.dat);
				keywords.inFunction = true;
			}
			else if (compareFull(line.dat, keywords.InitCustomTriggers)) {
				keywords.processedInitCustomTriggers = true;
				output.concat(line.dat);
				keywords.inFunction = true;
			}
		}
		else if (!keywords.processedRunInitializationTriggers && compareFull(line.dat, keywords.RunInitializationTriggers)) {
			keywords.processedRunInitializationTriggers = true;
			output.concat(line.dat);
			keywords.inFunction = true;
		}
	}

	return keywords.inFunction;
}

bool checkComment(StrLine& line) {
	return line.dat[0] == '/';
}

bool checkInitCustomPlayerSlots(StrLine& line, Script& output, Keywords& keywords) {
	if (!keywords.processedInitCustomPlayerSlots && compareFull(line.dat, keywords.InitCustomPlayerSlots)) {
		keywords.processedInitCustomPlayerSlots = true;
		keywords.inFunction = true;
		output.concat(line.dat);

		return true;
	}

	return false;
}

bool checkInitCustomTeams(StrLine& line, Script& output, Keywords& keywords) {
	if (!keywords.processedInitCustomTeams && compareFull(line.dat, keywords.InitCustomTeams)) {
		keywords.processedInitCustomTeams = true;
		keywords.inFunction = true;
		output.concat(line.dat);

		return true;
	}

	return false;
}

bool checkMain(StrLine& line, Script& output, Keywords& keywords) {
	if (!keywords.processedmain && compareFull(line.dat, keywords.main)) {
		keywords.processedmain = true;
		keywords.inFunction = true;
		keywords.inMain = true;
		output.concat(line.dat);

		return true;
	}

	return false;
}

bool checkConfig(StrLine& line, Script& output, Keywords& keywords) {
	if (!keywords.processedconfig && compareFull(line.dat, keywords.config)) {
		keywords.processedconfig = true;
		keywords.inFunction = true;
		output.concat(line.dat);

		return true;
	}

	return false;
}

Script* cleanScript(Script& input, const DWORD FILE_SIZE) {
	Keywords keywords;
	Script& output = *new Script(FILE_SIZE);

	StrLine line;

	while (input.dat[input.pos] != 0) {
		readLine(input, line);

		if (checkComment(line));
		else if (checkFunc(line, output, keywords));
		else if (checkGlobals(line, output, keywords));
		else if (checkTriggers(line, output, keywords));
		else if (checkInitCustomPlayerSlots(line, output, keywords));
		else if (checkInitCustomTeams(line, output, keywords));
		else if (checkMain(line, output, keywords));
		else if (checkConfig(line, output, keywords));
	}

	return &output;
}

list<TCHAR*>* retrieveArgs(const int argc, TCHAR* argv []) {
	list<TCHAR*>& args = *new list<TCHAR*>();

	for (int i = 2; i < argc; ++i) {
		if (argv[i][0] == '-' && argv[i][1] == 'i') {
			args.push_back(argv[++i]);
		}
	}

	return &args;
}

int main(int argc, TCHAR* argv []) {
	if (argc == 1) {
		cout << "Expected Map Name" << endl;
		return 0;
	}

	HANDLE mpq = NULL;
	HANDLE file = NULL;

	if (SFileOpenArchive(argv[1], NULL, 0, &mpq)) {
		LCID id = NULL;

		if (SFileOpenFileEx(mpq, "war3map.j", SFILE_OPEN_FROM_MPQ, &file)) {
			const DWORD FILE_SIZE = SFileGetFileSize(file, NULL);

			Script input(FILE_SIZE);

			SFileReadFile(file, input.dat, FILE_SIZE, &input.bytes, NULL);
			SFileCloseFile(file);

			Script& output = *cleanScript(input, FILE_SIZE);

			list<TCHAR*>& args = *retrieveArgs(argc, argv);
			for (char* str : args) {
				output.concat("//! import \"");
				output.concat(str);
				output.concat("\"\n");
			}
			delete &args;

			if (SFileCreateFile(mpq, "war3map.j", 0, output.pos, id, MPQ_FILE_REPLACEEXISTING | MPQ_FILE_COMPRESS, &file)) {
				SFileWriteFile(file, output.dat, output.pos, MPQ_COMPRESSION_HUFFMANN);
				SFileFinishFile(file);

				cout << "Map Cleaned of vJASS" << endl;
			}

			delete &output;
		}

		SFileCompactArchive(mpq, NULL, false);
		SFileCloseArchive(mpq);
	}

	return 0;
}