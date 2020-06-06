program test(input, output);
    var a,b,c: integer;
        e,f: real;
    procedure d1p1(var m: char);
        var a,b: char;
        begin a := b; end; {end of d1p1}
    procedure d1p2(var n: real);
        var c,b,d: boolean;
        procedure d2p1(var q: boolean);
            var k,m: integer;
            begin k := 2; end; {end of d2p1]
        begin
            c := true;
            d2p1(c);
        end; {end of d1p2}
    begin
        a := 3;
        b := 4;
        c := 5;
    end. {end of test}
