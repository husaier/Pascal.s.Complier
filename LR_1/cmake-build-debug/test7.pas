program test7(intput, output);
  var A, B : Integer;
  T : Integer;
  arr2: array [0 .. 7]of integer;
  arr: array [0 .. 7]of array [0 .. 3, 6 .. 10] of integer;
  function d1p1(var a : integer;var b : integer;var t : integer): integer;
          begin
           while b > 0 do
              begin
                t := b;
                b := a mod b;
                a := t;
              end;
           Write(a,b);

          end; {end of d1p1}
  begin
    arr2[5] := 60;
    arr[2][1][2]:= 100;
    t := d1p1(arr2[5],arr[2,1,2],t);
  end.



