program test4(input, output);
    const pai = 3.14; {常量声明}
          pai2 = pai; {常量id声明}
          pai3 = pai4; {错误,常量不存在id声明}
    type list = array [0 .. 10] of integer; {类型声明}
    var a,b,c: integer; {变量声明}
        e,f: real;
        g,h: char;
        i,j: boolean;
        i: char; {语义错误，重复声明}
        rec: record
                i: integer;
                c: record
                    a: integer;
                    b: boolean;
                    c: real;
                    d: char
                    end; {end of record c}
                j,k: char
             end; {end of record rec}
        arr: array [0 .. 7, 0 .. 3] of integer;
        brr: array [0 .. 7]of array [0 .. 3, 6 .. 10] of integer;

    function func1 ( var id2 : real ;var id4 : real) : integer ; {Subprogram声明}
            procedure proc1 (var id3 : integer;var id4 : real) ;
                const pai4 = a; {错误,常量声明,id不为常量}
                      pai6 = pai4;
                function func2 ( var id2 : real) : integer ;
                    begin C:=pi*2; end ; {end of func2}
                begin C:=pi*2; end ; {end of proc1}
            begin
                C:=pi*2; end ; {end of func1}
    procedure d1p1(var m: char);
        var a,b: char;
        begin a := b; end; {end of d1p1}
    procedure d1p2(var n: real);
        var c,b,d: boolean;
        procedure d2p1(var q: boolean);
            var k,m: integer;
            begin k := 2; end; {end of d2p1}
        begin
            c := true;
            d2p1(c);
        end; {end of d1p2}
    begin
        w := 3; {语义错误，未定义的变量}
        a := 3;
        b := 4;
        c := 5;
        a := 4 * 7; {正确}
        a := 0.2 * 3; {错误，REAL赋值给了INTEGER}
        rec := 4 * 7; {错误，INTEGER赋值给了RECORD}
        a := 'c' * 7; {错误，运算对象类型错误，这时表达式的类型为VOID}
        e := 4 / 2; {正确}
        e := 4 / 0; {除0错误}
        e := 4.0 / 2.0; {正确}
        a := 5 mod 2; {正确}
        a := 5 mod 0; {除0错误}
        a := 4 div 2; {正确}
        a := 4 div 0; {除0错误}
        i := ( 3 = 4 ); {正确}
        i := ( 'a' < 6 ); {操作数类型错误}
        i := rec.i + 1; {赋值类型错误}
        i.a := 4; {错误,i不是一个记录类型}
        i := rec.abc; {错误,记录rec中没有abc领域}
        i := rec.c.b; {正确}
        rec.i := 4; {正确}
        arr[0][0] := brr[0][0][6]; {正确}
        arr[0] := brr[0][0][6]; {错误，arr[0]是一个数组，不能直接参与运算}
    end. {end of test}