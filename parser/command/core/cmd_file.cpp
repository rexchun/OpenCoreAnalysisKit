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
#include "command/core/cmd_file.h"
#include "base/utils.h"
#include "api/core.h"
#include <stdio.h>

int FileCommand::main(int argc, char* const argv[]) {
    if (!CoreApi::IsReady())
        return 0;

    uint64_t address = 0x0;
    if (argc > 1) address = Utils::atol(argv[1]) & CoreApi::GetVabitsMask();
    auto callback = [argc, address](File* file) -> bool {
        if (!(argc > 1)) {
            LOGI(ANSI_COLOR_CYAN "[%" PRIx64 ", %" PRIx64 ")" ANSI_COLOR_RESET "  %016" PRIx64 "  " ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET,
                    file->begin(), file->end(),
                    file->offset(), file->name().c_str());
        } else {
            if (address >= file->begin() && address < file->end()) {
                LOGI(ANSI_COLOR_CYAN "[%" PRIx64 ", %" PRIx64 ")" ANSI_COLOR_RESET "  %016" PRIx64 "  " ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET,
                        file->begin(), file->end(),
                        file->offset(), file->name().c_str());
                return true;
            }
        }
        return false;
    };

    CoreApi::ForeachFile(callback);
    return 0;
}

void FileCommand::usage() {
    LOGI("Usage: file [ADDRESS]\n");
    ENTER();
    LOGI("core-parser> file | grep app_process\n");
    LOGI("[5a224127f000, 5a2241282000)  0000000000000000  /system/bin/app_process64\n");
    LOGI("[5a2241282000, 5a2241286000)  0000000000002000  /system/bin/app_process64\n");
    LOGI("[5a2241286000, 5a2241288000)  0000000000005000  /system/bin/app_process64\n");
    LOGI("[5a2241288000, 5a2241289000)  0000000000006000  /system/bin/app_process64\n");
    ENTER();
    LOGI("core-parser> file 0x71907dc5\n");
    LOGI("[71224000, 71b89000)  0000000000203000  /system/framework/x86_64/boot-framework.oat\n");
}
