A = LOAD 'user-links-small.txt' AS(id1:int, id2:int);
B = GROUP A by $0;
C = FOREACH B GENERATE group, COUNT(A);
D = GROUP C by $1;
E = FOREACH D GENERATE group, COUNT(C);
STORE E INTO 'question1_output' USING PigStorage();
