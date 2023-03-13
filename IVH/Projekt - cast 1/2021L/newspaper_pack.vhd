 -- Samuel Kuchta (xkucht11)
  
library IEEE;
use IEEE.STD_LOGIC_1164.all;

package newspaper_pack is
	type DIRECTION_T is (DIR_LEFT, DIR_RIGHT); -- výètový typ
	function GETCOLUMN (signal DATA : in std_logic_vector; COLID : in integer; ROWS : in integer) return std_logic_vector;
end newspaper_pack;

package body newspaper_pack is

	function GETCOLUMN (signal DATA : in std_logic_vector; COLID : in integer; ROWS : in integer) return std_logic_vector is
	
		variable col_max : integer := (DATA'length / ROWS) - 1;
		variable vector : std_logic_vector(0 to ROWS-1);
		
	begin
	
		if COLID > col_max then
			vector := DATA(0 to ROWS-1);  -- 
		elsif COLID < 0 then
			vector := DATA(DATA'length - ROWS to DATA'length - 1);
		else
			vector := DATA(ROWS*COLID to (ROWS*(COLID+1))-1);
		end if;
		
		return vector;
		
	end;
end newspaper_pack;

