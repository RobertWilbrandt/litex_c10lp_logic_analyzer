#!/usr/bin/env python3
import argparse
import os

from litex.soc.cores.clock import Cyclone10LPPLL
from litex.soc.integration.builder import Builder
from litex.soc.integration.soc_core import SoCCore
from litex_boards.platforms import c10lprefkit
from migen import ClockDomain, Module, Signal


class CRG(Module):
    def __init__(self, platform, sys_clk_freq):
        self.clock_domains.cd_sys = ClockDomain()

        self.rst = Signal()
        clk_pin = platform.request("clk12")

        self.submodules.pll = pll = Cyclone10LPPLL(speedgrade="-A7")
        self.comb += pll.reset.eq(~platform.request("cpu_reset") | self.rst)
        pll.register_clkin(clk_pin, 12e6)
        pll.create_clkout(self.cd_sys, sys_clk_freq)


class SoC(SoCCore):
    def __init__(self):
        platform = c10lprefkit.Platform()

        sys_clk_freq = 50e6

        SoCCore.__init__(
            self,
            platform,
            clk_freq=sys_clk_freq,
            ident="LiteX C10 LP Logic Analyzer",
            integrated_rom_size=131072,
        )

        self.submodules.crg = CRG(platform, sys_clk_freq)


def main():
    parser = argparse.ArgumentParser(description="abc")
    parser.add_argument("--build", action="store_true", help="Build bitstream")
    parser.add_argument("--load", action="store_true", help="Load bitstream to device")

    args = parser.parse_args()

    soc = SoC()

    directory_structure = {
        "output_dir": os.path.join("build", "soc"),
    }

    builder = Builder(soc, **directory_structure)
    builder.build(run=args.build)

    if args.load:
        prog = soc.platform.create_programmer()
        prog.load_bitstream(os.path.join(builder.gateware_dir, soc.build_name + ".sof"))


if __name__ == "__main__":
    main()
