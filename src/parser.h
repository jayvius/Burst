#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "rules.h"

void parse(Scanner &scanner, std::vector<Rule> &rules, size_t &axiomRuleIndex);

#endif
