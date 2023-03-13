library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.numeric_std.all;

architecture Behavioral of tlv_gp_ifc is
  signal A : std_logic_vector(3 downto 0) := (others => '0');  -- vyberá aktívny ståpec
  signal R : std_logic_vector(7 downto 0) := (others => '0');  -- riadok led svetiel
  signal DATA : std_logic_vector(0 to 127) :=
    "11111111" &
    "00000000" &
    "01000110" &
    "10001001" &
    "10001001" &
    "10010001" &
    "01100010" &
    "00000000" &

    "11111111" &
    "00001000" &
    "00010100" &
    "00100010" &
    "01000001" &
    "10000000" &
    "00000000" &
    "11111111";  -- pamä obsahujúca iniciály S a K

  constant COL_CNT : integer := (A'length)*(A'length);  -- poèet ståpcov
  constant ROW_CNT : integer := R'length;  -- poèet riadkov
  
  constant CLK_PERIOD   : integer := 1000000 / 25000; 	-- 4e-8
  constant OUT_PERIOD   : integer := 1000000 / 5; 		-- 2e-4
  constant PERIOD_RATIO : integer := (OUT_PERIOD/CLK_PERIOD)-1;  -- 4999

  signal cnt        : std_logic_vector(13 downto 0) := (others => '0');  -- poèíta po hodnotu OUT_PERIOD/CLK_PERIOD
  signal frame_cnt  : std_logic_vector(5 downto 0)  := (others => '0');  -- poèíta, kolko krat sa rozsvietily LED-diódy zlava do prava (jeden frame)
  
  signal EN   			      : std_logic;  -- aktivuje hlavnı proces
  signal shift            : std_logic_vector(4 downto 0) := "00000"; -- aplikuje posun (rotáciu) ståpcov
  signal pocet_rotaci   	: std_logic_vector(2 downto 0) := "000";   -- poèíta poèet rotácii a urèuje, èi má prebieha posun do¾ava alebo doprava.
  signal animation_state  : std_logic := '0'; -- aktivuje animáciu

begin
  process (CLK, RESET)
  begin
    if (RESET = '1') then
      EN <= '0';
      cnt <= (others => '0');
    elsif rising_edge(CLK) then
      EN <= '0';
      if (cnt = PERIOD_RATIO) then
        cnt <= (others => '0');
        EN <= '1';
      else 
        cnt <= cnt + 1;
      end if;
    end if;
  end process;

  FSM: process(EN)
  variable colid : integer range 0 to 15;
  begin
    if rising_edge(EN) then

      colid := (to_integer(unsigned(A)) + to_integer(unsigned(shift))) mod COL_CNT;
      R <= DATA(ROW_CNT*colid to (ROW_CNT*(colid+1))-1);

      if animation_state = '1' then  -- implementacia animacie
        for i in 0 to 7 loop
          R(i) <= not R(i);
        end loop;
      end if;

      if A = "1111" then
        frame_cnt <= frame_cnt + 1;
        if frame_cnt = "11111" then

          if pocet_rotaci < "011" then -- 3x posun doprava
            shift <= shift - 1;
          else
            shift <= shift + 1;  -- 3x posun dolava
          end if;
  
          if shift = "01111" or shift = "10001" then 
            shift <= "00000";
            if pocet_rotaci >= "110" then -- animacia 
              animation_state <= '1';
            else
              animation_state <= '0';
            end if;
            pocet_rotaci <= pocet_rotaci + 1;
          end if;

        end if;  -- frame_cnt = 80
      end if; -- A = "1111"
      A <= A + 1; -- posuvanie stlpcov
    end if;  -- rising_edge(EN)
  end process FSM;

  -- mapovani vystupu
  -- nemenit
  X(6) <= A(3);
  X(8) <= A(1);
  X(10) <= A(0);
  X(7) <= '0'; -- en_n
  X(9) <= A(2);

  X(16) <= R(1);
  X(18) <= R(0);
  X(20) <= R(7);
  X(22) <= R(2);

  X(17) <= R(4);
  X(19) <= R(3);
  X(21) <= R(6);
  X(23) <= R(5);
end Behavioral;
