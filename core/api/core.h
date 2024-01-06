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

#ifndef CORE_API_CORE_H_
#define CORE_API_CORE_H_

#include "base/memory_map.h"
#include "common/load_block.h"
#include "common/note_block.h"
#include <stdint.h>
#include <sys/types.h>
#include <memory>
#include <vector>
#include <string>

/*
             ---------- <-
            |          |  \
          --| Program  |   \
         |  | 1 Header |   |
         |  |          |   |
         |   ----------    |
         |  |          |   |        ----------
       --|--| Program  |   |       |          |
      |  |  | 2 Header |   |       |ELF Header|
      |  |  |          |   \       |          |
      |  |   ----------     ------- ----------       ----> ----------
      |  |  |**********|           |          |     |     |          |
      |  |  |**********|           |          |     |     | Thread 1 |
      |  |  |**********|           |  Program |     |     | Registers|
      |  |  |**********|           |  Headers |     |     |          |
      |  |  |**********|           |          |     |      ----------
      |  |  |**********|           |          |    /      |          |
      |  |  |**********|           |          |   /       | Thread 2 |
      |  |   ----------            |          |  /        | Registers|
      |  |  |          |     ------ ---------- --         |          |
    --|--|--| Program  |    /      |          |            ----------
   |  |  |  | N Header |   |     ->|  Segment |           |**********|
   |  |  |  |          |   /    |  | (PT_NOTE)|           |**********|
   |  |  |   ---------- <--   --   |          |           |**********|
   |  |  |                   |      ---------- --         |**********|
   |  |   -------------------      |          |  \         ----------
   |  |                            |  Segment |   \       |          |
   |   --------------------------->| (PT_LOAD)|    |      | Thread N |
   |                               |          |    |      | Registers|
   |                                ----------     |      |          |
   |                               |          |    |       ----------
   |                               |  Segment |    \      |          |
   |                               | (PT_LOAD)|     \     |   AUXV   |
   |                               |          |      \    |          |
   |                                ----------        ---> ----------
   |                               |**********|
   |                               |**********|
   |                               |**********|
   |                               |**********|
   |                               |**********|
   |                                ----------
   |                               |          |
    ------------------------------>|  Segment |
                                   | (PT_LOAD)|
                                   |          |
                                    ----------
*/

class CoreApi {
public:
    static bool Load(const char* corefile);
    static void UnLoad();
    static const char* GetMachine();
    static int GetPointSize();
    static uint64_t GetReal(uint64_t vaddr);
    static uint64_t GetVirtual(uint64_t raddr);
    CoreApi() {}
    CoreApi(std::unique_ptr<MemoryMap>& map) {
        mCore = std::move(map);
    }
    virtual ~CoreApi();
    uint64_t begin();
    uint64_t size();
    std::string& getName();
    void addLoadBlock(std::shared_ptr<LoadBlock>& block);
    void removeAllLoadBlock();
    inline uint64_t v2r(uint64_t vaddr);
    inline uint64_t r2v(uint64_t raddr);
    void addNoteBlock(std::unique_ptr<NoteBlock>& block);
    void removeAllNoteBlock();
private:
    static CoreApi* INSTANCE;
    virtual bool load() = 0;
    virtual void unload() = 0;
    virtual const char* getMachine() = 0;
    virtual int getPointSize() = 0;

    std::unique_ptr<MemoryMap> mCore;
    std::vector<std::shared_ptr<LoadBlock>> mLoad;
    std::vector<std::unique_ptr<NoteBlock>> mNote;
};

#endif // CORE_API_CORE_H_
