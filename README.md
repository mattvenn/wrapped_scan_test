[![multi_tool](https://github.com/mattvenn/wrapped_scan_test/actions/workflows/multi_tool.yaml/badge.svg)](https://github.com/mattvenn/wrapped_scan_test/actions/workflows/multi_tool.yaml)

# Scan Wrapper Test

Proof of concept for a scan chain. 

The idea is that there will be 100's of modules with 8 ins and 8 outs.
They are all digital, and operate at slow speeds. 

Rather than try to multiplex all IO, or use tristate buffers, we use scan chain flipflops and latches to first latch in data, and then capture the output.

![block diagram](docs/blockdiagram.jpg)

This trace shows loading 8 ins, then capturing 8 outs.

![trace](docs/trace.png)

# logs

How to test this on ASIC? 

* with multi tools everything expects tristate
* these will need to be instantiated by hand
* for now, put 4 blocks inside one bigger block, build all, then add as a normal project

# Zero to ASIC Course

This project was made as part of the [Zero to ASIC Course](https://zerotoasiccourse.com)!

# License

This project is [licensed under Apache 2](LICENSE)
