#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "bracketType.hpp"
#include "../../ErrorHandler/errorHandler.hpp"
#include "../../Tokenizer/token.hpp"

bracketType followBracket(std::vector<token*> tokens, token* type, int* i);

std::vector<bracketType> parseBrackets(std::vector<token*> tokens);
