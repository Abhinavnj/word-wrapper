# C Word Wrapper

*Completed in collaboration between Abhinav Sirohi (as3249) & Srinandini Marpaka (sm2237) for Systems Programmong (01:198:214)*

### Testing
#### Test Cases
- empty file
- 0, 1, or multiple newline characters at the beginning of the file
- 0, 1, or multiple newline characters at the end of the file
- 0, 1, or multiple newline characters in the middle of the file
- 0, 1, or multiple whitespace characters at the beginning of the file
- 0, 1, or multiple whitespace characters at the end of the file
- 0, 1, or multiple whitespace characters in the middle of the file
- invalid arguments (width <= 0)
- case where the final word would need to be moved to a new line if the desired width was exceeded
- a singular word exceeds the desired width, the program should continue and return exit failure
- ensuring that all the files starting with '.' or 'wrap' are skipped
- ensuring that the output gets overwritten each time the program is run on it
- going through each of the files even if one of the files is not able to be wrapped properly
- the word length exceeds buffer size
- the width is less than or equal to 0
- buffer length 1
#### Strategy
We ensured the correctness of our program by outputting the the result to a txt file and comparing using the cmp bash command with a file that contained the desired result. We also opened the output file in a text editor that allows us to see if there are extra whitespace characters and we ensured that this was not the case.

We ensured the robustness of our buffer by setting it to different buffer sizes including some that are less than, greater than, or equal to the width of the largest word in the input file.

We ensured the consistency of our program by redirecting the output of our program to a file, output1. We then ran our program on the output1 and redirected the output of that to another file, output2. We then ran the cmp bash command on the two files, output1 and output2, and ensured that the files were exactly the same.
