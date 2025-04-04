/*
 * Copyright (C) 2024-present, Guanyou.Chen. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PARSER_COMMAND_ANDROID_CMD_METHOD_H_
#define PARSER_COMMAND_ANDROID_CMD_METHOD_H_

#include "command/command.h"
#include "android.h"
#include "runtime/art_method.h"

class MethodCommand : public Command {
public:
    static constexpr int METHOD_DUMP_NAME = 1 << 0;
    static constexpr int METHOD_DUMP_DEXCODE = 1 << 1;
    static constexpr int METHOD_DUMP_OATCODE = 1 << 2;
    static constexpr int METHOD_DUMP_BINARY = 1 << 3;

    MethodCommand() : Command("method") {}
    ~MethodCommand() {}
    int main(int argc, char* const argv[]);
    bool prepare(int argc, char* const argv[]);
    void Dexdump();
    void Oatdump();
    void Binarydump();
    void usage();
private:
    art::ArtMethod method = 0x0;
    int dump_opt = METHOD_DUMP_NAME;
    int count;
    api::MemoryRef instref = 0x0;
    bool verbose;
    uint64_t pc = 0x0;
    uint64_t dexpc = 0x0;
};

#endif // PARSER_COMMAND_ANDROID_CMD_METHOD_H_
