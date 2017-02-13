/*
 * Copyright 2016 Juraj Durech <durech.juraj@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

/**
 cc7/CC7.h
 
 This is a top level header. You can include this file from your project,
 whenever you need include everything from cc7 library at once. This is
 typically useful for precompiled headers to speedup the whole build.
 */

#include <cc7/DebugFeatures.h>
#include <cc7/Endian.h>
#include <cc7/ByteArray.h>
#include <cc7/Utilities.h>
#include <cc7/Base64.h>
#include <cc7/HexString.h>
