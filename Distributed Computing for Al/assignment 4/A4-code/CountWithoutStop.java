import java.io.IOException;
import java.io.BufferedReader;
import java.io.FileReader;
import java.util.StringTokenizer;
import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.HashSet;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

// MSC inArtificial Intelligence
// Course: Distributed Computing for AI ITSC-2112
// january 2024
// Mintesnot fikir abate
// Linkedin: https://www.linkedin.com/in/mintesnot-fikir
// GitHub: https://github.com/mintesnot96





// A Hadoop MapReduce program that counts words in input files excluding stop words.
public class WordCountWithoutStopWords {
  
  // Mapper that tokenizes input text and filters out stop words.
  public static class TokenizerMapper extends Mapper<Object, Text, Text, IntWritable>{
    private final static IntWritable one = new IntWritable(1);
    private Text word = new Text();
    // Set to hold stop words for filtering.
    private Set<String> stopWords = new HashSet<>();
    
    // Setup method to load stop words from a file specified in the job configuration.
    @Override
    protected void setup(Context context) throws IOException, InterruptedException {
      Configuration conf = context.getConfiguration();
      String stopWordsPath = conf.get("stopwords.file");
      if (stopWordsPath != null && !stopWordsPath.isEmpty()) {
        try (BufferedReader reader = new BufferedReader(new FileReader(stopWordsPath))) {
          String line;
          while ((line = reader.readLine()) != null) {
            stopWords.add(line.trim());
          }
        }
      }
    }

    // Map method to write words and their counts, excluding stop words.
    public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
      StringTokenizer itr = new StringTokenizer(value.toString());
      while (itr.hasMoreTokens()) {
        String currentWord = itr.nextToken().replaceAll("[^a-zA-Z]","").toLowerCase();
        if (!currentWord.isEmpty() && !stopWords.contains(currentWord)) {
          word.set(currentWord);
          context.write(word, one);
        }
      }
    }
  }

  // Reducer to sum up word counts.
  public static class IntSumReducer extends Reducer<Text, IntWritable, Text, IntWritable> {
    public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException {
      int sum = 0;
      for (IntWritable val : values) {
        sum += val.get();
      }
      context.write(key, new IntWritable(sum));
    }
  }

  // Main method to set up and start the MapReduce job.
  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    conf.set("stopwords.file", "/home/mifaroot/A4mapreduce/stopwords.txt");
    Job job = Job.getInstance(conf, "Word Count Excluding Stop Words");
    job.setJarByClass(WordCountWithoutStopWords.class);
    job.setMapperClass(TokenizerMapper.class);
    job.setReducerClass(IntSumReducer.class);
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(IntWritable.class);
    FileInputFormat.addInputPath(job, new Path(args[0]));
    FileOutputFormat.setOutputPath(job, new Path(args[1]));
    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }
}
