/*
 * SPDX-FileCopyrightText: 2020 Efabless Corporation
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
 * SPDX-License-Identifier: Apache-2.0
 */

#include <defs.h>
#include <stub.c>

// change to your project's ID - ask Matt
#define PROJECT_ID 13

#define CLOCK 0
#define DATA_IN 1
#define SCAN_SELECT 2
#define LATCH_EN 3
#define DATA_OUT 0

#define SET(PIN,N) (PIN |=  (1<<N))
#define CLR(PIN,N) (PIN &= ~(1<<N))
#define GET(PIN,N) (PIN &   (1<<N))

void main()
{
	/* 
	IO Control Registers
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 3-bits | 1-bit | 1-bit | 1-bit  | 1-bit  | 1-bit | 1-bit   | 1-bit   | 1-bit | 1-bit | 1-bit   |

	Output: 0000_0110_0000_1110  (0x1808) = GPIO_MODE_USER_STD_OUTPUT
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 110    | 0     | 0     | 0      | 0      | 0     | 0       | 1       | 0     | 0     | 0       |
	
	 
	Input: 0000_0001_0000_1111 (0x0402) = GPIO_MODE_USER_STD_INPUT_NOPULL
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 001    | 0     | 0     | 0      | 0      | 0     | 0       | 0       | 0     | 1     | 0       |

	*/

    reg_mprj_io_8 = GPIO_MODE_MGMT_STD_OUTPUT; // output serial stream
    reg_mprj_xfer = 1;
    while (reg_mprj_xfer == 1);

    // activate the project by setting the 0th bit of 1st bank of LA
    reg_la0_iena = 0; // input enable off
    reg_la0_oenb = 0xFFFFFFFF; // enable all of bank0 logic analyser outputs (ignore the name, 1 is on, 0 off)
    reg_la0_data |= (1 << PROJECT_ID); // enable the project


    // check the first design
    /*
        .clk            (la1_data_in[0]),
        .data_in        (la1_data_in[1]),
        .scan_select    (la1_data_in[2]),
        .latch_enable   (la1_data_in[3]),
    */

    int i ; 
    reg_la1_oenb = 0xFFFFFFFF; // enable
    reg_la1_iena = 0; // enable

    SET(reg_la1_data, SCAN_SELECT);
    // scan and latch are low
    // clock 8 times with no data
    for(i = 0; i < 8; i ++ )
    {
        // set 0 bit high
        if( i == 7 )
            SET(reg_la1_data, DATA_IN);
        else
            CLR(reg_la1_data, DATA_IN);

        SET(reg_la1_data, CLOCK);
        CLR(reg_la1_data, CLOCK);
    }

    // latch it
    SET(reg_la1_data, LATCH_EN);
    CLR(reg_la1_data, LATCH_EN);

    // capture the output data into the scan chain, one clock cycle with scan_select low
    CLR(reg_la1_data, SCAN_SELECT);
    SET(reg_la1_data, CLOCK);
    CLR(reg_la1_data, CLOCK);
    SET(reg_la1_data, SCAN_SELECT);

    // clock it out, need 32 clocks as there are 4 designs
    for(i = 0; i < 32; i ++ )
    {
        SET(reg_la1_data, CLOCK);

        // put the data out on pin 8
        // should be able to read it in reg_la1_data_in
        reg_mprj_datal = GET(reg_la1_data_in, DATA_OUT) << 8;
            
        CLR(reg_la1_data, CLOCK);
    }


}

