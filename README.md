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
- different buffer sizes, starting from 1
- different widths, starting from 1
- word in a file exceeeds the desired width (should continue & return failure)
- word in a file would need to move to next line as if printed on current line, it would exceed directed width
- in directories, ensuring that files starting with '.' and 'wrap' are skipped
- in directories, ensuring that the output file is overwritten each time
- in directories, ensuring that the wrapping continues even if one file fails

#### Strategy
We ensured the correctness of our program by outputting the the result to a txt file and comparing using the cmp bash command with a file that contained the desired result. We also opened the output file in a text editor that allows us to see if there are extra whitespace characters and we ensured that this was not the case.

We ensured the robustness of our buffer by setting it to different buffer sizes including some that are less than, greater than, or equal to the width of the largest word in the input file.

We ensured the consistency of our program by redirecting the output of our program to a file, output1. We then ran our program on the output1 and redirected the output of that to another file, output2. We then ran the cmp bash command on the two files, output1 and output2, and ensured that the files were exactly the same.
