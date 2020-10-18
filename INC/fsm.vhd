-- fsm.vhd: Finite State Machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- 2763055548
-- 3453819436
-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is ( ZACIATOK, CISLO_ERROR, CISLO_2, CISLO_27, CISLO_276, CISLO_2763, CISLO_27630, CISLO_276305, CISLO_2763055, CISLO_27630555, CISLO_276305554, CISLO_2763055548, CISLO_3, CISLO_34, CISLO_345, CISLO_3453, CISLO_34538, CISLO_345381, CISLO_3453819, CISLO_34538194, CISLO_345381943, CISLO_3453819436,VYPISAT_ERROR, CISLO_OK, HOTOVO );
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= ZACIATOK;
		--present_state <= TEST_DEFAULT2;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when ZACIATOK =>
      next_state <= ZACIATOK;
		if (KEY(2) = '1') then
         next_state <= CISLO_2;
      elsif (KEY(3) = '1') then
         next_state <= CISLO_3;			
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_2 =>
      next_state <= CISLO_2;
		if (KEY(7) = '1') then
         next_state <= CISLO_27;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_27 =>
      next_state <= CISLO_27;
		if (KEY(6) = '1') then
         next_state <= CISLO_276;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_276 =>
      next_state <= CISLO_276;
		if (KEY(3) = '1') then
         next_state <= CISLO_2763;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_2763 =>
      next_state <= CISLO_2763;
		if (KEY(0) = '1') then
         next_state <= CISLO_27630;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;	
    -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_27630 =>
      next_state <= CISLO_27630;
		if (KEY(5) = '1') then
         next_state <= CISLO_276305;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
     -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_276305 =>
      next_state <= CISLO_276305;
		if (KEY(5) = '1') then
         next_state <= CISLO_2763055;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_2763055 =>
      next_state <= CISLO_2763055;
		if (KEY(5) = '1') then
         next_state <= CISLO_27630555;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_27630555 =>
      next_state <= CISLO_27630555;
		if (KEY(4) = '1') then
         next_state <= CISLO_276305554;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_276305554 =>
      next_state <= CISLO_276305554;
		if (KEY(8) = '1') then
         next_state <= CISLO_2763055548;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;		
	 -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_2763055548 =>
      next_state <= CISLO_2763055548;
      if (KEY(15) = '1') then
         next_state <= CISLO_OK;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
		-- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_3 =>
     next_state <= CISLO_3;
		if (KEY(4) = '1') then
        next_state <= CISLO_34;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
  when CISLO_34 =>
     next_state <= CISLO_34;
		if (KEY(5) = '1') then
        next_state <= CISLO_345;
      elsif (KEY(15) = '1') then
        next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
     end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_345 =>
      next_state <= CISLO_345;
		if (KEY(3) = '1') then
         next_state <= CISLO_3453;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_3453 =>
      next_state <= CISLO_3453;
		if (KEY(8) = '1') then
         next_state <= CISLO_34538;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_34538 =>
      next_state <= CISLO_34538;
		if (KEY(1) = '1') then
         next_state <= CISLO_345381;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_345381 =>
      next_state <= CISLO_345381;
		if (KEY(9) = '1') then
         next_state <= CISLO_3453819;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_3453819 =>
      next_state <= CISLO_3453819;
		if (KEY(4) = '1') then
         next_state <= CISLO_34538194;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
  when CISLO_34538194 =>
      next_state <= CISLO_34538194;
		if (KEY(3) = '1') then
         next_state <= CISLO_345381943;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_345381943 =>
      next_state <= CISLO_345381943;
		if (KEY(6) = '1') then
         next_state <= CISLO_3453819436;
      elsif (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_3453819436 =>
        next_state <= CISLO_3453819436;
      if (KEY(15) = '1') then
         next_state <= CISLO_OK;
		elsif (KEY(14 downto 0) /= "000000000000000") then
		   next_state <= CISLO_ERROR;
      end if;		
   -- - - - - - - - - - - - - - - - - - - - - - -
  when CISLO_ERROR =>
      next_state <= CISLO_ERROR;
		if (KEY(15) = '1') then
         next_state <= VYPISAT_ERROR;
		end if;	
		-- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  when VYPISAT_ERROR =>
      next_state <= VYPISAT_ERROR;
      if (CNT_OF = '1') then
         next_state <= HOTOVO;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_OK =>
      next_state <= CISLO_OK;
      if (CNT_OF = '1') then
         next_state <= HOTOVO;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HOTOVO =>
      next_state <= HOTOVO;
      if (KEY(15) = '1') then
         next_state <= ZACIATOK;		
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= ZACIATOK;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when ZACIATOK | CISLO_ERROR | CISLO_2 | CISLO_27 | CISLO_276 | CISLO_2763 | CISLO_27630 | CISLO_276305 | CISLO_2763055 | CISLO_27630555 | CISLO_276305554 | CISLO_2763055548 | CISLO_3 | CISLO_34 | CISLO_345 | CISLO_3453 | CISLO_34538 | CISLO_345381 | CISLO_3453819 | CISLO_34538194 | CISLO_345381943 | CISLO_3453819436 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when VYPISAT_ERROR =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
	-- - - - - - - - - - - - - - - - - - - - - - -
   when CISLO_OK => 
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
      FSM_MX_MEM     <= '1';		
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HOTOVO =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
   end case;
end process output_logic;

end architecture behavioral;

