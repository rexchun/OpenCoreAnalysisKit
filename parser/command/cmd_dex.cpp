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
#include "command/cmd_dex.h"
#include "api/core.h"
#include "android.h"
#include "runtime/runtime.h"
#include "runtime/class_linker.h"
#include <string>

int DexCommand::main(int argc, char* const argv[]) {
    if (!CoreApi::IsReady() || !Android::IsSdkReady())
        return 0;

    art::Runtime& runtime = art::Runtime::Current();
    art::ClassLinker& linker = runtime.GetClassLinker();
    for (const auto& value : linker.GetDexCacheDatas()) {
        art::mirror::DexCache& dex_cache = value->GetDexCache();
        if (dex_cache.IsDexCache()) {
            LOGI("%s\n", dex_cache.GetLocation().ToModifiedUtf8().c_str());
        }
    }

    return 0;
}

void DexCommand::usage() {
    LOGI("Usage: dex\n");
}
