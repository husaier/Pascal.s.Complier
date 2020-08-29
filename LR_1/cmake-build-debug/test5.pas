program test5(input, output);
{const pai = 3.14; {常量声明}
{      pai2 = pai; {常量id声明}
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

  for b := 1 to 5 do
    b2 := 1;

  b := b + c;
  b := b2;
  b := rec.i;
  rec.c.a := b;
{  b := arr[0][0][6];
  arr[b][rec.c.a][6] := b;}
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
  a := b=2;
  if (1 = 1) then
  begin
    b := 1;
    c := 1.2;
    c := 1.3;
  end;
  if a then
    b := 1
  else
    b := 2;

  case b of
    1 : b2 := 1;
    2 : b2 := 2;
    3 :
    begin
      b2 := 3;
      b2 := 4;
    end {这里不能加分号否则会异常}
  end;

  while b<5 do
  begin
    b := b+1;
    c := c+0.1;
  end;

  repeat b := b+1 until (b > 10);


end.