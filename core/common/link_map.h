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

#ifndef CORE_COMMON_LINKMAP_H_
#define CORE_COMMON_LINKMAP_H_

#include "api/memory_ref.h"
#include <string>
#include <unordered_set>

struct LinkMap_OffsetTable {
    uint32_t l_addr;
    uint32_t l_name;
    uint32_t l_ld;
    uint32_t l_next;
    uint32_t l_prev;
};

struct LinkMap_SizeTable {
    uint32_t THIS;
};

extern struct LinkMap_OffsetTable __LinkMap_offset__;
extern struct LinkMap_SizeTable __LinkMap_size__;

class LinkMap : public api::MemoryRef {
public:
    LinkMap(uint64_t m) : api::MemoryRef(m) {
        ReadDynsyms();
    }
    ~LinkMap() { dynsyms.clear(); }
    static void Init();
    inline uint64_t l_addr() { return VALUEOF(LinkMap, l_addr); }
    inline uint64_t l_name() { return VALUEOF(LinkMap, l_name); }
    inline uint64_t l_ld() { return VALUEOF(LinkMap, l_ld); }

    inline uint64_t map() { return Ptr(); }
    uint64_t begin();
    const char* name();
    LoadBlock* block();

    class NiceSymbol {
    public:
        NiceSymbol() : off(0), size(0) {}
        void SetNiceMethod(const char* sym, uint64_t o, uint64_t s) {
            sym ? symbol = sym : "";
            off = o;
            size = s;
        }
        std::string& GetSymbol() { return symbol; }
        std::string& GetMethod();
        uint64_t GetOffset() { return off; }
        uint64_t GetSize() { return size; }
        bool IsValid() { return off && size; }
        static NiceSymbol Invalid() { return NiceSymbol(); }
    private:
        std::string symbol;
        std::string method;
        uint64_t off;
        uint64_t size;
    };
    void ReadSymbols();
    void ReadDynsyms();
    void NiceMethod(uint64_t pc, NiceSymbol& symbol);
    SymbolEntry DlSymEntry(const char* symbol);
    SymbolEntry DlRegionSymEntry(uint64_t addr);
    inline uint64_t DlSym(const char* symbol) { return DlSymEntry(symbol).offset; }
    api::MemoryRef& GetAddrCache();
    api::MemoryRef& GetNameCache();
    inline std::unordered_set<SymbolEntry, SymbolEntry::Hash>& GetDynsyms() { return dynsyms; }
    std::unordered_set<SymbolEntry, SymbolEntry::Hash>& GetCurrentSymbols();
private:
    api::MemoryRef addr_cache = 0x0;
    api::MemoryRef name_cache = 0x0;
    std::unordered_set<SymbolEntry, SymbolEntry::Hash> dynsyms;
};

#endif  // CORE_COMMON_LINKMAP_H_
