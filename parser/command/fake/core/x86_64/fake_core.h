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

#ifndef PARSER_COMMAND_FAKE_CORE_X86_64_FAKECORE_IMPL_H_
#define PARSER_COMMAND_FAKE_CORE_X86_64_FAKECORE_IMPL_H_

#include "command/fake/core/lp64/fake_core.h"
#include "common/prstatus.h"

namespace x86_64 {

class FakeCore : public lp64::FakeCore {
public:
    FakeCore() : lp64::FakeCore(),
                 prnum(0), prstatus(nullptr) {}
    FakeCore(std::unique_ptr<FakeCore::Stream>& in) {
        FakeCore();
        InitStream(in);
    }
    int execute(const char* output);
    int getMachine() { return EM_X86_64; }
    void CreateCorePrStatus();
    uint64_t WriteCorePrStatus(std::unique_ptr<MemoryMap>& map, uint64_t off);

    ~FakeCore();
private:
    int prnum;
    Elf64_prstatus *prstatus;
};

} // namespace x86_64

#endif // PARSER_COMMAND_FAKE_CORE_X86_64_FAKECORE_IMPL_H_
