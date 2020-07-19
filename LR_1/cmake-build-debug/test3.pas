program test3(input, output);
    var a,b,c: integer;
        e,f: real;
        arr: array [0 .. 7, 0 .. 3] of boolean;
        brr: array [6 .. 10]of array [0 .. 7, 0 .. 3] of boolean;
        crr: array [6 .. 10]of array [0 .. 7, 0 .. 3] of
            record
                a: integer;
                c: char;
                i: boolean;
            end; {end of record}
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
        arr[0][0] := 3 = 4;
        arr[0][0] := 3 <> 4;
        arr[0][0] := 3 < 4;
        arr[0][0] := 3 <= 4;
        arr[0][0] := 3 > 4;
        arr[0][0] := 3 >= 4;
        a := +3;
        a := -3;
        a := 3 + 3;
        e := 3 + 0.1;
        a := 3 - 3;
        e := 3 - 0.1;
        arr[0][0] := (3 = 3) or (3 <> 3);
        a := 3 * 4;
        e := 3 * 0.4;
        e := 3 / 0.4;
        a := 7 div 2;
        a := 7 mod 2;
        arr := brr[6];
        arr[0][0] := (3 = 3) and (3 <> 3);
        arr[0][0] := arr[0][1] and brr[6][0][0];
        arr[0][0] := not (3 = 3);

        crr[6][0][0].a := 6;
        crr[6][0][0].c := 'a';
        crr[6][0][0].i := 1 = 1;

        rec.i := 5;
        rec.c.a := 3;
        rec.c.b := 1 = 1;
        rec.c.c := 5 / 3;
        rec.c.d := 'c';
        rec.j := 'j';
        rec.k := 'k';
        c := 5;
    end. {end of test}