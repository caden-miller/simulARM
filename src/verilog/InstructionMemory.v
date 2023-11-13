`timescale 1ns / 1ps
/*
 * Module: InstructionMemory
 *
 * Implements read-only instruction memory
 *
 */
module InstructionMemory(Data, Address);
    
    parameter T_rd = 20;
    parameter MemSize = 40;
    
    output [31:0] Data;
    input [63:0]  Address;
    reg [31:0]     Data;
    always @ (Address) begin
        $display("InstructionMemory: data = %h", Data);
        case(Address)
		63'h0 : Data = 32'h8b020020;
		63'h4 : Data = 32'h8b020020;
		63'h8 : Data = 32'h8b020020;
		63'hc : Data = 32'h8b020020;
		63'h10 : Data = 32'h8b020020;
		default : Data = 32'h00000000;
	endcase
end

endmodule
