// Copyright 2021 Howard Lau
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package board.basys3

import chisel3._
import chisel3.experimental.ChiselEnum
import chisel3.stage.{ChiselGeneratorAnnotation, ChiselStage}
import chisel3.util._
import peripheral.{CharacterDisplay, Dummy, InstructionROM, Memory, ROMLoader, VGADisplay}
import riscv._
import riscv.core.{CPU, ProgramCounter}

object BootStates extends ChiselEnum {
  val Init, Loading, Finished = Value
}

class Top extends Module {
  val binaryFilename = "sb.asmbin"
  val io = IO(new Bundle {
    val switch = Input(UInt(16.W))

    val segs = Output(UInt(8.W))
    val digit_mask = Output(UInt(4.W))

    val hsync = Output(Bool())
    val vsync = Output(Bool())
    val rgb = Output(UInt(12.W))
    val led = Output(UInt(16.W))

  })
  val vga_display = Module(new VGADisplay)
  val display = Module(new CharacterDisplay)

  val mem = Module(new Memory(Parameters.MemorySizeInWords))
  val dummy = Module(new Dummy)

  display.io.bundle <> dummy.io.bundle
  mem.io.bundle <> dummy.io.bundle
  mem.io.debug_read_address := 0.U

  val instruction_rom = Module(new InstructionROM(binaryFilename))
  val rom_loader = Module(new ROMLoader(instruction_rom.capacity))

  rom_loader.io.rom_data := instruction_rom.io.data
  rom_loader.io.load_address := Parameters.EntryAddress
  instruction_rom.io.address := rom_loader.io.rom_address

  val CPU_clkdiv = RegInit(UInt(2.W), 0.U)
  val CPU_tick = Wire(Bool())
  val CPU_next = Wire(UInt(2.W))
  CPU_next := Mux(CPU_clkdiv === 3.U, 0.U, CPU_clkdiv + 1.U)
  CPU_tick := CPU_clkdiv === 0.U
  CPU_clkdiv := CPU_next

  withClock(CPU_tick.asClock) {
    val cpu = Module(new CPU)
    cpu.io.debug_read_address := 0.U
    cpu.io.instruction_valid := rom_loader.io.load_finished
    mem.io.instruction_address := cpu.io.instruction_address
    cpu.io.instruction := mem.io.instruction

    when(!rom_loader.io.load_finished) {
      rom_loader.io.bundle <> mem.io.bundle
      cpu.io.memory_bundle.read_data := 0.U
    }.otherwise {
      rom_loader.io.bundle.read_data := 0.U
      when(cpu.io.deviceSelect === 1.U) {
        cpu.io.memory_bundle <> display.io.bundle
      }.otherwise {
        cpu.io.memory_bundle <> mem.io.bundle
      }
    }
  }
  io.hsync := vga_display.io.hsync
  io.vsync := vga_display.io.vsync

  display.io.x := vga_display.io.x
  display.io.y := vga_display.io.y
  display.io.video_on := vga_display.io.video_on

  io.rgb := display.io.rgb

  mem.io.debug_read_address := io.switch(15, 1).asUInt << 2
  io.led := Mux(
    io.switch(0),
    mem.io.debug_read_data(31, 16).asUInt,
    mem.io.debug_read_data(15, 0).asUInt,
  )

  val onboard_display = Module(new OnboardDigitDisplay)
  io.digit_mask := onboard_display.io.digit_mask

  val sysu_logo = Module(new SYSULogo)
  sysu_logo.io.digit_mask := io.digit_mask

  val seg_mux = Module(new SegmentMux)
  seg_mux.io.digit_mask := io.digit_mask
  seg_mux.io.numbers := io.led

  io.segs := MuxLookup(
    io.switch,
    seg_mux.io.segs,
    IndexedSeq(
      0.U -> sysu_logo.io.segs
    )
  )
}

object VerilogGenerator extends App {
  (new ChiselStage).execute(Array("-X", "verilog", "-td", "verilog/basys3"), Seq(ChiselGeneratorAnnotation(() => new Top)))
}