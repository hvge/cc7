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
# Unfortunately, cc7 has to be linked against GNUs' STL libraries, 
# because support for preferred libc++ is still experimental in NDK.
# Check {NDK}/sources/cxx-stl/llvm-libc++/README.NDK for details.
#
# This is of course weird, but fact is, that r12b still doesn't support
# some features, required by cc7. I'll try to find workaround as soon
# as possible, but until then, the libstdc++ must be used.
#
APP_STL := gnustl_static

#
# This is so lame :) I can't use "all", because it still has
# profile for a "thumb" architecture, which is now considered as
# deprecated and basically doesn't work for any STL library.
#
# That makes sense of course :)
#
APP_ABI := armeabi-v7a arm64-v8a x86 x86_64 mips mips64