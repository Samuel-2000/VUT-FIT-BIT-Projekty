-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2021 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Samuel Kuchta (xkucht11)

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
   DATA_WREN  : out std_logic;                    -- cteni z pameti (DATA_WREN='0') / zapis do pameti (DATA_WREN='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WREN musi byt '0'
   OUT_WREN : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

  -- signaly PC
	signal program_counter_increment : std_logic;  -- PC+1
	signal program_counter_decrement : std_logic;  -- PC-1
	signal program_counter_adr       : std_logic_vector (11 downto 0);

  -- signaly CNT
   signal counter_data_increment    : std_logic;
   signal counter_data_decrement    : std_logic;
	signal counter_data_null         : std_logic;
   signal counter_data              : std_logic_vector(7 downto 0);

  -- Signaly PTR
	signal ptr_increment             : std_logic;
	signal ptr_decrement             : std_logic;
	signal pointer_adr               : std_logic_vector (9 downto 0);

  -- Signaly multiplexoru
	signal mux_select                : std_logic_vector (1 downto 0);
   signal data_rdata_null : std_logic;


	type state_list is (
		initialize, -- výchozí stav
		fetch,      -- načtení instrukce
		decode,     -- dekódování instrukce
		val_inc,    -- '+' inkrementace hodnoty aktuální buňky
		  val_inc_2,
		val_dec,    -- '-' dekrementace hodnoty aktuální buňky
		  val_dec_2,
		ptr_inc,    -- '>' inkrementace hodnoty ukazatele
		ptr_dec,    -- '<' dekrementace hodnoty ukazatele
        s_while,    -- '[' začátek cyklu while
          s_while_1,
          s_while_2,
          s_while_3,
        s_end_while, -- ']' konec cyklu while
          s_end_while_1,
          s_end_while_2,
          s_end_while_3,
          s_end_while_4,
		print_lcd,  -- '.' tisk hodnoty aktuální buňky
		  print_lcd_2,
		load_lcd,   -- ',' načtení hodnoty do aktuální buňky
		  load_lcd_2,
        s_break,    -- '~' ukončení prováděného cyklu while
          s_break_1,
          s_break_2,
		halt,       -- zastavení vykonávání programu
		ostatni     -- kdyz je znak nedefinovany
	);
	signal state : state_list := initialize; -- aktuální stav
	signal next_state : state_list; -- následující stav

begin

  -- PC programovy citac, ukazuje do pameti ROM (pamet programu), ctou se instrukce
	PC_ROM_INST: process (CLK, RESET, program_counter_adr) is
	begin
		if RESET = '1' then
			program_counter_adr <= (others => '0');
		elsif rising_edge(CLK) then
            if (program_counter_increment = '1') then
				program_counter_adr <= program_counter_adr + 1; --inc
			elsif (program_counter_decrement = '1') then
				program_counter_adr <= program_counter_adr - 1; --dec
			end if;
		end if;
	end process PC_ROM_INST;

	CODE_ADDR <= program_counter_adr;



  -- CNT Určení začátku a konce příkazu smyček while ([~]), čítání počtu závorek.
    Counter_Loop: process (CLK, RESET)
    begin
        if (RESET = '1') then
            counter_data <= (others => '0');
        elsif rising_edge(CLK) then 
            if (counter_data_increment = '1') then
                counter_data <= counter_data + 1; --inc
            elsif (counter_data_decrement = '1') then
                counter_data <= counter_data - 1; --dec
            end if;
        end if;
    end process Counter_Loop;

    counter_data_null <= '1' when (counter_data = "00000000") else '0';



  -- PTR programovy citac, ukazuje do pameti RAM (datova pamet)
	PTR_RAM_DATA: process (CLK, RESET) is
	begin
		if RESET = '1' then
			pointer_adr <= (others => '0');
		elsif rising_edge(CLK) then
			if (ptr_increment = '1') then
				pointer_adr <= pointer_adr + 1;
			elsif (ptr_decrement = '1') then
				pointer_adr <= pointer_adr - 1;
			end if;
		end if;
	end process PTR_RAM_DATA;

	DATA_ADDR <= pointer_adr;



  -- MUX, co se ma zapisovat do pameti RAM
    DATA_WDATA <= IN_DATA when (mux_select = "00") else
        (DATA_RDATA + 1) when (mux_select = "01") else
        (DATA_RDATA - 1) when (mux_select = "10") else
        (others => '0');

    data_rdata_null <= '1' when (DATA_RDATA = "00000000") else '0';     


	-- Automat, který ovládá řídící signály (FSM).
  -- Logika aktuálního stavu.
	FSM_Present_State: process (CLK, RESET) is
	begin
		if RESET = '1' then
			state <= initialize;
		elsif rising_edge(CLK) then
			if EN = '1' then
				state <= next_state;
			end if;
		end if;
	end process FSM_Present_State;

  -- Logika následujícího stavu a zároveň výstupní logika.
	FSM_Next_State: process (state, DATA_RDATA, CODE_DATA, IN_VLD, OUT_BUSY, data_rdata_null, counter_data_null) is
	begin
		-- signaly
      -- PC
		program_counter_increment <= '0';
		program_counter_decrement <= '0';

      -- PTR
		ptr_increment <= '0';
		ptr_decrement <= '0';

      -- CNT
       counter_data_increment <= '0';
	    counter_data_decrement <= '0';

      -- MUX
		mux_select <= "11"; -- 00

      -- ROM
        CODE_EN <= '0';

      -- RAM
		DATA_WREN <= '0';  -- cteni
		DATA_EN <= '0';

      -- Input/Output
		OUT_WREN <= '0';
		IN_REQ <= '0';


		case state is
			when initialize => -- výchozí stav
				next_state <= fetch;


			when fetch => -- načtení instrukce
			   CODE_EN <= '1';
				next_state <= decode;


			when decode => -- dekódování instrukce
				case CODE_DATA is 
					when X"3E" =>
						next_state <= ptr_inc;      -- inkrementace ukazatele na RAM
					when X"3C" =>
						next_state <= ptr_dec;      -- dektrementace ukazatele na RAM
					when X"2B" =>
						next_state <= val_inc;      -- inkrementace aktualni bunky v RAM
					when X"2D" =>
						next_state <= val_dec;      -- dekrementace aktualni bunky v RAM
					when X"5B" =>
						next_state <= s_while;      -- zacatek cyklu
					when X"5D" =>
						next_state <= s_end_while;  -- konec cyklus
					when X"2E" =>
						next_state <= print_lcd;    -- tisk dat na LCD display
					when X"2C" =>
						next_state <= load_lcd;     -- cteni z klavesnice
                    when X"7E" =>
						next_state <= s_break;      -- ~ ukončení prováděného cyklu while
					when X"00" =>
						next_state <= halt;         -- zastavení vykonávání programu
					when others =>                  -- kdyz je kod jiny
                  next_state <= ostatni;          -- kdyz je znak nedefinovany
				end case;
				

			when ptr_inc => -- > incrementace citace PTR, incrementace hodnoty ukazatele
				ptr_increment <= '1';
				    program_counter_increment <= '1';
				    next_state <= fetch;
				

			when ptr_dec => -- < decrementace citace PTR, decrementace hodnoty ukazatele	
				ptr_decrement <= '1';
				    program_counter_increment <= '1';
				    next_state <= fetch;


			when val_inc => -- + incrementace hodnoty aktualni bunky 
				DATA_EN <= '1';
				DATA_WREN <= '0';  -- cteni
				    next_state <= val_inc_2;
				
            when val_inc_2 =>
                mux_select <= "01";
                DATA_EN <= '1';
                DATA_WREN <= '1';  -- zapis
                    program_counter_increment <= '1';
                    next_state <= fetch;


			when val_dec => -- - decrementace hodnoty aktualni bunky 	
				DATA_EN <= '1';
				DATA_WREN <= '0';  -- cteni
				    next_state <= val_dec_2;
				
            when val_dec_2 =>
                mux_select <= "10";
                DATA_EN <= '1';
                DATA_WREN <= '1';  -- zapis
                    program_counter_increment <= '1';
                    next_state <= fetch;


            when s_while =>  -- [ začátek cyklu while
            DATA_EN <= '1';
            DATA_WREN <= '0';  -- cteni
                program_counter_increment <= '1';
                next_state <= s_while_1;

            when s_while_1 =>
                if (data_rdata_null = '0') then -- (DATA_RDATA != 0)
                    next_state <= fetch;
                else -- (DATA_RDATA == 0)
                    counter_data_increment <= '1';
                    CODE_EN <= '1';
                    next_state <= s_while_2;
                end if;

            when s_while_2 =>
                if (counter_data_null = '1') then -- (CNT == 0)
                    next_state <= fetch;
                else -- (CNT != 0)
                    if CODE_DATA = X"5B" then -- (CODE_DATA == '[')
                        counter_data_increment <= '1';
                    elsif CODE_DATA = X"5D" then -- (CODE_DATA == ']')
                        counter_data_decrement <= '1';
                    end if;
                    program_counter_increment <= '1';
                    next_state <= s_while_3;
                end if;

            when s_while_3 =>
                CODE_EN <= '1';
                next_state <= s_while_2;


            when s_end_while => -- ] konec cyklu while
                DATA_EN <= '1';
                DATA_WREN <= '0';  -- cteni
                    next_state <= s_end_while_1;

            when s_end_while_1 =>
                if (data_rdata_null = '1') then -- (DATA_RDATA == 0)
                    program_counter_increment <= '1';
                    next_state <= fetch;
                else -- (DATA_RDATA != 0)
                    counter_data_increment <= '1';
                    program_counter_decrement <= '1';
                    next_state <= s_end_while_4;
                end if;

            when s_end_while_2 =>
                if (counter_data_null = '1') then -- (CNT == 0)
                    next_state <= fetch;
                else -- (CNT != 0)
                    if CODE_DATA = X"5D" then -- (CODE_DATA == ']')
                        counter_data_increment <= '1';
                    elsif CODE_DATA = X"5B" then -- (CODE_DATA == '[')
                        counter_data_decrement <= '1';
                    end if;
                    next_state <= s_end_while_3;
                end if;

            when s_end_while_3 =>
                if (counter_data_null = '1') then -- (CNT == 0)
                    program_counter_increment <= '1';
                else -- (CNT != 0)
                    program_counter_decrement <= '1';
                end if;
                next_state <= s_end_while_4;

            when s_end_while_4 =>
                CODE_EN <= '1';
                    next_state <= s_end_while_2;


			when print_lcd =>  -- . tisknuti hodnoty aktualni bunky na LCD
				if (OUT_BUSY = '1') then
					next_state <= print_lcd;
				else
					DATA_EN <= '1';
					DATA_WREN <= '0';  -- cteni
                        next_state <= print_lcd_2;
				end if;
							
            when print_lcd_2 =>
                OUT_DATA <= DATA_RDATA;
                OUT_WREN <= '1';
                    program_counter_increment <= '1';
                    next_state <= fetch;
				

			when load_lcd =>-- , hodnota z klavesnice se ulozi do aktualni bunky.	
				IN_REQ <= '1';
                if IN_VLD = '0' then
					next_state <= load_lcd;
                else
				    next_state <= load_lcd_2; 
				end if;

            when load_lcd_2 =>
                mux_select <= "00";
                DATA_EN <= '1';
                DATA_WREN <= '1';  -- zapis
                    program_counter_increment <= '1';
                    next_state <= fetch;


			when s_break =>  -- ~ - ukončení prováděného cyklu while
				counter_data_increment <= '1';
				program_counter_increment <= '1';
                next_state <= s_break_2;

			when s_break_1 =>
				if (counter_data_null = '1') then -- (CNT == 0)
					next_state <= fetch;
				else -- (CNT != 0)
					if CODE_DATA = X"5B" then -- (CODE_DATA == '[')
						counter_data_increment <= '1';
					elsif CODE_DATA = X"5D" then -- (CODE_DATA == ']')
						counter_data_decrement <= '1'; 
					end if;
					program_counter_increment <= '1';
					next_state <= s_break_2;
				end if;

			when s_break_2 =>
				CODE_EN <= '1';
				    next_state <= s_break_1;


			when halt =>
				next_state <= halt;  -- zastavení vykonávání programu

			when ostatni =>  -- kdyz je znak nedefinovany
				program_counter_increment <= '1';
				next_state <= fetch;

            when others =>
            null;
				
		end case;
    end process FSM_Next_State;
end behavioral;
