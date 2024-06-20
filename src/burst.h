#ifndef BURST_H
#define BURST_H

#include <string>
#include <variant>
#include <vector>
#include <memory>
#include <mutex>
#include "ofxVboAppender.h"
#include "server.h"
#include "rules.h"
#include "opcodes.h"

void run(std::string src, ofxVboAppender &ofxVboAppender, std::mutex &updateMutex);

#endif
