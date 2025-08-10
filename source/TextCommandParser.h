#pragma once

#include "Window.h"

class TextCommandParser {
private:
	std::vector<std::string> CommandAI;

	void FormatingTextInCommandAI(std::string textAI); // Форматирование текста в массив команд 

public:
	TextCommandParser(std::string textAI);

};