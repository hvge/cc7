#
# Copyright 2016 Juraj Durech <durech.juraj@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# cc7 is using LLVM's STL libraries to be compatible with Apple's clang
#
APP_STL := c++_static
#
# List of supported ABIs. Note that if you change this, then you should
# probably also validate platforms, supported by the OpenSSL build system.
#
APP_ABI := armeabi-v7a arm64-v8a x86 x86_64