program test7(intput, output);
  var A, B : Integer;
  T : Integer;

  function d1p1(var a : integer;var b : integer;var t : integer): integer;
          begin
           {while b > 0 do
              begin
                t := b;
                b := a mod b;
                a := t;
              end;
           Write(a,b);}
           d1p1 := a;
          end; {end of d1p1}
  begin
    a := 60;
    b := 100;
    t := d1p1(a,b,t);
    Write(t);
  end.
