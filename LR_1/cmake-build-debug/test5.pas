program test5(input, output);
var
  a: Boolean;
  b,b2: Integer;
  c: Real;
  arr: array [0 .. 7]of array [0 .. 3, 6 .. 10] of integer;
  rec: record
    i: integer;
    c: record
      a: integer;
      b: boolean;
      c: real;
      d: char;
    end; {end of record c}
    j,k: char;
  end; {end of record rec}
begin
  b := 5;
  b := b2;
  b := rec.i;
  rec.c.a := b;
  {b := arr[0][0][6];}
  a := 'a' = 'c';
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