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

#include "common/xz/codec.h"
#include "common/xz/lzma.h"
#include <string.h>

namespace xz {

bool Codec::HasLZMASupport() {
#if defined(__LZMA__)
    return true;
#else
    return false;
#endif // __LZMA__
}

bool Codec::IsLZMA(uint8_t *data) {
    return !memcmp(data, LZMA::kMagic, 6);
}

std::unique_ptr<Codec> Codec::Create(uint8_t *data, uint64_t size) {
    std::unique_ptr<Codec> impl;
#if defined(__LZMA__)
    if (Codec::IsLZMA(data))
        impl = std::make_unique<LZMA>(data, size);
#endif // __LZMA__
    return std::move(impl);
}

} // xz
