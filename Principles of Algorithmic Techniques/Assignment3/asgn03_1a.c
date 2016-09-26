#include <stdio.h>


int main(int argc, char *argv[])
{
int counter,vertex_cover_counter,input_value,no_of_vertices,no_of_edges,vertex_cover_candidate,vertex_cover_rank,i,no_of_vetices_rank_become_zero;
char input_char1[2];
char input_char2[20];

no_of_vertices=0;
vertex_cover_counter=0;
no_of_vetices_rank_become_zero=0;


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

			printf("The input char1 is: %s\n",input_char1);
			printf("The input char2 is: %s\n",input_char2);
			printf("The number of vertices are: %d\n",no_of_vertices);
			printf("The number of edges are: %d\n",no_of_edges);

			int vertices [no_of_vertices][2];
			int edges[no_of_edges][2];
			int vertex_cover[500];
				
			
			for(counter=0;counter<no_of_vertices;counter++)
			{
			vertices[counter][0]=counter+1;
			vertices[counter][1]=0;
			//vertices[counter][2]=0;
			//vertices[counter][3]=0;
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
			//edges[counter][2]=0;
			//edges[counter][3]=0;
			}
		
			
			fclose(fp);

			for(counter=0;counter<no_of_edges;counter++)
			{
			printf("The Edges are: %d %d\n",edges[counter][0],edges[counter][1]);
			vertices[edges[counter][0]-1][1]++;
			vertices[edges[counter][1]-1][1]++;
			}
						
			for(counter=0;counter<no_of_vertices;counter++)
			{
			printf("The Vertices are: %d %d\n",vertices[counter][0],vertices[counter][1]);
			}

			/* find out the vertex which has the maximum rank */

			while(no_of_vetices_rank_become_zero!=no_of_vertices)
			{

				vertex_cover_rank=0;
				for(i=0;i<no_of_vertices-1;i++)
				{
					if(vertex_cover_rank < vertices[i][1])
					{
					vertex_cover_rank=vertices[i][1];
					vertex_cover_candidate=vertices[i][0];
					}
				}

				printf("\nThe vertex cover candidate is:%d and the rank of the vertex is: %d\n",vertices[vertex_cover_candidate-1][0],vertices[vertex_cover_candidate-1][1]);
				vertex_cover[vertex_cover_counter]=vertex_cover_candidate;
				vertex_cover_counter++;


				for(counter=0;counter<no_of_edges;counter++)
				{
				//printf("The Edges are: %d %d\n",edges[counter][0],edges[counter][1]);

					if(edges[counter][0] == vertex_cover_candidate || edges[counter][1] == vertex_cover_candidate)
					{
						if(vertices[edges[counter][0]-1][1]!=0){vertices[edges[counter][0]-1][1]--; if(vertices[edges[counter][0]-1][1]==0){no_of_vetices_rank_become_zero++;}}
						if(vertices[edges[counter][1]-1][1]!=0){vertices[edges[counter][1]-1][1]--; if(vertices[edges[counter][1]-1][1]==0){no_of_vetices_rank_become_zero++;}}
					}
				}

				for(counter=0;counter<no_of_vertices;counter++)
				{
				printf("The Vertices are: %d %d\n",vertices[counter][0],vertices[counter][1]);
				}

					if(no_of_vetices_rank_become_zero!=no_of_vertices)
					{
					printf("The Intermediate solution  is:");
					}

					if(no_of_vetices_rank_become_zero==no_of_vertices)
					{
					printf("Cover:");
					}
							

					for(i=0;i<vertex_cover_counter;i++)
					{
					printf(" %d",vertex_cover[i]);
					}

			}

			printf("\n");
			
							
return 0;

}


