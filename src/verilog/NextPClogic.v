module NextPClogic(NextPC, CurrentPC, SignExtImm64, Branch, ALUZero, Uncondbranch); 
       input  [63:0] CurrentPC, SignExtImm64; 
       input Branch, ALUZero, Uncondbranch; 
       output [63:0] NextPC; 
       reg [63:0] NewPC;
       reg [63:0] ImmShift;
       always @ (*) begin
              if (Branch == 1'b1 && ALUZero == 1'b1) begin
                     ImmShift = SignExtImm64 << 2;
                     NewPC = CurrentPC + ImmShift;
              end
              else if (Uncondbranch == 1'b1) begin
                     ImmShift = SignExtImm64 << 2;
                     NewPC = CurrentPC + ImmShift;
              end
              else begin
                     NewPC = CurrentPC + 4;
              end
       end
       assign NextPC = NewPC;

endmodule