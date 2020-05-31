Program sort(input, output);
    var a: array[0..10] of integer;
        x : integer;
    procedure readarray;
        var i: integer;
        begin for i: =1 to 9 do read(a(i]) end;
    procedure exchange(i,i: integer);
        begin x: =a[i], a[i]: =a[j]; a[j]: =x end;
    procedure quicksort(m, n: integer);
        var k, v: integer;
        function partition(y, z: integer): integer;
            var i,j: integer;
            begin
                {a;}
                {v;}
                exchange(i, j);
             end; { end of partition }
        begin
            if (n>m)then
                begin
                    i: =partition (m, n);
                    quicksort(m, i-1);
                    quicksort (i+l, n);
                end
        end {end of quicksort}
    begin
        a[0]=-999;a[10]=999;
        readarray;
        quicksort(1, 9)
    end. {end of sort}