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

#include "logger/log.h"
#include "api/core.h"
#include "riscv64/unwind.h"
#include "common/ucontext.h"
#include "common/exception.h"
#include <string.h>

namespace riscv64 {

uint64_t UnwindStack::GetUContext() {
    return cur_uc_;
}

void UnwindStack::WalkStack() {
    ThreadInfo* thread = reinterpret_cast<ThreadInfo*>(GetThread());
    Register& regs = thread->GetRegs();
    cur_frame_pc_ = regs.pc;
    VisitFrame();
}

void UnwindStack::DumpContextRegister(const char* prefix) {
    api::MemoryRef uc = GetUContext();
    if (uc.Ptr()) {
        struct ucontext* context = (struct ucontext*)uc.Real();
        Register uc_regs;
        memcpy(&uc_regs, &context->uc_mcontext.sc_regs, sizeof(Register));
        uc_regs.Dump(prefix);
    }
}

} // namespace riscv64
