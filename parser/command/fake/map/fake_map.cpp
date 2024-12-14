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

#include "logger/log.h"
#include "api/core.h"
#include "common/bit.h"
#include "command/fake/map/fake_map.h"
#include <linux/elf.h>
#include <unistd.h>
#include <getopt.h>
#include <memory>

int FakeLinkMap::OptionMap(int argc, char* const argv[]) {
    if (!CoreApi::IsReady())
        return 0;

    int opt;
    int option_index = 0;
    optind = 0; // reset
    static struct option long_options[] = {
        {"ld",         no_argument,       0, 1},
    };

    bool fake_ld = false;
    while ((opt = getopt_long(argc, argv, "a",
                long_options, &option_index)) != -1) {
        switch(opt) {
            case 1:
                fake_ld = true;
                break;
        }
    }

    if (fake_ld) {
        auto callback = [&](LinkMap* map) -> bool {
            if (CoreApi::Bits() == 64)
                FakeLinkMap::FakeLD64(map);
            else
                FakeLinkMap::FakeLD32(map);
            return false;
        };
        CoreApi::ForeachLinkMap(callback);
    }
    return 0;
}

bool FakeLinkMap::FakeLD64(LinkMap* map) {
    LoadBlock* block = map->block();
    if (!block || !block->isMmapBlock())
        return false;

    Elf64_Ehdr *ehdr = reinterpret_cast<Elf64_Ehdr *>(block->begin(Block::OPT_READ_MMAP));
    Elf64_Phdr *phdr = reinterpret_cast<Elf64_Phdr *>(block->begin(Block::OPT_READ_MMAP) + ehdr->e_phoff);

    for (int num = 0; num < ehdr->e_phnum; ++num) {
        if (phdr[num].p_type == PT_PHDR) {
            if (phdr[num].p_offset != phdr[num].p_vaddr) {
                uint64_t vaddr = map->l_addr() + phdr[num].p_offset - phdr[num].p_vaddr;
                CoreApi::Write(map->Ptr() + OFFSET(LinkMap, l_addr), vaddr);
                LOGI("calibrate %s l_addr(%lx)\n", map->name(), vaddr);
            }
            continue;
        }

        if (phdr[num].p_type == PT_DYNAMIC) {
                uint64_t vaddr = map->l_addr() + phdr[num].p_vaddr;
                CoreApi::Write(map->Ptr() + OFFSET(LinkMap, l_ld), vaddr);
                LOGI("calibrate %s l_ld(%lx)\n", map->name(), vaddr);
            break;
        }
    }
    return true;
}

bool FakeLinkMap::FakeLD32(LinkMap* map) {
    return false;
}

void FakeLinkMap::Usage() {
    LOGI("Usage: fake map [OPTION]\n");
    LOGI("Option:\n");
    LOGI("    --ld     calibrate link_map l_addr and l_ld\n");
    ENTER();
    LOGI("core-parser> fake map --ld\n");
    LOGI("calibrate /apex/com.android.art/lib64/libart.so l_ld(7d5f20e8f8)\n");
    LOGI("calibrate /apex/com.android.art/lib64/libunwindstack.so l_ld(7d619fcb38)\n");
    LOGI("calibrate /apex/com.android.runtime/lib64/bionic/libc.so l_ld(7e0c7762e8)\n");
}
