# Copyright (C) 2024-present, Guanyou.Chen. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake -DCMAKE_C_COMPILER=$BUILD_HOST_C_COMPILER \
      -DCMAKE_CXX_COMPILER=$BUILD_HOST_CXX_COMPILER \
      -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      3rd-party/capstone/CMakeLists.txt \
      -B $INSTALL_OUTPUT/linux/3rd-party/capstone

make -C $INSTALL_OUTPUT/linux/3rd-party/capstone -j8
