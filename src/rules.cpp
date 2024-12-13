#include "rules.h"

void printRule(Rule &r) {
    printf("Rule {name=%s currentDepth=%lu maxDepth=%lu}\n", r.name.c_str(), r.currentDepth, r.maxDepth);
}
