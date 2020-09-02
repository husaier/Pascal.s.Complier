program test10(input, output);
  var a, b: Integer;
  function gcd(var i, j: Integer): Integer;
    var t : Integer;
    begin
      t := i mod j;
      if t = 0 then
        t := j
      else
        t := gcd(j, t);
      gcd := t;
    end;
  begin
    a := 60;
    b := 100;
    WriteLn(gcd(a, b));
  end.