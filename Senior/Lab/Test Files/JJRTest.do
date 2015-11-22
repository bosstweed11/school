restart -f
force -freeze /cpu_test/reset 0 0
force -freeze clock 1 0, 0 {50 ns} -r 100
force /cpu_test/instruction_memory/instruction_array(0) X"20070001"
force /cpu_test/instruction_memory/instruction_array(1) X"240B0200"
force /cpu_test/instruction_memory/instruction_array(2) X"240B0200"
force /cpu_test/instruction_memory/instruction_array(3) X"01600008"
force /cpu_test/instruction_memory/instruction_array(4) X"00000000"
force /cpu_test/instruction_memory/instruction_array(5) X"00000000"
force /cpu_test/instruction_memory/instruction_array(6) X"00000000"
force /cpu_test/instruction_memory/instruction_array(7) X"00000000"
run 200ns
force -freeze /cpu_test/reset 1 0
run 200ns
force -freeze /cpu_test/reset 0 0
run 1300ns
