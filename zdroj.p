var
   a : integer; { komentar }
   b : real;
   c : string;

function fact(num : integer) : integer; forward;

function fact(num : integer) : integer;
var
   temp_result : integer;
   decremented_num : integer;
begin
   if n < 2 then
   begin
      fact := 1
   end
   else
   begin
      decremented_num := num - 1;
      temp_result := fact(decremented_num);
      fact := n * temp_result
   end
end;

begin
   write('Zadejte cislo pro vypocet faktorialu: ');
   readln(a);
   
   if a < 0 then
   begin
      write('Faktorial nelze spocitat'#10'')
   end
   else
   begin
      a := fact(a);
      write('Vysledek je: ', vysl, ''#10'')
   end
   
   b := 31.415 * 0.1;

   c := 'Ahoj'#010'Sve''te'#30''
end.
