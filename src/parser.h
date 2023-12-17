#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "rules.h"

std::vector<Rule> parse(Scanner &scanner);

#endif
