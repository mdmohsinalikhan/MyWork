#include <stdio.h>

int sequenceno=10;
int no_of_edges=0;
int paths[1000][1000];
int current_length=0;
int current_path=0;

printresult()
{
int max_path_i,max_path_j;
int for_sorting[current_path];

//printf("This is from printresult. The value of current_path is:%d\n",current_path);

	for(max_path_i=0;max_path_i<current_path;max_path_i++)
	{
		for_sorting[max_path_i]=paths[max_path_i][0];
		//printf("\nHere is the path %d: ",max_path_i);
		for(max_path_j=0;max_path_j<paths[max_path_i][0]+2;max_path_j++)
		{
		//printf(" %d",paths[max_path_i][max_path_j]);		
		}
	//printf("\n");
	}

	
			int bubblesort_i,bubblesort_j,swap;
		
			for(bubblesort_i=0;bubblesort_i <current_path;bubblesort_i++)
			{
				for(bubblesort_j=0;bubblesort_j < current_path;bubblesort_j++)
				{				
					if(for_sorting[bubblesort_i] > for_sorting[bubblesort_j])
					{
					swap=for_sorting[bubblesort_i];
					for_sorting[bubblesort_i] = for_sorting[bubblesort_j];
					for_sorting[bubblesort_j] = swap;
					}
				}
			}
			
			for(bubblesort_i=0;bubblesort_i<current_path;bubblesort_i++)
			{
			//printf("%d ",for_sorting[bubblesort_i]);
			}
			//printf("\n");
			int max_path_length;

			max_path_length=for_sorting[0];
			
			for(max_path_i=0;max_path_i<current_path;max_path_i++)
			{
			//printf("\n At path:%d paths[Max_path_i][0]:%d	and 	max_path_length:%d",max_path_i,paths[max_path_i][0],max_path_length);
				if(paths[max_path_i][0]==max_path_length)
				{
				//printf("\nMatch found at the path:%d\n",max_path_i);
					for(max_path_j=2;max_path_j<paths[max_path_i][0]+2;max_path_j++)
					{
					printf("%d",paths[max_path_i][max_path_j]);
						if(max_path_j!=(max_path_length+2)-1)
						{
						printf(" ");
						} 						
					}
				printf("\n");
				break;
				}
			//printf("\n");
			}
			
};

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


void previsit_longest(int v)
{
//paths[0][0]=100;
//printf("\nPrevisit_longest function is called, now the current path:%d and current length:%d for the vertex:%d",current_path,current_length,v);
paths[current_path][current_length+2]= v;
//printf("\nPrevisit_longest function is called and the vertex inserted in path :%d",paths[current_path][current_length+2]);
current_length++;
paths[current_path][0]=current_length;
paths[current_path][1]=-1;
//printf("\n				Value of current_length variable at the end of previsit fuction:%d",current_length);

}

