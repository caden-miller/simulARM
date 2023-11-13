`timescale 1ns / 1ps

module RegisterFile(BusA, BusB, BusW, RA, RB, RW, RegWr, Clk);
    output reg [63:0] BusA;
    output reg [63:0] BusB;
    input [63:0] BusW;
    input [4:0] RA;
    input [4:0] RB;
    input [4:0] RW;
    input RegWr;
    input Clk;
    reg [63:0] registers [31:0];
    
    
    
    
    
    

    // set BusA and BusB 
    always @ (*) begin
        registers[31] = 64'd0;
        if (!registers[RA])
            registers[RA] = 64'd0;
        if (!registers[RB])
            registers[RB] = 64'd0;
        #2 BusA = registers[RA]; BusB = registers[RB];
    end
     
    always @ (negedge Clk) begin
        if(RegWr && RW != 5'd31)
            #3 registers[RW] = BusW;
    end
endmodule