Friendship = LOAD 'user-links-small.txt' AS(id1:int,id2:int);
Friendship_grouped = GROUP Friendship by $0;
Friendship_user_count = FOREACH Friendship_grouped GENERATE group as id1, COUNT(Friendship) as degree;

Wall_post = LOAD 'user-wall-small.txt' AS(id1:int,id2:int,timestamp:int);
Wall_post_grouped = GROUP Wall_post by $0;
Wall_post_user_count = FOREACH Wall_post_grouped GENERATE group as id1,COUNT(Wall_post) as postcount;

Friendship_count_post_count = JOIN Friendship_user_count BY id1 LEFT OUTER, Wall_post_user_count BY id1;
Friendship_count_post_count_grouped = GROUP Friendship_count_post_count BY $1;
Friendship_deg_post_dist = FOREACH Friendship_count_post_count_grouped GENERATE group as degree,(float)SUM(Friendship_count_post_count.postcount)/(float)COUNT_STAR(Friendship_count_post_count.postcount) as dist;
X = FILTER Friendship_deg_post_dist BY dist is null;
Y = FOREACH X GENERATE degree, 0 as dist;
Z = FILTER Friendship_deg_post_dist BY dist is not null;
W = UNION ONSCHEMA Y,Z;
U = FOREACH W GENERATE degree,dist;
V = ORDER U by degree;
STORE V INTO 'question2_output' USING PigStorage();
