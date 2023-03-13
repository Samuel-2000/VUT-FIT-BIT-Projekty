-- fsm.vhd: Finite State Machine
-- Author(s): Samuel Kuchta
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

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is 
   type t_state is (TEST_DEFAULT, TEST_FAIL, TEST_SUCCESS, PRINT_FAIL, PRINT_SUCCESS, FINISH,
						  TEST1, TEST14, TEST141, TEST1410, TEST14104, TEST141043, TEST1410430, TEST14104308, TEST141043086, TEST1410430865,
									TEST15, TEST156, TEST1567, TEST15671, TEST156714, TEST1567145, TEST15671454, TEST156714540, TEST1567145406);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST_DEFAULT;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
	
	
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_DEFAULT =>	-- kod1 = 1410430865 	 kod2 = 1567145406
      next_state <= TEST_DEFAULT;
		if (KEY(1) = '1') then
			next_state <= TEST1;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1 =>
      next_state <= TEST1;
		if (KEY(4) = '1') then
			next_state <= TEST14;
		end if;
		if (KEY(5) = '1') then
			next_state <= TEST15;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;
		
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST14 =>
      next_state <= TEST14;
		if (KEY(1) = '1') then
			next_state <= TEST141;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
	   	elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST141 =>
      next_state <= TEST141;
		if (KEY(0) = '1') then
			next_state <= TEST1410;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1410 =>
      next_state <= TEST1410;
		if (KEY(4) = '1') then
			next_state <= TEST14104;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - - 
   when TEST14104 =>
      next_state <= TEST14104;
		if (KEY(3) = '1') then
			next_state <= TEST141043;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST141043 =>
      next_state <= TEST141043;
		if (KEY(0) = '1') then
			next_state <= TEST1410430;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1410430 =>
      next_state <= TEST1410430;
		if (KEY(8) = '1') then
			next_state <= TEST14104308;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST14104308 =>
      next_state <= TEST14104308;
		if (KEY(6) = '1') then
			next_state <= TEST141043086;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - - 
   when TEST141043086 =>
      next_state <= TEST141043086;
		if (KEY(5) = '1') then
			next_state <= TEST_SUCCESS;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;









   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST15 =>
      next_state <= TEST15;
		if (KEY(6) = '1') then
			next_state <= TEST156;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST156 =>
      next_state <= TEST156;
		if (KEY(7) = '1') then
			next_state <= TEST1567;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1567 =>
      next_state <= TEST1567;
		if (KEY(1) = '1') then
			next_state <= TEST15671;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - - 	
   when TEST15671 =>
      next_state <= TEST15671;
		if (KEY(4) = '1') then
			next_state <= TEST156714;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST156714 =>
      next_state <= TEST156714;
		if (KEY(5) = '1') then
			next_state <= TEST1567145;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1567145 =>
      next_state <= TEST1567145;
		if (KEY(4) = '1') then
			next_state <= TEST15671454;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - - -
   when TEST15671454 =>
      next_state <= TEST15671454;
		if (KEY(0) = '1') then
			next_state <= TEST156714540;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST156714540 =>
      next_state <= TEST156714540;
		if (KEY(6) = '1') then
			next_state <= TEST_SUCCESS;
			elsif (KEY(15) = '1') then
				next_state <= PRINT_FAIL; 
		   elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= TEST_FAIL;
      end if;

		
		
		
		
		
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_FAIL =>
      next_state <= TEST_FAIL;
      if (KEY(15) = '1') then
         next_state <= PRINT_FAIL;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -

   when TEST_SUCCESS =>
      next_state <= TEST_FAIL;
      if (KEY(15) = '1') then
         next_state <= PRINT_SUCCESS;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
 
   when PRINT_FAIL =>
      next_state <= PRINT_FAIL;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_SUCCESS =>
      next_state <= PRINT_SUCCESS;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST_DEFAULT; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	
	
	
	
	
	
   when others =>
      next_state <= TEST_DEFAULT;
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
   when TEST_DEFAULT | TEST_FAIL | TEST_SUCCESS |
		  TEST1 | TEST14 | TEST141 | TEST1410 | TEST14104 | TEST141043 | TEST1410430 | TEST14104308 | TEST141043086 | TEST1410430865 |
					TEST15 | TEST156 | TEST1567 | TEST15671 | TEST156714 | TEST1567145 | TEST15671454 | TEST156714540 | TEST1567145406 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_FAIL =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_SUCCESS =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
		FSM_MX_MEM     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
		if (KEY(15) = '1') then
		FSM_LCD_CLR 	<= '1';
	end if;
      if (KEY(14 downto 0) /= "000000000000000") then
		FSM_LCD_CLR 	<= '1';
		end if;
   end case;
end process output_logic;

end architecture behavioral;

