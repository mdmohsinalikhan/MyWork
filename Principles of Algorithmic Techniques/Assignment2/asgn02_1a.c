#include <stdio.h>

int sequenceno=10;
int no_of_edges;

void previsit(int vertices[][4],int v)
{
vertices[v-1][2]=sequenceno;
sequenceno++;
}

void postvisit(int vertices[][4],int v)
{
vertices[v-1][3]=sequenceno;
sequenceno++;
}

void dfs(int vertices[][4],int edges[][4],int v)
{
int counter,i;
			//printf("The dfs function is called for exploring the vertex %d\n",v);
			if(vertices[v-1][1]!=1)
			{
			previsit(vertices,v);
			vertices[v-1][1]=1;
				for(counter=0;counter<5;counter++)
				{
				//printf("After setting the pre value for vertex:%d	: %d %d %d %d\n",v,vertices[counter][0],vertices[counter][1],vertices[counter][2],vertices[counter][3]);
				}
		
				for(i=0;i<no_of_edges;i++)
				{
					if(edges[i][0]==v)
					{
					//printf("\nRecursion Vertex %d has an edge to vertex:%d	",edges[i][0],edges[i][1]);
					dfs(vertices,edges,edges[i][1]);
					}				
				}
			postvisit(vertices,v);
			} 
}

int main(int argc, char *argv[])
{
int counter,input_value,no_of_vertices;
char input_char1[2];
char input_char2[20];

no_of_vertices=0;


FILE *fp;

if(argc == 2)
{
		fp = fopen(argv[1],"r");
				
		fscanf(fp,"%s",input_char1);
		fscanf(fp,"%s",input_char2);
		fscanf(fp,"%d",&input_value);
		no_of_vertices=input_value;
		fscanf(fp,"%d",&input_value);
		no_of_edges=input_value;
		
		fclose(fp);
		
}

			int vertices [no_of_vertices][4];
			int edges[no_of_edges][4];
			
			
			
			for(counter=0;counter<no_of_vertices;counter++)
			{
			vertices[counter][0]=counter+1;
			vertices[counter][1]=0;
			vertices[counter][2]=0;
			vertices[counter][3]=0;
			}
		
			
			fp = fopen(argv[1],"r");
		
		
			fscanf(fp,"%s",input_char1);
			fscanf(fp,"%s",input_char2);
			fscanf(fp,"%d",&input_value);
			no_of_vertices=input_value;
			fscanf(fp,"%d",&input_value);
			no_of_edges=input_value;
			
			for(counter=0;counter<no_of_edges;counter++)
			{
			fscanf(fp,"%s",input_char2);
			fscanf(fp,"%d",&input_value);
			edges[counter][0]=input_value;
			fscanf(fp,"%d",&input_value);
			edges[counter][1]=input_value;
			edges[counter][2]=0;
			edges[counter][3]=0;
			}
		
			
			fclose(fp);
			
			/*
			for(counter=0;counter<no_of_vertices;counter++)
			{
			printf("The Vertices are: %d %d %d %d\n",vertices[counter][0],vertices[counter][1],vertices[counter][2],vertices[counter][3]);
			}
			
			for(counter=0;counter<no_of_edges;counter++)
			{
			printf("The Edges are: %d %d %d %d\n",edges[counter][0],edges[counter][1],edges[counter][2],edges[counter][3]);
			}
			
			printf("The input char1 is: %s\n",input_char1);
			printf("The input char2 is: %s\n",input_char2);
			printf("The number of vertices are: %d\n",no_of_vertices);
			printf("The number of edges are: %d\n",no_of_edges);
			*/
int j;

		for(j=0;j<no_of_vertices;j++)
		{
			for(counter=0;counter<no_of_vertices;counter++)
			{
			//printf("After setting the pre value for vertex:%d %d %d %d\n",vertices[counter][0],vertices[counter][1],vertices[counter][2],vertices[counter][3]);
			}
				
			if(vertices[j][1]==0)
			{
			//printf("From main function Going to call the dfs function for the vertex %d",vertices[j][0]);
			dfs(vertices,edges,vertices[j][0]);
			}		
		}
		
		int u,v,k,upostval,vpostval;

		for(j=0;j<no_of_edges;j++)
		{
			u = edges[j][0];
			v = edges[j][1];
			
			for(k=0;k<no_of_vertices;k++)
			{
				if(u==vertices[k][0]) 
				{
				upostval = vertices[k][3];
				}
				if(v==vertices[k][0])
				{
				vpostval = vertices[k][3];
				}
			}
			
			if (upostval < vpostval) 
			{
			printf("cyclic\n");
			return 0;
			}
		}
		
		int bubblesort_i,bubblesort_j,swap;
		
			for(bubblesort_i=0;bubblesort_i < no_of_vertices;bubblesort_i++)
			{
				for(bubblesort_j=0;bubblesort_j < no_of_vertices;bubblesort_j++)
				{				
					if(vertices[bubblesort_i][3] < vertices[bubblesort_j][3])
					{
					swap=vertices[bubblesort_i][3];
					vertices[bubblesort_i][3] = vertices[bubblesort_j][3];
					vertices[bubblesort_j][3] = swap;
					
					swap=vertices[bubblesort_i][0];
					vertices[bubblesort_i][0] = vertices[bubblesort_j][0];
					vertices[bubblesort_j][0] = swap;
					
					swap=vertices[bubblesort_i][1];
					vertices[bubblesort_i][1] = vertices[bubblesort_j][1];
					vertices[bubblesort_j][1] = swap;
					
					swap=vertices[bubblesort_i][2];
					vertices[bubblesort_i][2] = vertices[bubblesort_j][2];
					vertices[bubblesort_j][2] = swap;
					}
				}
			}
			
					
			for(counter=no_of_vertices-1;counter>=0;counter--)
			{
			//printf("After setting the pre value for vertex:%d %d %d %d\n",vertices[counter][0],vertices[counter][1],vertices[counter][2],vertices[counter][3]);
			printf("%d",vertices[counter][0]);
			
				if(counter != 0)
				{
				printf(" ");
				}
				else
				{
				printf("\n");
				}
			}
		
return 0;

}


