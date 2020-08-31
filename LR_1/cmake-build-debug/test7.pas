program test7(intput, output);
  var A, B : Integer;
  T : Integer;
  begin
    a := 60;
    b := 100;
    while b > 0 do
      begin
        t := b;
        b := a mod b;
        a := t;
      end;
    Write(a);
  end.