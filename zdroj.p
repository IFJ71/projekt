var
   a : integer; { komentar }
   b : real;

begin
   a := 7;
   a++;

   if (a <= 5) then
      a--;
   else if (a >= 10) then
      b := a * 0.1;
   else if (a <> 8) then
      a := a + 2;
end.
