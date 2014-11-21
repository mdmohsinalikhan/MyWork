package org.apache.hadoop.examples;

import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class AuthorUrl {

  public static class TokenizerMapper 
       extends Mapper<Object, Text, Text, Text>{
    
    //private final static IntWritable one = new IntWritable(1);
    private Text word = new Text("xyz");
    private Text word1 = new Text("author");
    private Text word2 = new Text("url");
    private Text keys = new Text();
    private Text value = new Text();
    private Text keyvalue = new Text();
    private Text nullcheck = new Text("");
      
	    public void map(Object key, Text value, Context context
		            ) throws IOException, InterruptedException {
	      String[] lines = value.toString().split(System.getProperty("line.separator"));
	      

	    int i=0,j=0;
		     
		for(i = 0;i < lines.length;i++){	
		     j = 0;
		     StringTokenizer itr = new StringTokenizer(lines[i],"\"");
		
				while (itr.hasMoreTokens()) {
				word.set(itr.nextToken());

					if(word.equals(word1)){
						word.set(itr.nextToken());
						word.set(itr.nextToken());
						keys.set(word);
						j = 1;	
					}

					if(word.equals(word2)){
						word.set(itr.nextToken());
						word.set(itr.nextToken());
						value.set("\"" + word + "\"");
						if(j == 1){j=2;}
					}

					if(j==2){
						context.write(keys,value);
						break;
					}

				}
		 }

	    }
  }
  
  public static class IntSumReducer 
       extends Reducer<Text,Text,Text,Text> {
    
	private Text result = new Text();
   
    public void reduce(Text key, Iterable<Text> values, 
                       Context context
                       ) throws IOException, InterruptedException {
      String urls = "\"";
      String initcheck = "\"";
      for (Text val : values) {
	
	if(val.equals(null))
	{

	}
	else{
		if(urls.equals(initcheck)){
		 urls =  val.toString();
		}else{
		 urls =  urls + "," + val.toString();
		}
	}
      }
      result.set(urls);
      context.write(key, result);
    }
  }

  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
    if (otherArgs.length != 2) {
      System.err.println("Usage: wordcount <in> <out>");
      System.exit(2);
    }
    Job job = new Job(conf, "word count");
    job.setJarByClass(AuthorUrl.class);
    job.setMapperClass(TokenizerMapper.class);
    job.setCombinerClass(IntSumReducer.class);
    job.setReducerClass(IntSumReducer.class);
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(Text.class);
    FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
    FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }
}
