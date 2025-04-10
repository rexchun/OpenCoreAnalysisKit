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

#include "runtime/art_field.h"
#include "dex/descriptors_names.h"

struct ArtField_OffsetTable __ArtField_offset__;
struct ArtField_SizeTable __ArtField_size__;

namespace art {

void ArtField::Init() {
    __ArtField_offset__ = {
        .declaring_class_ = 0,
        .access_flags_ = 4,
        .field_dex_idx_ = 8,
        .offset_ = 12,
    };

    __ArtField_size__ = {
        .THIS = 16,
    };
}

const char* ArtField::GetTypeDescriptor(const char* def) {
    if (!GetDeclaringClass().Ptr()) return def;
    uint32_t field_index = GetDexFieldIndex();
    if (IsProxyField()) {
        return field_index == 0x0 ? "[Ljava/lang/Class;" : "[[Ljava/lang/Class;";
    }
    DexFile& dex_file = GetDexFile();
    dex::FieldId id = dex_file.GetFieldId(field_index);
    return dex_file.GetFieldTypeDescriptor(id);
}

std::string ArtField::PrettyTypeDescriptor() {
    std::string tmp;
    AppendPrettyDescriptor(GetTypeDescriptor(), &tmp, "B");
    return tmp;
}

const char* ArtField::GetName(const char* def) {
    if (!GetDeclaringClass().Ptr()) return def;
    int32_t field_index = GetDexFieldIndex();
    if (IsProxyField()) {
        return field_index == 0 ? "interfaces" : "throws";
    }
    DexFile& dex_file = GetDexFile();
    dex::FieldId id = dex_file.GetFieldId(field_index);
    return dex_file.GetFieldName(id);
}

} // namespace art
