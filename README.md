Project  - Part of Speech Tagging

Programming Language: Python v3.14

Description:
There is one .py file called POS_Tagger.py containing a single function and a single import.  The import call is to a standard module 
called csv which allows the program to read from a .csv file.  This is used to read the data contained in the transition_matrix.csv file 
and the emission_matrix.csv file.  These data are stored as simple arrays containing arrays as elements.  Separate arrays track the indices 
corresponding to words and part-of-speech types.  The POS_Tagger.py file has a single function called 
viterbi(text_file, possible_input_tags_file = None) which performs the viterbi algorithm for part-of-speech tagging on the text contained 
in text_file.  The argument possible_input_tags_file is an optional argument for a text file with a subset of POS types under which to 
restrict the viterbi algorithm.  The input text_file should be named according to the format: input_<number>.  The POS_Tagger.py file will 
write the associated tags to a text file named: output_<number>.

To Run: 
Put POS_Tagger.py, transition_matrix.csv, emission_matrix.csv, and any text files for testing the program in the same folder.  
Make sure that python3 is installed.  Open the command prompt and use the "cd" function to change directories to the folder containing the 
relevant files.  Type "python" and hit enter.  Type "import_POS_Tagger" and hit enter.  For a test input with number x, type 
POS_Tagger.viterbi('input_x.txt', '<your possible tag file here>').

Runtime Analysis:
We will consider input sizes as
Number of words in emission matrix: size n
Number of possible parts-of-speech: size m
Number of words in given sample text: size T

The creation of the transition matrix has runtime O(m^2).  The creation of the emission matrix has runtime O(mn).  The initialization of
the DP tables: the max probabilities delta and the the maximizer states psi each take O(mn) time.  The traceback at the end takes time O(T).  
For the recursion in the Viterbi algorithm, the outer for loop runs O(T) times.  The inner for loop runs O(m) times.  Within this loop, the
calculation to find the maximizing previous state takes O(m) times.  Thus, the recursion has runtime O(m^2T).  Hence, the overall runtime
is O(mn + m^2T).
