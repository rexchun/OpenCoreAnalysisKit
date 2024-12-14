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

#ifndef PARSER_COMMAND_FAKE_MAP_FAKEMAP_H_
#define PARSER_COMMAND_FAKE_MAP_FAKEMAP_H_

#include "common/link_map.h"

class FakeLinkMap {
public:
    static int OptionMap(int argc, char* const argv[]);
    static bool FakeLD64(LinkMap* map);
    static bool FakeLD32(LinkMap* map);
    static void Usage();
};

#endif // PARSER_COMMAND_FAKE_MAP_FAKEMAP_H_
