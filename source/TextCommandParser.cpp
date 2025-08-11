#include "TextCommandParser.h"
#include "InteractionWindows.h"

TextCommandParser::TextCommandParser(std::string textAI) : CommandAI{} {
	FormatingTextInCommandAI(textAI);
}

// 1) Сначала мы разделяем весь текст на массив слов и присваиваем его в локальный массив данной функции
// 2) Если модель думающая то убираем текст помеченный словами (<think> ... <think>)
// 3) В исходном тексте находим команды и присваиваем их в массив CommandAI.
void TextCommandParser::FormatingTextInCommandAI(std::string textAI)  {
	std::vector<std::string> ArrayText; // Отсортированные массив

	for (int i = 0, j = 0; i < textAI.length(); ++i) {
		if (iswspace(textAI[i]) || textAI[i] == ',' || textAI[i] == '!' || textAI[i] == '.' || textAI[i] == '?') {
			std::string word;
			for (int g = j; g < i; ++g) {
				word += textAI[g];
			}
			ArrayText.push_back(word);
			j = i + 1;
		}
	}

	// Находим конец в участке текста где нейросеть думаеть перед ответом и помечаем его конец значением END.
	int END = 0;
	for (int i = 0; i < ArrayText.size(); ++i) {
		if (ArrayText[i] == "</think>") {
			END = i;
		}
	}

	for (int i = END; i < ArrayText.size(); ++i) {
		if ((ArrayText[i] == "/volume") || (ArrayText[i] == "/vividness") || (ArrayText[i] == "/mode")) {
			CommandAI.push_back(ArrayText[i]);
			CommandAI.push_back(ArrayText[i + 1]);
		}
	}

	InteractrionWindows inter(CommandAI);
}


