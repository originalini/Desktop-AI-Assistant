#include "Utility.h"

int myUtility::GetNumberInString(std::string StrNum) {
	int number = 0;

	for (int g = 0; g < StrNum.length(); ++g) {
		if (g == 0) {
			for (int h = 48; h <= 59; ++h) {
				if (h == StrNum[g]) {
					number += h - 48;
				}
			}
		}
		if (g >= 1) {
			for (int h = 48; h <= 59; ++h) {
				if (h == StrNum[g]) {
					number = number * 10;
					number += h - 48;
				}
			}
		}
	}

	return number;
}

