import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;



// MSC inArtificial Intelligence
// Course: Distributed Computing for AI ITSC-2112
// january 2024
// Mintesnot fikir abate
// Linkedin: https://www.linkedin.com/in/mintesnot-fikir
// GitHub: https://github.com/mintesnot96


public class SortWord {
    public static void main(String[] args) {
        // Check if exactly one argument (the file name) is provided
        if (args.length != 1) {
            // Exit the program if the condition is not met
            System.exit(1);
        }

        // Store the provided filename from command line arguments
        String filePath = args[0];

        // Try-with-resources statement to ensure the BufferedReader is closed after use
        try (BufferedReader fileReader = new BufferedReader(new FileReader(filePath))) {
            String currentLine;
            // Create a map to store words and their corresponding counts
            Map<String, Integer> wordFrequencies = new HashMap<>();

            // Read the file line by line
            while ((currentLine = fileReader.readLine()) != null) {
                // Split the line into word and count based on tab delimiter
                String[] wordAndCount = currentLine.split("\t");
                if (wordAndCount.length == 2) {
                    // Extract the word and its count, trimming any leading/trailing whitespace
                    String word = wordAndCount[0];
                    int count = Integer.parseInt(wordAndCount[1].trim());
                    // Store the word and count in the map
                    wordFrequencies.put(word, count);
                }
            }

            // Convert the map entries to a list for sorting
            List<Map.Entry<String, Integer>> entriesSortedByCount = new ArrayList<>(wordFrequencies.entrySet());
            // Sort the list in descending order of count
            entriesSortedByCount.sort(Comparator.comparing(Map.Entry::getValue, Comparator.reverseOrder()));

            // Extract the top 25 entries, or the total number of entries if fewer than 25
            List<Map.Entry<String, Integer>> top25Words = entriesSortedByCount.subList(0, Math.min(25, entriesSortedByCount.size()));
            System.out.println("The top 25 words with highest counts:");

            // Iterate over the top 25 entries and print each word and its count
            for (Map.Entry<String, Integer> entry : top25Words) {
                System.out.println(entry.getKey() + " - " + entry.getValue());
            }
        } catch (IOException e) {
            // Print the stack trace if an IOException is encountered
            e.printStackTrace();
        }
    }
}
