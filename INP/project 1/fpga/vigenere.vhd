library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

-- rozhrani Vigenerovy sifry
entity vigenere is
   port(
         CLK : in std_logic;
         RST : in std_logic;
         DATA : in std_logic_vector(7 downto 0);
         KEY : in std_logic_vector(7 downto 0);

         CODE : out std_logic_vector(7 downto 0)
    );
end vigenere;

-- V souboru fpga/sim/tb.vhd naleznete testbench, do ktereho si doplnte
-- znaky vaseho loginu (velkymi pismeny) a znaky klice dle vaseho prijmeni.

architecture behavioral of vigenere is

    -- Sem doplnte definice vnitrnich signalu, prip. typu, pro vase reseni,
    -- jejich nazvy doplnte tez pod nadpis Vigenere Inner Signals v souboru
    -- fpga/sim/isim.tcl. Nezasahujte do souboru, ktere nejsou explicitne
    -- v zadani urceny k modifikaci.

	signal posun: std_logic_vector(7 downto 0);
	signal PlusKorekcia:  std_logic_vector(7 downto 0);
	signal MinusKorekcia:  std_logic_vector(7 downto 0);
	signal stav: std_logic_vector(1 downto 0);
	signal NasledujuciStav: std_logic_vector(1 downto 0);

begin
	VypocetPosunu: process (KEY) is
	begin
		posun <= KEY - 64;
	end process;

	SchrankaPlus: process (posun, DATA) is
	begin
		PlusKorekcia <= DATA + posun;
		if DATA + posun > 90 then
			PlusKorekcia <= DATA + posun - 26;
		end if;

	end process;

	SchrankaMinus: process (posun, DATA) is
	begin
		MinusKorekcia<= DATA - posun;
		if DATA - posun < 65 then
			MinusKorekcia <= DATA - posun + 26;
		end if;
	end process;

	StavovyRegister : process(RST, CLK, stav, NasledujuciStav) is
	begin
		if RST = '1' then
			stav <= "00";
		elsif rising_edge(CLK) then
			stav <= NasledujuciStav;
		end if;
	end process;

	Mealy: process (CLK, RST, stav, NasledujuciStav, MinusKorekcia, PlusKorekcia, DATA) is
	begin
		if RST = '1' then
			CODE <= "00100011";
			NasledujuciStav <= "00";
		elsif (conv_integer(unsigned(DATA)) >= 48) and (conv_integer(unsigned(DATA)) <= 57) then
			CODE <= "00100011";
			NasledujuciStav <= "00";
		elsif stav = "00" then
			CODE <= PlusKorekcia;
			NasledujuciStav <= "01";
		else
			CODE <= MinusKorekcia;
			NasledujuciStav <= "00";
		end if;
	end process;
	
    -- Sem doplnte popis obvodu. Doporuceni: pouzivejte zakladni obvodove prvky
    -- (multiplexory, registry, dekodery,...), jejich funkce popisujte pomoci
    -- procesu VHDL a propojeni techto prvku, tj. komunikaci mezi procesy,
    -- realizujte pomoci vnitrnich signalu deklarovanych vyse.

    -- DODRZUJTE ZASADY PSANI SYNTETIZOVATELNEHO VHDL KODU OBVODOVYCH PRVKU,
    -- JEZ JSOU PROBIRANY ZEJMENA NA UVODNICH CVICENI INP A SHRNUTY NA WEBU:
    -- http://merlin.fit.vutbr.cz/FITkit/docs/navody/synth_templates.html.


end behavioral;
