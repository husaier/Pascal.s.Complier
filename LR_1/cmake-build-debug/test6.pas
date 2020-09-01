{快速排序}
program test9(input, output);
  var a,b :Integer;
  procedure initial();
    begin
      a := 20;
      b := 9;
    end;{end of intital}

  procedure print();
    var i :Integer;
      begin
          WriteLn(a,b);
      end;{end of print}

  procedure exchange(i, j: Integer);
    begin
      t := num[i];
      num[i] := num[j];
      num[j] := t;
    end;{end of exchange}

  procedure qSort(m, n: Integer);
    var k: Integer;
    function partit(l, r: Integer) : Integer;
      var i, j, mid: Integer;
        begin
          mid := l;
          i := l;
          j := r;
          while i < j do
            begin
              while (i < j) and (num[j] >= num[mid]) do
                j := j - 1;
              while (i < j) and (num[i] <= num[mid]) do
                i := i + 1;
              if i < j then
                begin
                  exchange(i, j);
                  i := i + 1;
                  j := j - 1;
                end;
              if i = j then
                exchange(mid, i);
          end;
          partit := i;
        end;{end of partit}

    begin
      if n > m then
        begin
          k := partit(m, n);
          qSort(m, k - 1);
          qSort(k + 1, n);
        end;{end of if}
    end;{end of qSort}

  begin
    initial();
    qSort(0, 9);
    print();
  end.{end of program}
