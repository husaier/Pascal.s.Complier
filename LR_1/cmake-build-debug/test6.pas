program test6(intput, output);
var
  B : Integer;
  function d1p1(var a : integer): integer;
          begin
           Write(a);
           d1p1 := a;
          end; {end of d1p1}
  begin
    b := d1p1(60);
    Write(b);
  end.