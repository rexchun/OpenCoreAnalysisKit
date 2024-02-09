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

#ifndef ANDROID_ART_RUNTIME_GC_SPACE_LARGE_OBJECT_SPACE_H_
#define ANDROID_ART_RUNTIME_GC_SPACE_LARGE_OBJECT_SPACE_H_

#include "runtime/gc/space/space.h"

struct LargeObjectSpace_OffsetTable {
    uint32_t lock_;
    uint32_t num_bytes_allocated_;
    uint32_t num_objects_allocated_;
    uint32_t total_bytes_allocated_;
    uint32_t total_objects_allocated_;
    uint32_t begin_;
    uint32_t end_;
};

struct LargeObjectSpace_SizeTable {
    uint32_t THIS;
};

extern struct LargeObjectSpace_OffsetTable __LargeObjectSpace_offset__;
extern struct LargeObjectSpace_SizeTable __LargeObjectSpace_size__;

struct LargeObjectMapSpace_OffsetTable {
    uint32_t large_objects_;
};

struct LargeObjectMapSpace_SizeTable {
    uint32_t THIS;
};

extern struct LargeObjectMapSpace_OffsetTable __LargeObjectMapSpace_offset__;
extern struct LargeObjectMapSpace_SizeTable __LargeObjectMapSpace_size__;

struct AllocationInfo_OffsetTable {
    uint32_t prev_free_;
    uint32_t alloc_size_;
};

struct AllocationInfo_SizeTable {
    uint32_t THIS;
};

extern struct AllocationInfo_OffsetTable __AllocationInfo_offset__;
extern struct AllocationInfo_SizeTable __AllocationInfo_size__;

struct FreeListSpace_OffsetTable {
    uint32_t mem_map_;
    uint32_t allocation_info_map_;
    uint32_t allocation_info_;
    uint32_t free_end_;
    uint32_t free_blocks_;
};

struct FreeListSpace_SizeTable {
    uint32_t THIS;
};

extern struct FreeListSpace_OffsetTable __FreeListSpace_offset__;
extern struct FreeListSpace_SizeTable __FreeListSpace_size__;

namespace art {
namespace gc {
namespace space {

class LargeObjectSpace : public DiscontinuousSpace, AllocSpace {
public:
    inline static const char* FREELIST_SPACE = "free list large object space";
    inline static const char* MEMMAP_SPACE = "mem map large object space";

    LargeObjectSpace() : DiscontinuousSpace() {}
    LargeObjectSpace(uint64_t v) : DiscontinuousSpace(v) {}
    LargeObjectSpace(uint64_t v, LoadBlock* b) : DiscontinuousSpace(v, b) {}
    LargeObjectSpace(const DiscontinuousSpace& ref) : DiscontinuousSpace(ref) {}
    LargeObjectSpace(uint64_t v, DiscontinuousSpace& ref) : DiscontinuousSpace(v, ref) {}
    LargeObjectSpace(uint64_t v, DiscontinuousSpace* ref) : DiscontinuousSpace(v, ref) {}
    template<typename U> LargeObjectSpace(U *v) : DiscontinuousSpace(v) {}
    template<typename U> LargeObjectSpace(U *v, DiscontinuousSpace* ref) : DiscontinuousSpace(v, ref) {}

    static void Init();
    inline uint64_t begin() { return VALUEOF(LargeObjectSpace, begin_); }
    inline uint64_t end() { return VALUEOF(LargeObjectSpace, end_); }

