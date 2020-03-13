# TestManager
School project for introduction to system programming course.

The program gets a file with student names, ID'S and grades in different subjects at school and it calculates the mean average
for each student seperatly, using a thread for each student.
The main purpose of the project was to teach us about thread's and basic work with them.

## How to compile and run
First initalize the TestManager, by running the program with one argument, Grades_Directory - the directory in which all the students
grades are stored. The TestManager will initialize the TestGrade program for each student seperatly, using a different thread for
each student such that the grades will be computed ain a parallel way.

## IDE
Written and compiled in Visual Studio 2017
