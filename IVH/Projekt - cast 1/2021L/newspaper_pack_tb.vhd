-- Samuel Kuchta (xkucht11)

  LIBRARY ieee;
  USE ieee.std_logic_1164.ALL;
  USE ieee.numeric_std.ALL;
  USE work.newspaper_pack.ALL;

  ENTITY testbench IS
  END testbench;

  ARCHITECTURE behavior OF testbench IS 
			 
-- Inputs
	signal DATA : std_logic_vector(0 to 11) := "011000100110";
	signal sloupec :  integer := -2;
	signal pocet_radku :  integer := 3;
	 
-- Output
	signal vector : std_logic_vector(0 to pocet_radku-1);

	BEGIN

	assert (pocet_radku > 0) report "pocet_radku musi byt vetsi nez nula" severity failure;
	assert (DATA'length mod pocet_radku = 0) report "velikost dat musi byt delitelna poctem radku" severity failure;

	  tb : PROCESS
		  BEGIN
		  
			  sloupec <= sloupec + 1;
			  vector <= GETCOLUMN(DATA, sloupec, pocet_radku);
			  wait for 50 ns;		
			  
		end process tb;

	END behavior;
