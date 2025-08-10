#pragma once

#include "Window.h"

class TextCommandParser {
private:
	std::vector<std::string> CommandAI;

	int GetNumberInString(std::string StrNum); // Взятие со строки число.
	void FormatingTextInCommandAI(std::string textAI); // Форматирование текста в массив команд 

public:
	TextCommandParser(std::string textAI);

};