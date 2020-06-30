program test(input, output);
    const pai = 3.14; {常量声明}
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
        arr: array [0 .. 7, 0 .. 3] of char;
        brr: array [0 .. 7]of array [0 .. 3, 6 .. 10] of integer;
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
        rec.i := 4 {正确}



    end. {end of test}