    inline bool Contains(mirror::Object& object) { return begin() <= object.Ptr() && object.Ptr() < end(); }
    bool IsFreeListSpace();
    bool IsMemMapSpace();
    SpaceType GetType() { return kSpaceTypeLargeObjectSpace; }
};

class LargeObjectMapSpace : public LargeObjectSpace {
public:
    LargeObjectMapSpace() : LargeObjectSpace() {}
    LargeObjectMapSpace(uint64_t v) : LargeObjectSpace(v) {}
    LargeObjectMapSpace(uint64_t v, LoadBlock* b) : LargeObjectSpace(v, b) {}
    LargeObjectMapSpace(const LargeObjectSpace& ref) : LargeObjectSpace(ref) {}
    LargeObjectMapSpace(uint64_t v, LargeObjectSpace& ref) : LargeObjectSpace(v, ref) {}
    LargeObjectMapSpace(uint64_t v, LargeObjectSpace* ref) : LargeObjectSpace(v, ref) {}
    template<typename U> LargeObjectMapSpace(U *v) : LargeObjectSpace(v) {}
    template<typename U> LargeObjectMapSpace(U *v, LargeObjectSpace* ref) : LargeObjectSpace(v, ref) {}

    static void Init();
    void Walk(std::function<bool (mirror::Object& object)> fn);
};

class AllocationInfo : public api::MemoryRef {
public:
    AllocationInfo(uint64_t v) : api::MemoryRef(v) {}
    AllocationInfo(const api::MemoryRef& ref) : api::MemoryRef(ref) {}
    AllocationInfo(uint64_t v, api::MemoryRef* ref) : api::MemoryRef(v, ref) {}
    AllocationInfo(uint64_t v, api::MemoryRef& ref) : api::MemoryRef(v, ref) {}
    template<typename U> AllocationInfo(U *v) : api::MemoryRef(v) {}
    template<typename U> AllocationInfo(U *v, api::MemoryRef* ref) : api::MemoryRef(v, ref) {}

    static void Init();
    inline uint32_t prev_free() { return *reinterpret_cast<uint32_t*>(Real() + OFFSET(AllocationInfo, prev_free_)); }
    inline uint32_t alloc_size() { return *reinterpret_cast<uint32_t*>(Real() + OFFSET(AllocationInfo, alloc_size_)); }

    inline bool IsFree() { return (alloc_size() & kFlagFree) != 0; }
    inline uint32_t AlignSize() { return alloc_size() & kFlagsMask; }
    inline void MoveNexInfo() { MovePtr(AlignSize() * SIZEOF(AllocationInfo)); }
private:
    static constexpr uint32_t kFlagFree = 0x80000000;
    static constexpr uint32_t kFlagZygote = 0x40000000;
    static constexpr uint32_t kFlagsMask = ~(kFlagFree | kFlagZygote);
};

class FreeListSpace : public LargeObjectSpace {
public:
    FreeListSpace() : LargeObjectSpace() {}
    FreeListSpace(uint64_t v) : LargeObjectSpace(v) {}
    FreeListSpace(uint64_t v, LoadBlock* b) : LargeObjectSpace(v, b) {}
    FreeListSpace(const LargeObjectSpace& ref) : LargeObjectSpace(ref) {}
    FreeListSpace(uint64_t v, LargeObjectSpace& ref) : LargeObjectSpace(v, ref) {}
    FreeListSpace(uint64_t v, LargeObjectSpace* ref) : LargeObjectSpace(v, ref) {}
    template<typename U> FreeListSpace(U *v) : LargeObjectSpace(v) {}
    template<typename U> FreeListSpace(U *v, LargeObjectSpace* ref) : LargeObjectSpace(v, ref) {}

    static void Init();
    inline uint64_t free_end() { return VALUEOF(FreeListSpace, free_end_); }
    inline uint64_t allocation_info() { return VALUEOF(FreeListSpace, allocation_info_); }

    api::MemoryRef& GetAlloctionInfoCache();
    void Walk(std::function<bool (mirror::Object& object)> fn);
    uint64_t GetAllocationInfoForAddress(uint64_t address);
    uint64_t GetSlotIndexForAddress(uint64_t address);
    uint64_t GetAddressForAllocationInfo(AllocationInfo& info);
    uint64_t GetAllocationAddressForSlot(uint64_t slot);
private:
    // quick memoryref cache
    api::MemoryRef allocation_info_cache;
};

} // namespace space
} // namespace gc
} // namespace art

#endif // ANDROID_ART_RUNTIME_GC_SPACE_LARGE_OBJECT_SPACE_H_