void postvisit_longest()
{
//printf("\n												This is from postvisit:%d",paths[0][0]);

//printf("\nPostvisit_longest\ncurrent paths is:%d\n	current_length:%d",current_path,current_length);
int m;
paths[current_path][1]= -10;
current_path++;
paths[current_path][0]=current_length-1;
current_length--;
//printf("\n In postvisit. Copying earlier path to the new path:");
		for(m=2;m < current_length+2;m++)
		{
		//printf("\ninserting :%d",paths[current_path-1][m]);
		paths[current_path][m]=paths[current_path-1][m];
		}

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



void dfs_longest_path(int vertices[][4],int edges[][4],int v)
{
int counter,i;
			//printf("The dfs_longest_path function is called for exploring the vertex %d\n",v);
			previsit_longest(v);
			if(vertices[v-1][1]!=1)
			{
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
					dfs_longest_path(vertices,edges,edges[i][1]);
					}				
				}
			} 
			postvisit_longest();
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
			//printf("The Vertices are: %d %d %d %d\n",vertices[counter][0],vertices[counter][1],vertices[counter][2],vertices[counter][3]);
			}
			*/
			
			for(counter=0;counter<no_of_edges;counter++)
			{
			//printf("The Edges are: %d %d %d %d\n",edges[counter][0],edges[counter][1],edges[counter][2],edges[counter][3]);
			}
			
			/**
			printf("The input char1 is: %s\n",input_char1);
			printf("The input char2 is: %s\n",input_char2);
			printf("The number of vertices are: %d\n",no_of_vertices);
			printf("The number of edges are: %d\n",no_of_edges);
			**/
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
					if(vertices[bubblesort_i][3] > vertices[bubblesort_j][3])
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
			
			int topological_order[no_of_vertices];
			for(counter=no_of_vertices-1;counter>=0;counter--)
			{
			//printf("After setting the pre value for vertex:%d %d %d %d\n",vertices[counter][0],vertices[counter][1],vertices[counter][2],vertices[counter][3]);
			//printf("%d",vertices[counter][0]);
			topological_order[counter]=vertices[counter][0];
//			dfs_longest_path(vertices,edges,vertices[counter][0]);		
				if(counter != 0)
				{
				//printf(" ");
				}
				else
				{
				//printf("\n");
				}
			}
			
		
			int max_path_i,max_path_j,max_path_k,max_u,max_v;
			int paths[no_of_vertices][no_of_vertices+2],path_length_counter;
			
			for(max_path_i=0;max_path_i<no_of_vertices;max_path_i++)
			{
				for(max_path_j=0;max_path_j<no_of_vertices+2;max_path_j++)
				{
				paths[max_path_i][max_path_j]=-1;
				}
			}
			
		/*	
			int tracker;
			max_path_i=0;
			int outer_loop=0;
		
		while (outer_loop<no_of_vertices)
		{		
		paths[outer_loop][0]=0;   //need change
		paths[outer_loop][1]=topological_order[outer_loop]; //need change
		max_path_i=outer_loop;
			
			while(max_path_i < no_of_vertices)
			{
			tracker = 0;
			path_length_counter=2;
					for(max_path_j=max_path_i+1;max_path_j<no_of_vertices;max_path_j++)
					{
					tracker = 0;
					printf("I J Value: %d %d\n",max_path_i,max_path_j);
					max_u = topological_order[max_path_i];
					max_v = topological_order[max_path_j];
					printf("U V Value: %d %d\n",max_u,max_v);
							for(max_path_k=0;max_path_k<no_of_edges;max_path_k++)
							{
								if(edges[max_path_k][0]==max_u && edges[max_path_k][1]==max_v)
								{
								printf("\n Matching found edges[%d][0] and edges[%d][1] that is max_u:%d and max_v:%d",max_path_k,max_path_k,max_u,max_v);
								paths[outer_loop][0]++; //need change in row index
								printf("\n Going to insert max_v:%d in paths matrixs [0][%d] location\n",max_v,path_length_counter);
								paths[outer_loop][path_length_counter] = max_v; //need change in row index
								path_length_counter++;
								max_path_i=max_path_j;
								tracker++;
								}
							}
					}
					
					if(tracker==0)
					{
					//max_path_i++; 
						//if(max_path_i++>=5)
						//{
							break;
						//}
					}
			}
		outer_loop++;
		}	
	
			
	for(max_path_i=0;max_path_i<no_of_vertices;max_path_i++)
	{
		printf("Here is the path:");
		for(max_path_j=0;max_path_j<no_of_vertices+2;max_path_j++)
		{
		printf(" %d",paths[max_path_i][max_path_j]);
		}
	printf("\n");
	}

**/		
	
//printf("Topological order: %d %d %d %d %d %d %d %d %d %d %d %d %d:",topological_order[0],topological_order[1],topological_order[2],topological_order[3],topological_order[4],topological_order[5],topological_order[6],topological_order[7],topological_order[8],topological_order[9],topological_order[10],topological_order[11],topological_order[12]);

	for(max_path_i=0;max_path_i<no_of_vertices;max_path_i++)
	{
		//printf("\nHere are the vertices:");
		for(max_path_j=0;max_path_j<4;max_path_j++)
		{
		//printf(" %d",vertices[max_path_i][max_path_j]);
		vertices[max_path_i][1]=0;
		}
	//printf("\n");
	}
	
	
	//printf("\nHere is the lenght of the array topological order:%d",len(topological_order));
	
//printf("\nGoing to call the dfs longest path function");


int q;
for(q=1;q<=no_of_vertices;q++)
{
//dfs_longest_path(vertices,edges,topological_order[0]);
dfs_longest_path(vertices,edges,q);		
for(max_path_i=0;max_path_i<no_of_vertices;max_path_i++)
	{
		//printf("\nHere are the vertices:");
		for(max_path_j=0;max_path_j<4;max_path_j++)
		{
		//printf(" %d",vertices[max_path_i][max_path_j]);
		vertices[max_path_i][1]=0;
		}
	//printf("\n");
	}
//dfs_longest_path(vertices,edges,1);
}
//printf("\n The current_path and length is variable is: %d	%d",paths[12][3],paths[12][2]);

printresult();
	
return 0;

}


