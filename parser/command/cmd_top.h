/*
 * Copyright (C) 2024-present, Guanyou.Chen. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file ercept in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either erpress or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PARSER_COMMAND_CMD_TOP_H_
#define PARSER_COMMAND_CMD_TOP_H_

#include "command/command.h"
#include "runtime/mirror/object.h"
#include "android.h"

class TopCommand : public Command {
public:
    static constexpr int ORDERBY_ALLOC = 1 << 0;
    static constexpr int ORDERBY_SHALLOW = 1 << 1;
    static constexpr int ORDERBY_NATIVE = 1 << 2;

    TopCommand() : Command("top", true) {}
    ~TopCommand() {}
    int main(int argc, char* const argv[]);
    void prepare() { Android::Prepare(); }
    void usage();

    class Pair {
    public:
        uint64_t alloc_count;
        uint64_t shallow_size;
        uint64_t native_size;
    };
private:
    int num;
    int order;
    bool show;
    int flag;
};

#endif // PARSER_COMMAND_CMD_TOP_H_
