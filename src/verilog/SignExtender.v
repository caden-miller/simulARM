`define I_TYPE 2'b00
`define D_TYPE 2'b01
`define B      2'b10
`define CBZ    2'b11

module SignExtender(BusImm, Ins26, Ctrl); 
  output reg [63:0] BusImm; 
  input [25:0] Ins26; 
  input [1:0] Ctrl; 

  reg extBit;
  always @(Ins26 or Ctrl) begin
        case(Ctrl)
            `I_TYPE: begin
                extBit = (Ctrl ? 1'b0 : Ins26[21]); 
                BusImm = {{52{extBit}}, Ins26[21:10]}; 
            end
            `D_TYPE: begin
                extBit = (Ctrl ? 1'b0 : Ins26[20]); 
                BusImm = {{55{extBit}}, Ins26[20:12]};
            end
            `B: begin
                extBit = (Ctrl ? 1'b0 : Ins26[25]); 
                BusImm = {{38{extBit}}, Ins26[25:0]};
            end
            `CBZ: begin
                extBit = (Ctrl ? 1'b0 : Ins26[23]); 
                BusImm = {{45{extBit}}, Ins26[23:5]};
            end
        endcase
    end 

endmodule