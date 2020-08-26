program test5(input, output);
const pai = 3.14; {常量声明}
      pai2 = pai; {常量id声明}
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
  a := 2=2;
{  if (1 = 1) then
    begin
      b := 1;
      c := 1.2;
      c := 1.3;
    end;}{这里begin和end会导致异常,貌似是relocate的问题}
  if a then
      b := 1
  else
      b := 2;

  case b of
    1 : b2 := 1;
    2 : b2 := 2{这里不能加分号否则会异常}
  end;

  b := 5;
  b := b2;
  b := rec.i;
  rec.c.a := b;
  b := arr[b][0][6];
  arr[b][rec.c.a][6] := b;
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