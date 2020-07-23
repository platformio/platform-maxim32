/* Copyright (c) 2018 Arm Limited
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "mbed.h"
#include <inttypes.h>

#if !defined(MBED_SYS_STATS_ENABLED)
#error [NOT_SUPPORTED] System statistics not supported
#endif

int main()
{
    mbed_stats_sys_t stats;
    mbed_stats_sys_get(&stats);

    printf("Mbed OS Version: %" PRId32 "\n", stats.os_version);

    /* CPUID Register information
    [31:24]Implementer      0x41 = ARM
    [23:20]Variant          Major revision 0x0  =  Revision 0
    [19:16]Architecture     0xC  = Baseline Architecture
                            0xF  = Constant (Mainline Architecture)
    [15:4]PartNO            0xC20 =  Cortex-M0
                            0xC60 = Cortex-M0+
                            0xC23 = Cortex-M3
                            0xC24 = Cortex-M4
                            0xC27 = Cortex-M7
                            0xD20 = Cortex-M23
                            0xD21 = Cortex-M33
    [3:0]Revision           Minor revision: 0x1 = Patch 1
    */
    printf("CPU ID: 0x%" PRIx32 "\n", stats.cpu_id);

    /* Compiler IDs
        ARM     = 1
        GCC_ARM = 2
        IAR     = 3
    */
    printf("Compiler ID: %d \n", stats.compiler_id);

    /* Compiler versions:
       ARM: PVVbbbb (P = Major; VV = Minor; bbbb = build number)
       GCC: VVRRPP  (VV = Version; RR = Revision; PP = Patch)
       IAR: VRRRPPP (V = Version; RRR = Revision; PPP = Patch)
    */
    printf("Compiler Version: %" PRId32 "\n", stats.compiler_version);

    /* RAM / ROM memory start and size information */
    for (int i = 0; i < MBED_MAX_MEM_REGIONS; i++) {
        if (stats.ram_size[i] != 0) {
            printf("RAM%d: Start 0x%" PRIx32 " Size: 0x%" PRIx32 "\n", i, stats.ram_start[i], stats.ram_size[i]);
        }
    }
    for (int i = 0; i < MBED_MAX_MEM_REGIONS; i++) {
        if (stats.rom_size[i] != 0) {
            printf("ROM%d: Start 0x%" PRIx32 " Size: 0x%" PRIx32 "\n", i, stats.rom_start[i], stats.rom_size[i]);
        }
    }
    return 0;
}
