#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <regex>


using namespace std;

const string START_INS_VERILOG_CODE = "`timescale 1ns / 1ps\n/*\n * Module: InstructionMemory\n *\n * Implements read-only instruction memory\n *\n */\nmodule InstructionMemory(Data, Address);\n    \n    parameter T_rd = 20;\n    parameter MemSize = 40;\n    \n    output [31:0] Data;\n    input [63:0]  Address;\n    reg [31:0]     Data;\n    always @ (Address) begin\n        $display(\"InstructionMemory: data = %h\", Data);\n        case(Address)\n";
const string END_INS_VERILOG_CODE = "\t\tdefault : Data = 32'h00000000;\n\tendcase\nend\n\nendmodule\n";


const string START_TEST_VERILOG_CODE = "`timescale 1ns / 1ps\n`define STRLEN 32\n`define HalfClockPeriod 60\n`define ClockPeriod `HalfClockPeriod * 2\n\nmodule SingleCycleProcTest_v;\n\n    // These tasks are used to check if a given test has passed and\n    // confirm that all tests passed.\n\n    // Inputs\n    reg         CLK;\n    reg         Reset_L;\n    reg [63:0]  startPC;\n    reg [7:0]   passed;\n    reg [15:0]  watchdog;\n\n    // Outputs\n    wire [63:0] MemtoRegOut;\n    wire [63:0] currentPC;\n    reg [63:0] finalPC;   \n\n    // Instantiate the Unit Under Test (UUT)\n    singlecycle uut (\n                          .CLK(CLK),\n                          .resetl(Reset_L),\n                          .startpc(startPC),\n                          .currentpc(currentPC),\n                          .MemtoRegOut(MemtoRegOut)\n                          );\n\n    initial begin\n        // Initialize Inputs\n        Reset_L = 1;\n        startPC = 63'h0;\n        passed = 0;\n\n        // Initialize Watchdog timer\n        watchdog = 0;\n\n        // Wait for global reset\n        #(1 * `ClockPeriod);\n\n        // Program 1\n        #1";
const string END_TEST_VERILOG_CODE = "@(posedge CLK);\n@(negedge CLK);\n@(posedge CLK);\nReset_L = 1;\n\n// ***********************************************************\n// This while loop will continue cycling the processor until the\n// PC reaches the final instruction in the first test.  If the\n// program forms an infinite loop, never reaching the end, the\n// watchdog timer will kick in and kill simulation after 64K\n// cycles.\n// ***********************************************************\n\nwhile (currentPC < finalPC)\nbegin\n    @(posedge CLK);\n    @(negedge CLK);\n    $display(\"CurrentPC:%h\",currentPC);\nend\n\n\n// ***********************************************************\n// Add your new tests here\n// ***********************************************************\n\n// Done\n   // Be sure to change the one to match\n// the number of tests you add.\n$finish;\nend\n\n// Initialize the clock to be 0\ninitial begin\n    CLK = 0;\nend\n\n// The following is correct if clock starts at LOW level at StartTime //\nalways begin\n    #`HalfClockPeriod CLK = ~CLK;\n    #`HalfClockPeriod CLK = ~CLK;\n    watchdog = watchdog +1;\nend\n\n// Kill the simulation if the watchdog hits 64K cycles\nalways @*\nif (watchdog == 16'hFF)\nbegin\n    $display(\"Watchdog Timer Expired.\");\n    $finish;\nend\n\nendmodule\n";
// pass string of instructions to this function
// this function will generate the verilog code
vector<string> get_instructions(int instruction_length) {
    // TODO : get instructions from GUI and pass it to the assembler
    vector<string> instructions(instruction_length);
    // TODO : get the instructions from the GUI
    // placeholder
    for (int i = 0; i < instruction_length; ++i) {
        instructions[i] = "add x0, x1, x2";
    }

    return instructions;
}


