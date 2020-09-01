{冒泡排序}
program test8(input, output);
  var num : array[0 .. 10] of Integer;
  n, t, i, j : Integer;
  procedure initial();
    begin
      num[0] := 20;
      num[1] := 9;
      num[2] := 11;
      num[3] := 4;
      num[4] := 17;
      num[5] := 12;
      num[6] := 6;
      num[7] := 16;
      num[8] := 2;
      num[9] := 8;
    end;
  begin
    n := 10;
    initial();
    for i := 9 downto 1 do
      for j := 0 to i - 1 do
          if num [j] > num[j + 1] then
            begin
              t := num[j];
              num[j] := num[j + 1];
              num[j + 1] := t;
            end;
    WriteLn(num[0]);
    WriteLn(num[1]);
    WriteLn(num[2]);
    WriteLn(num[3]);
    WriteLn(num[4]);
    WriteLn(num[5]);
    WriteLn(num[6]);
    WriteLn(num[7]);
    WriteLn(num[8]);
    WriteLn(num[9]);
  end.