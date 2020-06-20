PROGRAM yuan (i);
var R,S,C:real;
    W:array[0 .. 4]of char;
    N:array[0 .. 4, 2 .. 7]of integer;
    M:array[0 .. 4]of array [2 .. 7] of integer;
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
 M[4][7] := 3;
 readln;
END.
