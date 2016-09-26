fid = fopen('T-61_3050_data_set.txt');
A = fscanf(fid, '%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f', [16 inf]);
fclose(fid);

A = A';

x = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];
xsquare = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];
xvar = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];
max = 0;
secondmax = 0;
maxi = 0;
secondmaxi = 0;

for i=1:16,
    for j=1:4000
        x(i) = x(i) + A(j,i);
        xsquare(i) = xsquare(i) + A(j,i)^2;
    end
xvar(i) = xsquare(i)/4000 - (x(i)/4000)^2;
if xvar(i) > max
    secondmax = max;
    secondmaxi = maxi;
    max = xvar(i);
    maxi = i;
end
end

maxvariancecolumn = A(:,maxi);
secondmaxvariancecolumn = A(:,secondmaxi);

plot(secondmaxvariancecolumn,maxvariancecolumn,'o');