vector<string> get_hex_instructions(string assembled_file) {
    vector<string> instructions_hex;

    // Use popen() to run objdump
    FILE* pipe = popen(("aarch64-linux-gnu-objdump -d " + assembled_file).c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("popen failed!");
    }

    // Read the output of objdump
    cout << "Reading objdump output...\n";
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        string line(buffer);
        istringstream iss(line);
        vector<string> words((istream_iterator<string>(iss)),
                             istream_iterator<string>());

        // If the line has at least 2 words and the second word is a hexadecimal number, add it to instructions_hex
        if (words.size() >= 2 && words[1].find_first_not_of("0123456789abcdefABCDEF") == string::npos) {
            instructions_hex.push_back(words[1]);
        }
    }

    // Wait for objdump to finish
    pclose(pipe);

    return instructions_hex;
}

// generates the assembly file 
string generate_assembly_file(vector<string> instructions_natural, int instruction_length) {
    // TODO : generate the assembly file populated with instructions from instructions_natural
    // use popen() to run the GNU assembler
    ofstream assembly_file("assembly.s");

    // Write the instructions to the file
    for (int i = 0; i < instruction_length; i++) {
        assembly_file << instructions_natural[i] << "\n";
    }

    assembly_file.close();

    // Use popen() to run the GNU assembler
    FILE* pipe = popen("aarch64-linux-gnu-as assembly.s -o assembly.o", "r");
    if (!pipe) {
        return "popen failed!";
    }

    // Wait for the assembler to finish
    pclose(pipe);

    // Return the name of the assembly file
    return "assembly.o";

}


// generates the verilog file for reading by CPU verilog implementation
void generate_verilog_files(vector<string> instructions_hex, int instruction_length, int start_address) {
    // TODO : generate the verilog file
    // use get_hex_instructions() to get the hex instructions
    // write using ofstream
    ofstream verilog_ins_file("src/verilog/InstructionMemory.v");
    if (!verilog_ins_file) {
        throw runtime_error("Failed to open InstructionMemory.v for writing");
    }
    verilog_ins_file << START_INS_VERILOG_CODE;
    for (int i = 0; i < instruction_length; i++) 
    {
        verilog_ins_file << "\t\t63'h" << hex << start_address + i*4 << dec << " : Data = 32'h" << instructions_hex[i] << ";\n";
    }
    verilog_ins_file << END_INS_VERILOG_CODE;
    verilog_ins_file.close();

    ofstream verilog_test_file("src/verilog/SingleCycleProcTest.v");
    if (!verilog_test_file) {
        throw runtime_error("Failed to open SingleCycleProcTest.v for writing");
    }
    verilog_test_file << START_TEST_VERILOG_CODE;
    verilog_test_file << "\t\t\tReset_L = 0; startPC = 63'h" << hex << start_address << "; finalPC = 63'h" << start_address + instruction_length*4 << ";\n";
    verilog_test_file << END_TEST_VERILOG_CODE;
    verilog_test_file.close();
    return;
}

void get_verilog_output() {
    // TODO : compile the verilog file
    // use popen() to run the verilog compiler
    // return the name of the verilog file
    FILE* pipe = popen("iverilog -o verilog_exe src/verilog/*.v", "r");
    if (!pipe) {
        throw runtime_error("popen failed!");
    }

    // Wait for the assembler to finish

    pclose(pipe);
    pipe = popen("./verilog_exe", "r");
    // output the pipe contents
    if (!pipe) {
        throw runtime_error("popen failed!");
    }
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        cout << buffer;
    }
    pclose(pipe);
    return;
}

int main() {
    int instruction_length = 5;
    vector<string> instructions_natural = get_instructions(instruction_length);
    string assembled_file = generate_assembly_file(instructions_natural, instruction_length);
    vector<string> instructions_hex = get_hex_instructions(assembled_file);
    for (int i = 0; i < instruction_length; i++) {
        cout << instructions_hex[i] << "\n";
    }
    generate_verilog_files(instructions_hex, instruction_length, 0);
    get_verilog_output();


    return 0;
}
