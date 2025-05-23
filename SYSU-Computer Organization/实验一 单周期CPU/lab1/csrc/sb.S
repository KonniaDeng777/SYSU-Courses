# Copyright 2021 Howard Lau
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

.globl _start
_start:
  li a0, 0x4
  li t0, 0xDEADBEEF
  sb t0, 0(a0)
  lw t1, 0(a0)
  li a1, 0x10
  li t1, 0x00006666
  sw t1, 0(a1)
  lw ra, 0(a0)
loop:
  j loop
