-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2020 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Emma Krompascikova, xkromp00
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- ram[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_WE    : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti 
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu

    --program counter
    signal pcounter_reg : std_logic_vector (11 downto 0);
    signal pcounter_inc : std_logic;
    signal pcounter_dec : std_logic;
    signal pcounter_load : std_logic;

    --memory pointer
    signal pointer_reg : std_logic_vector (9 downto 0);
    signal pointer_inc : std_logic;
    signal pointer_dec : std_logic;

    --loop count
    signal count_reg : std_logic_vector (3 downto 0);
    signal count_inc : std_logic;
    signal count_dec : std_logic;

    --return adress register
    type type_addr_array is array (0 to 15) of std_logic_vector (11 downto 0);

    signal return_addr_reg : type_addr_array;
    signal return_addr_index : std_logic_vector (3 downto 0);
    signal return_addr_inc : std_logic; -- push
    signal return_addr_dec : std_logic; -- pop
    signal load_address : std_logic_vector (11 downto 0);

    -- multiplexor
    signal sel : std_logic_vector (1 downto 0);

    type FSMstate is (
  	state_init,
  	state_fetch,
	state_decode,
	state_pointer_inc,
	state_pointer_dec,
	state_value_inc,
	state_value_inc_2,
	state_value_inc_3,
	state_value_dec,
	state_value_dec_2,
	state_value_dec_3,
	state_while_start,
	state_while_start_2,
	state_while_start_3,
	state_while_noop,
	state_while_noop_2,
	state_while_end,
	state_while_end_2,
	state_write,
	state_write_2,
	state_read,
	state_read_2,
	state_stop);

    signal pstate : FSMstate; -- actual state
    signal nstate : FSMstate; -- next state

begin
	--program counter
	pcounter : process (RESET, CLK) begin
		if RESET = '1' then
			pcounter_reg <= "000000000000";
		else
			if rising_edge(CLK)  then
				if pcounter_inc = '1' then
					pcounter_reg <= pcounter_reg + 1;
				elsif pcounter_dec = '1' then
					pcounter_reg <= pcounter_reg - 1;
				elsif pcounter_load = '1' then
					pcounter_reg <= load_address;
				end if;
			end if;

		end if;
	end process;

	CODE_ADDR <= pcounter_reg;

	--memory pointer
	pointer :process (RESET, CLK) begin
		if RESET = '1' then
			pointer_reg <= "0000000000" ;
		else
			if rising_edge(CLK)  then
				if pointer_inc = '1' then
					pointer_reg <= pointer_reg + 1;
				elsif pointer_dec = '1' then
					pointer_reg <= pointer_reg - 1;
				end if;
			end if;

		end if;
	end process;

	DATA_ADDR <= pointer_reg;

	--loop counter
	counter :process (RESET, CLK) begin
		if RESET = '1' then
			count_reg <= "0000";
		else
			if rising_edge(CLK)  then
				if count_inc = '1' then
					count_reg <= count_reg + 1;
				elsif count_dec = '1' then
					count_reg <= count_reg - 1;
				end if;
			end if;

		end if;
	end process;

	--return address register
	return_addr :process (CLK, RESET) begin
		if RESET = '1' then
			return_addr_index <= "0000";
			return_addr_reg <= (others=>(others=>'0'));
			load_address <= "000000000000";
		else
			if rising_edge(CLK) then
				if return_addr_inc = '1' then -- start of while
					return_addr_reg(conv_integer(unsigned(return_addr_index))) <= pcounter_reg;
					return_addr_index <= return_addr_index + 1;
					load_address <= pcounter_reg;
				elsif return_addr_dec = '1' then -- end of while -> pop
					if return_addr_index >= 2 then
						load_address <= return_addr_reg(conv_integer(unsigned(return_addr_index - 2)));
					else
						load_address <= "000000000000";
					end if;

					return_addr_index <= return_addr_index - 1;
				end if;
			end if;
		end if;
	end process;

	--multiplexor
	multiplexor :process(RESET, CLK) begin
		if RESET = '1' then
			DATA_WDATA <= "00000000";
		else
			if rising_edge(CLK) then
				if SEL = "00" then
					DATA_WDATA <= IN_DATA;
				elsif SEL = "01" then
					DATA_WDATA <= DATA_RDATA - 1;
				elsif SEL = "10" then
					DATA_WDATA <= DATA_RDATA + 1;
				else
					DATA_WDATA <= (others => '0');
				end if;
			end if;
		end if;
	end process;

	--mealy fsm
	--Present State registr
	present_state_reg: process(RESET, CLK)
	begin
  		if (RESET='1') then
  	   		pstate <= state_init;
		else 
			if rising_edge(CLK) then
				if EN = '1' then
					pstate <= nstate;
				end if;
			end if;
		end if;

	end process;
 
	--Next State logic, Output logic
	next_state_logic: process(pstate, OUT_BUSY, IN_VLD, CODE_DATA, DATA_RDATA, count_reg)
	begin
 		pcounter_inc <='0';
   		pcounter_dec <='0';
 		pcounter_load <='0';

	        pointer_inc <='0';
		pointer_dec <='0';

		count_inc <= '0';
		count_dec <= '0';

		return_addr_inc <= '0';
		return_addr_dec <= '0';

		CODE_EN <='0';
		DATA_EN <='0';
		DATA_WE <='0';
		IN_REQ <='0';
		OUT_WE <='0';
		
		SEL <= "11";

   		case pstate is
  		    when state_init =>
        		   nstate <= state_fetch;
			-- enable memory
			when state_fetch =>
				CODE_EN <='1';
				nstate <= state_decode;
			-- decode symbol
			when state_decode =>
				case CODE_DATA is 
					when x"3E" =>
						nstate <= state_pointer_inc;
					when x"3C" =>
						nstate <= state_pointer_dec;
					when x"2B" =>
						nstate <= state_value_inc;
					when x"2D" =>
						nstate <= state_value_dec;
					when x"5B" =>
						nstate <= state_while_start;
					when x"5D" =>
						nstate <= state_while_end;
					when x"2E" =>
						nstate <= state_write;
					when x"2C" =>
						nstate <= state_read;
					when x"00" =>
						nstate <= state_stop;
					when others =>
						pcounter_inc <= '1';
						nstate <= state_fetch;
				end case;

			-- increase value
			when state_value_inc =>
				DATA_EN <= '1';
				nstate <= state_value_inc_2;

			when state_value_inc_2 =>
				SEL <= "10";
				nstate <= state_value_inc_3;

			when state_value_inc_3 =>
				DATA_EN <= '1';
				DATA_WE <= '1';
				pcounter_inc <= '1';
				nstate <= state_fetch;
			
			-- decrease value
			when state_value_dec =>
				DATA_EN <= '1';
				nstate <= state_value_dec_2;

			when state_value_dec_2 =>
				nstate <= state_value_dec_3;
				SEL <= "01";

			when state_value_dec_3 =>
				DATA_EN <= '1';
				DATA_WE <= '1';
				pcounter_inc <= '1';
				nstate <= state_fetch;
			
			-- increase pointer position
			when state_pointer_inc =>
				pointer_inc <= '1';
				pcounter_inc <= '1';
				nstate <= state_fetch;

			when state_pointer_dec =>
				pointer_dec <= '1';
				pcounter_inc <= '1';
				nstate <= state_fetch;

			-- start while
			when state_while_start =>
				DATA_EN <= '1';
				nstate <= state_while_start_2;

			when state_while_start_2 =>
				if DATA_RDATA = 0 then -- skip loop
					count_inc <= '1';
					nstate <= state_while_noop;
				else -- run loop
					nstate <= state_while_start_3;
				end if;
				pcounter_inc <= '1';

			when state_while_start_3 =>
				return_addr_inc <= '1';
				nstate <= state_fetch;
			
			-- inside while which should not be ran
			when state_while_noop =>
				CODE_EN <= '1';
				nstate <= state_while_noop_2;

			when state_while_noop_2 =>
				if CODE_DATA = x"5D" then
					pcounter_inc <= '1';

					if count_reg = 1 then 
						nstate <= state_fetch;
					else
						nstate <= state_while_noop;
					end if;
					count_dec <= '1';
				elsif CODE_DATA = x"5B" then
					count_inc <= '1';
					pcounter_inc <= '1';
					nstate <= state_while_noop;
				else
					pcounter_inc <= '1';
					nstate <= state_while_noop;
				end if;

			-- end while
			when state_while_end =>
				DATA_EN <= '1';
				nstate <= state_while_end_2;

			when state_while_end_2 =>
				if DATA_RDATA = 0 then -- end loop
					pcounter_inc <= '1';
					return_addr_dec <= '1';
				else -- another iteration, jump to start
					pcounter_load <= '1';
				end if;
				nstate <= state_fetch;
			
			-- write to display
			when state_write =>
				DATA_EN <= '1';
				nstate <= state_write_2;

			when state_write_2 =>
				if OUT_BUSY = '0' then
					OUT_WE <= '1';
					pcounter_inc <= '1';
					nstate <= state_fetch;
				else -- wait
					OUT_WE <= '1';
					nstate <= state_write_2;
				end if;
			
			-- get input from fitkit keypad
			when state_read =>
				IN_REQ <= '1';
				nstate <= state_read_2;

			when state_read_2 =>
				DATA_EN <= '1';
				DATA_WE <= '1';
				SEL <= "00";
				if IN_VLD = '0' then -- wait
					IN_REQ <= '1';
					nstate <= state_read_2;
				else
					nstate <= state_fetch;
					pcounter_inc <= '1';
				end if;

			-- program end
			when state_stop =>
				nstate <= state_stop;

			when others =>
				nstate <= state_stop;
        	   	
   	 	end case;
	end process;
	
	OUT_DATA <= DATA_RDATA; 

end behavioral;

