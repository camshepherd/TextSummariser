# TextSummariser
C++ text summariser implementing tf and tf-idf algorithms and generating variable length summarisation reports.
# About 
This was generated during my third year of University and is a re-upload.
This was developed for Windows but does not have any dependencies requiring that a Windows system is used.
# Usage
The completed exe requires command line arguments to run. 
* --tf should be added to use the term frequency algorithm 
* --tf-idf should be added to use the term frequency-inverse document frequency algorithm
* --length specifies the length of the report to be generated for each document
Each document to be added should be included separately. e.g. to generate a 1000 word summary document on each of the eight given test data using tf-idf, 
give the parameters:
```
"Web Technologies_x64.exe" "Doc 1.txt" "Doc 2.txt" "Doc 3.txt" "Doc 4.txt" "Doc 5.txt" "Doc 6.txt" "Doc 7.txt" "Doc 8.txt" --tf --length 1000
```
