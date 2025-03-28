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

#ifndef ANDROID_ART_RUNTIME_JNI_LOCAL_REFERENCE_TABLE_H_
#define ANDROID_ART_RUNTIME_JNI_LOCAL_REFERENCE_TABLE_H_

#include "api/memory_ref.h"
#include "runtime/mirror/object.h"
#include <functional>

struct LocalReferenceTable_OffsetTable {
    uint32_t previous_state_;
    uint32_t segment_state_;
    uint32_t small_table_;
    uint32_t tables_;
};

extern struct LocalReferenceTable_OffsetTable __LocalReferenceTable_offset__;

struct LrtEntry_OffsetTable {
    uint32_t root_;
};

struct LrtEntry_SizeTable {
    uint32_t THIS;
};

extern struct LrtEntry_OffsetTable __LrtEntry_offset__;
extern struct LrtEntry_SizeTable __LrtEntry_size__;

namespace art {
namespace jni {

static uint32_t kInitialLrtBytes = 512;
static uint32_t kSmallLrtEntries = kInitialLrtBytes / 4;

class LrtEntry : public api::MemoryRef {
public:
    LrtEntry(uint64_t v) : api::MemoryRef(v) {}
    LrtEntry(const api::MemoryRef& ref) : api::MemoryRef(ref) {}
    LrtEntry(uint64_t v, api::MemoryRef& ref) : api::MemoryRef(v, ref) {}
    LrtEntry(uint64_t v, api::MemoryRef* ref) : api::MemoryRef(v, ref) {}

    static void Init();
    inline uint32_t root() { return value32Of(OFFSET(LrtEntry, root_)); }
};

class LocalReferenceTable : public api::MemoryRef {
public:
    LocalReferenceTable(uint64_t v) : api::MemoryRef(v) {}
    LocalReferenceTable(const api::MemoryRef& ref) : api::MemoryRef(ref) {}
    LocalReferenceTable(uint64_t v, api::MemoryRef& ref) : api::MemoryRef(v, ref) {}
    LocalReferenceTable(uint64_t v, api::MemoryRef* ref) : api::MemoryRef(v, ref) {}

    static void Init();
    static void Init34();
    static void Init35();
    inline uint32_t segment_state() { return value32Of(OFFSET(LocalReferenceTable, segment_state_)); }
    inline uint64_t small_table() { return VALUEOF(LocalReferenceTable, small_table_); }
    inline uint64_t tables() { return Ptr() + OFFSET(LocalReferenceTable, tables_); }

    mirror::Object DecodeReference(uint64_t iref);

    void Walk(std::function<bool (mirror::Object& object)> fn);
    void Walk(std::function<bool (mirror::Object& object, uint64_t iref)> fn);
};

} // namespace jni
} // namespace art

#endif  // ANDROID_ART_RUNTIME_JNI_LOCAL_REFERENCE_TABLE_H_
