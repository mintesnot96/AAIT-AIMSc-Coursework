import java.io.IOException;
import java.util.StringTokenizer;



// MSC inArtificial Intelligence
// Course: Distributed Computing for AI ITSC-2112
// january 2024
// Mintesnot fikir abate
// Linkedin: https://www.linkedin.com/in/mintesnot-fikir
// GitHub: https://github.com/mintesnot96




// Hadoop configuration and data types for map and reduce functions
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class WordCount {

  // Mapper class that tokenizes each line of input text into words
  public static class WordMapper extends Mapper<Object, Text, Text, IntWritable>{

    // A constant to represent a count of one for each word encountered
    private final static IntWritable one = new IntWritable(1);
    // A Text object to store each word processed
    private Text processedWord = new Text();

    // The map method processes text input, one line at a time
    public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
      // Tokenizes the input text
      StringTokenizer words = new StringTokenizer(value.toString());
      while (words.hasMoreTokens()) {
        // Cleans each token by removing non-alphabetic characters and converting to lowercase
        String cleanWord = words.nextToken().replaceAll("[^a-zA-Z]", "").toLowerCase();
        // Skips any empty tokens
        if (!cleanWord.isEmpty()) {
          // Sets the cleaned word into the Text object
          processedWord.set(cleanWord);
          // Writes the word as key and one as value to the context
          context.write(processedWord, one);
        }
      }
    }
  }

  // Reducer class that sums up the counts for each word
  public static class CountReducer extends Reducer<Text, IntWritable, Text, IntWritable> {
    // A writable integer to store the sum of counts for a word
    private IntWritable totalCount = new IntWritable();

    // The reduce method processes each key (word) and its list of counts
    public void reduce(Text key, Iterable<IntWritable> counts, Context context) throws IOException, InterruptedException {
      int sum = 0;
      // Iterates over the counts and sums them
      for (IntWritable count : counts) {
        sum += count.get();
      }
      // Sets the sum as the total count for the word
      totalCount.set(sum);
      // Writes the word and its total count to the context
      context.write(key, totalCount);
    }
  }

  // The main method to set up and start the MapReduce job
  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    // Defines a new job named "Enhanced Word Count"
    Job job = Job.getInstance(conf, "Enhanced Word Count");
    // Sets the jar by the class
    job.setJarByClass(WordCount.class);
    // Sets mapper, combiner, and reducer classes
    job.setMapperClass(WordMapper.class);
    job.setCombinerClass(CountReducer.class);
    job.setReducerClass(CountReducer.class);
    // Sets the types of output key and value
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(IntWritable.class);
    // Sets the paths to input and output
    FileInputFormat.addInputPath(job, new Path(args[0]));
    FileOutputFormat.setOutputPath(job, new Path(args[1]));
    // Exits with the job completion status
    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }
}
