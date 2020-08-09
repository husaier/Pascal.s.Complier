program test5(input, output);
var
  a: Boolean;
  b: Integer;
  c: Real;
begin
  a := 1 = 2;
  a := 1 <> 2;
  a := 1 < 2;
  a := 1 <= 2;
  a := 1 > 2;
  a := 1 >= 2;
  a := (1 < 2) and (1 = 2);
  a := (2 > 3) or (1 <> 1);
  a := not (2 > 3);
  b := 1 + 2 + 3 + 4 + 5;
  b := 1 div 3;
  b := 1 * 3;
  b := 1 mod 3;
  b := + 3;
  c := + 0.3;
  b := - 3;
  c := - 0.3;
  c := 1 / 3;
  a := 3 * (2 + 4) < 10;
end.