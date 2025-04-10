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

#ifndef CORE_COMMON_DISASSEMBLE_CAPSTONE_H_
#define CORE_COMMON_DISASSEMBLE_CAPSTONE_H_

#include "api/memory_ref.h"

namespace capstone {

class Disassember {
public:
    class Option {
    public:
        static constexpr uint32_t ARCH_ARM = 0;
        static constexpr uint32_t ARCH_ARM64 = 1;
        static constexpr uint32_t ARCH_X86 = 3;
        static constexpr uint32_t ARCH_X86_64 = 3;
        static constexpr uint32_t ARCH_RISCV = 15;

        static constexpr uint32_t MODE_DEF = 0;
        static constexpr uint32_t MODE_ARM = 0;
        static constexpr uint32_t MODE_32 = 1 << 2;
        static constexpr uint32_t MODE_64 = 1 << 3;
        static constexpr uint32_t MODE_THUMB = 1 << 4;

        Option(int64_t s, uint32_t n) : start(s), num(n), force(false) {}
        void SetArchMode(uint32_t a, uint32_t m) {
            force = true;
            arch = a;
            mode = m;
        }
        bool force;
        uint32_t arch;
        uint32_t mode;

        uint64_t start;
        uint32_t num;
    };

    static void Dump(const char* prefix, uint64_t code, uint32_t size) {
        api::MemoryRef begin = code;
        Dump(prefix, begin, size);
    }
    static void Dump(const char* prefix, uint64_t code, uint32_t size, Option& opt) {
        api::MemoryRef begin = code;
        Dump(prefix, begin, size, opt);
    }
    static void Dump(const char* prefix, uint64_t code, uint32_t size, uint64_t address) {
        api::MemoryRef begin = code;
        Dump(prefix, begin, size, address);
    }
    static void Dump(const char* prefix, uint64_t code, uint32_t size, uint64_t address, Option& opt) {
        api::MemoryRef begin = code;
        Dump(prefix, begin, size, address, opt);
    }

    static void Dump(const char* prefix, api::MemoryRef& begin, uint32_t size) {
        Dump(prefix, begin, size, begin.Ptr());
    }
    static void Dump(const char* prefix, api::MemoryRef& begin, uint32_t size, Option& opt) {
        Dump(prefix, begin, size, begin.Ptr(), opt);
    }
    static void Dump(const char* prefix, api::MemoryRef& begin, uint32_t size, uint64_t address) {
        Option opt(0, -1);
        Dump(prefix, begin, size, address, opt);
    }

    static void Dump(const char* prefix, api::MemoryRef& begin, uint32_t size, uint64_t address, Option& opt);
    static void Dump(const char* prefix, uint8_t* begin, uint32_t size, uint64_t address, Option& opt);

    static void SetArmMode(const char* mode);
    static int GetArmMode() { return ARM_MODE; }
private:
    static int ARM_MODE;
};

} // namespace capstone

#endif // CORE_COMMON_DISASSEMBLE_CAPSTONE_H_
