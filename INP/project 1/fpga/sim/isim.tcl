proc isim_script {} {

   add_divider "Signals of the Vigenere Interface"
   add_wave_label "" "CLK" /testbench/clk
   add_wave_label "" "RST" /testbench/rst
   add_wave_label "-radix ascii" "DATA" /testbench/tb_data
   add_wave_label "-radix ascii" "KEY" /testbench/tb_key
   add_wave_label "-radix ascii" "CODE" /testbench/tb_code

   add_divider "Vigenere Inner Signals"
   add_wave_label "" "state" /testbench/uut/state
   # sem doplnte vase vnitrni signaly. chcete-li v diagramu zobrazit desitkove
   # cislo, vlozte do prvnich uvozovek: -radix dec
	add_wave_label " -radix dec" "posun" /testbench/uut/posun
   	add_wave_label " -radix ascii" "plus" /testbench/uut/PlusKorekcia
	add_wave_label " -radix ascii" "minus" /testbench/uut/MinusKorekcia
	add_wave_label "" "stav" /testbench/uut/stav
	add_wave_label "" "NasledujuciStav" /testbench/uut/NasledujuciStav
   run 8 ns
}
