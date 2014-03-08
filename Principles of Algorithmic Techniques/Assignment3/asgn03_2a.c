#include <stdio.h>

float vertices [100000][5];

void print_placements(int inputnode);


void print_placements(int inputnode)
{
int i;

	i=(int)vertices[inputnode][4];

	if(i!=0)
	{	
	print_placements(i-1);
	}

	printf(" %d",(int)vertices[inputnode][1]);

}


int main(int argc, char *argv[])
{
int counter,min_distance,no_of_possible_installation,no_of_vertices,i,j,no_of_edges,temp_step;
float input_value,temp_profit;

no_of_edges=0;
temp_profit=0;

FILE *fp;

			
			fp = fopen(argv[1],"r");

			fscanf(fp,"%d",&min_distance);
			fscanf(fp,"%d",&no_of_possible_installation);
			no_of_vertices=no_of_possible_installation;
		
			//printf("The number of vertices are: %d\n",no_of_vertices);
			//printf("The minimum distance in between two installation: %d\n",min_distance);

			for(counter=0;counter<no_of_vertices;counter++)
			{
			vertices[counter][0]=counter+1;
			vertices[counter][1]=0;
			vertices[counter][2]=0;
			vertices[counter][3]=0;
			vertices[counter][4]=0;
			//vertices[counter][5]=0;
			}
		
					
			for(counter=1;counter<3;counter++)
			{
				for(i=0;i<no_of_vertices;i++)
				{
				fscanf(fp,"%f",&input_value);
				vertices[i][counter]=input_value;
				}
			}
		
			fclose(fp);

/*
			for(counter=0;counter<no_of_vertices;counter++)
			{
			printf("The Vertices are: %f %f %f %f\n",vertices[counter][0],vertices[counter][1],vertices[counter][2],vertices[counter][3]);
			}

*/
			for(i=0;i<no_of_vertices-1;i++)
			{
				for(j=i;j<no_of_vertices;j++)
				{
					if(vertices[j+1][1]-vertices[i][1]>=min_distance)
					{
					no_of_edges++;
					}
				}
			}

			//printf("The number of edges are: %d\n",no_of_edges);

			int edges[no_of_edges][2];

			no_of_edges=0;
			for(i=0;i<no_of_vertices-1;i++)
			{
				for(j=i;j<no_of_vertices;j++)
				{
					if(vertices[j+1][1]-vertices[i][1]>=min_distance)
					{
					edges[no_of_edges][0] = (int)vertices[i][0];
					edges[no_of_edges][1] = (int)vertices[j+1][0];
					no_of_edges++;
					}
				}
			}

/*
			printf("The edges are:\n");
			for(i=0;i<no_of_edges;i++)
			{
			printf("%d %d\n",edges[i][0],edges[i][1]);	
			}

*/
			vertices[0][3]= vertices[0][2];

			printf("#1 profit: %f\n",vertices[0][3]);
			printf("placements: %d\n",(int)vertices[0][1]);

			for(i=1;i<no_of_vertices;i++)
			{
			//printf("\n at vertex %f ",vertices[i][0]);
//				vertices[i][3]= vertices[i][2];
				for(j=0;j<no_of_edges;j++)
				{
					if(edges[j][1] == (int)vertices[i][0])
					{
					//printf("\n checking edge %d: %d %d",j,edges[j][0],edges[j][1]);
						if(vertices[i][3] < vertices[edges[j][0]-1][3])
						{
						//printf("\n rank of %f is %f",vertices[edges[j][0]-1][0],vertices[edges[j][0]-1][3]);
						vertices[i][3] = vertices[edges[j][0]-1][3];
						vertices[i][4] = vertices[edges[j][0]-1][0];
						}
					//printf("\n rank of %f is %f",vertices[i][0],vertices[i][3]);
					}
				}

			vertices[i][3]=vertices[i][3]+vertices[i][2];
			//vertices[i][5]=i+1;
					//printf("\nTemp profit and temp step before checking: %f and %d",temp_profit,temp_step);
					if(temp_profit < vertices[i][3]){temp_profit=vertices[i][3];temp_step=i;}
					//printf("\nTemp profit and temp step after checking: %f and %d",temp_profit,temp_step);
			printf("#%d profit: %.3f\n",i+1,vertices[i][3]);
			printf("placements:");
			print_placements(i);
			printf("\n");
			}

			printf("#%d profit: %.3f\n",temp_step+1,vertices[temp_step][3]);
			printf("placements:");
			print_placements(temp_step);
			printf("\n");

			for(counter=0;counter<no_of_vertices;counter++)
			{
			//printf("The Vertices are: %f %f %f %f %f\n",vertices[counter][0],vertices[counter][1],vertices[counter][2],vertices[counter][3],vertices[counter][4]);
			}

			
			
							
return 0;

}


