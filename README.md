# CPlane
A C interpreter

<h4>Contributors: Ethan, Logan, Sean, Felix, Martin</h4>

<h2>Functionality</h2>
This program will take an input c file, then perform the following:
<ul>
 <li>Remove non-code objects such as comments.</li>
 <li>Tokenize the code.</li>
 <li>Check for syntax errors.</li>
 <li>Build an abstract syntax tree and symbol table for the program.</li>
 <li>Run the program.</li>
</ul>

<h2>To compile</h2>
(in source directory) $ make

<h2>To run</h2>

```
./cplane (path/to/c_file.c)
```
Or, to run with Debug mode enabled
```
./cplane (path/to/c_file.c) -d
```

The generated files will be in (source dir)/output_files
 
<h3>Note about generated outputs</h3>
Some outputs (such as the CST and AST) are larger than will fit on any normal sized monitor. You may need to scroll to view the entire output.
