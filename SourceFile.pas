PROGRAM yuan (i);
var R,S,C:real;
    K,a,b:integer;
    FLOAT:real;
    W:array[0 .. 4]of char;
    N:array[0 .. 4, 2 .. 7]of integer;
    M:array[0 .. 4]of array [2 .. 7] of integer;
    rec: record
    	i: integer;
        j,k: char
    end; {end of record}
begin
 write('请输入圆的半径');
 readln(R);
 if R<5 
 then write('半径小于5')
 else write('半径大于或等于5');
 S:=pi*r*r;
 C:=pi*r*2;
 writeln('圆的面积为',s);
 writeln('圆的周长为',c);
 W:='fwea';
 writeln(w);
 writeln(w[0]);
 writeln(w[1]);
 writeln(w[2]);
 writeln(w[3]);
 writeln(w[4]);
 {writeln(w[5]);}
 {N[4] := 1;}
 {N[5] := 2;}
 N[4][7] := 2;
 N[4, 7] := 1;
 {N[1.2, 5] := 0;}
 M[4][7] := 3;

 rec.i := 4;
 rec.j := 'a';

 {k := 4 mod 0; 除零错误，不允许}
 k := 0 div 3; {允许，输出是0}
 {k := 0.2 div 3; 不允许，类型错误，div运算符两边必须是INTEGER类型}
 writeln(k);
 {k := 3 div 0; 不允许，除零错误}
 float := 3 / 0; {允许，输出是 +Inf}
 writeln(float);
 {k := 4 / 6; k为integer，表达式运算结果为REAL，不允许}
 a := + 123;
 writeln(a);
 b := - 123;
 writeln(b);
 float := +1.5;
 writeln(float);
 float := -1.5;
 writeln(float);
 readln;
END.
