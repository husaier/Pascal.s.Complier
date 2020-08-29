program test5(input, output);
const pai = 3.14; {常量声明}
      pai2 = pai; {常量id声明}
var
  a: Boolean;
  b,b2,b3: Integer;
  c,c2: Real;
  ch: Char;
  arr: array [0 .. 7]of array [0 .. 3, 6 .. 10] of integer;
  arr2: array [0 .. 7]of integer;
rec,rec2: record
    i: integer;
    c: record
      a: integer;
      b: boolean;
      c: real;
      d: char;
    end; {end of record c}
    j,k: char;
  end; {end of record rec}

function func1 ( var id2 : real ;var id4 : real) : integer ; {Subprogram声明}
  procedure proc1 (var id3 : integer;var id4 : real) ;
    const pai6 = pai;
    function func2 ( var id2 : real) : integer ;
      begin C := pai * 2; end ; {end of func2}
    begin C := pai * 2; end ; {end of proc1}
  begin
    C := pai * 2; end ; {end of func1}

procedure d1p1;
  var a,b: char;
  begin a := b; end; {end of d1p1}

procedure d1p2(var n: integer);
    var c,b,d: boolean;
    procedure d2p1(var q: boolean);
      var k,m: integer;
      begin k := 2; end; {end of d2p1}
    begin
      c := (1 = 1);
      d2p1(c);
    end; {end of d1p2}

procedure d1p3(var m: char;n:real);
  var a,b: char;
begin a := b; end; {end of d1p1}

begin
  b := 111111111111;
  b := arr2[2] + arr2[1];
  arr2[arr2[2]] := 1;
  b := arr[b+1][0,6];
  arr[b][rec.c.a][6] := b;
  d1p1;
  d1p3(ch,c);
  b := 6789876;
  b := func1(c,c2);
  b := 3333333333;
  d1p2(func1(c,c2));
  repeat
    repeat b := b+1 until (b > 10);
  until (b2 > 10);

  while b<5 do
    while b2<5 do
      {begin}
      b2 := b2 + 1;
      {b3 := b3 + 1;
      end;}
    {b3 := b3+2;}

  for b := 1 to 5 do
    for b2 := 5 downto 1 do
      b3 := 1;

  b := 5;
  b := b2;
  b := rec.i;
  rec.c.a := b;

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

  repeat
    repeat b := b+1 until (b > 10);
  until (b > 10);


end.